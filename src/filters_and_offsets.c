//----------------------------------------------------
// #### DEXEL 2CH PROJECT - Custom Board ####
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### FILTERS_AND_OFFSETS.C ########################
//----------------------------------------------------

// Includes --------------------------------------------------------------------
#include "filters_and_offsets.h"
#include "dsp.h"
#include "pwm.h"
#include "tim.h"


// Module Private Types & Macros -----------------------------------------------
typedef enum {
    FILTERS_BKP_CONFIG_AND_CHANNELS,
    FILTERS_LIMIT_EACH_CHANNEL,
    FILTERS_OUTPUTS,
    FILTERS_SET_PWM,
    FILTERS_DUMMY
    
} filters_and_offsets_e;


// Externals -------------------------------------------------------------------


// Globals ---------------------------------------------------------------------
ma32_u16_data_obj_t ch1_filter;
ma32_u16_data_obj_t ch2_filter;
unsigned short limit_output [2] = { 0 };
unsigned char saved_current_config = 8;    // 4 amps by default
unsigned char running_current_config = 0;


// Module Private Functions ----------------------------------------------------


// Module Functions ------------------------------------------------------------
filters_and_offsets_e filters_sm = FILTERS_BKP_CONFIG_AND_CHANNELS;
void FiltersAndOffsets_Post_Mapping_SM (volatile unsigned char * ch_dmx_val)
{    
    unsigned short ch1_pwm, ch2_pwm, ena1_pwm, ena2_pwm;
    int calc;
    
    switch (filters_sm)
    {
    case FILTERS_BKP_CONFIG_AND_CHANNELS:
        running_current_config = saved_current_config;
        limit_output[0] = *(ch_dmx_val + 0);
        limit_output[1] = *(ch_dmx_val + 1);
        
        filters_sm++;
        break;

    case FILTERS_LIMIT_EACH_CHANNEL:
        // ex. 7.9 amps
        calc = limit_output[0] * running_current_config;    //255 * 158 = 40290
        calc = calc / 10;    //40290 / 10 = 4029
        limit_output[0] = (unsigned short) calc;

        calc = limit_output[1] * running_current_config;
        calc = calc / 10;
        limit_output[1] = (unsigned short) calc;        

        filters_sm++;
        break;

    case FILTERS_OUTPUTS:    // apply filters before mapping        
        // channel 1
        limit_output[0] = MA32_U16Circular (&ch1_filter, limit_output[0]);

        // channel 2
        limit_output[1] = MA32_U16Circular (&ch2_filter, limit_output[1]);
        
        filters_sm++;
        break;
        
    case FILTERS_SET_PWM:
        PWM_Map_Post_Filter (limit_output[0],
                            &ena1_pwm,
                            &ch1_pwm);

        PWM_Soft_Set_Channels (1, ena1_pwm);
        PWM_Update_CH1(ch1_pwm);

        PWM_Map_Post_Filter (limit_output[1],
                            &ena2_pwm,
                            &ch2_pwm);

        PWM_Soft_Set_Channels (2, ena2_pwm);
        PWM_Update_CH2(ch2_pwm);
        
        filters_sm++;
        break;

    case FILTERS_DUMMY:
        filters_sm = FILTERS_BKP_CONFIG_AND_CHANNELS;
        break;
        
    default:
        filters_sm = FILTERS_BKP_CONFIG_AND_CHANNELS;
        break;
    }
}


void FiltersAndOffsets_Filters_Reset (void)
{
    MA32_U16Circular_Reset(&ch1_filter);
    MA32_U16Circular_Reset(&ch2_filter);
}


void FiltersAndOffsets_Set_Current (unsigned char new_current_int, unsigned char new_current_dec)
{
    // ex. 7.9 amps
    int calc = new_current_int * 10;    // 70
    calc = calc + new_current_dec;    //79
    calc = calc * 2;    //158
    saved_current_config = (unsigned char) calc;
}


//--- end of file ---//
