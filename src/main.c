//------------------------------------------------
// #### DEXEL 2CH POTE PROJECT - Custom Board ####
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### MAIN.C ###################################
//------------------------------------------------

// Includes --------------------------------------------------------------------
#include "hard.h"
#include "stm32f0xx.h"

#include "gpio.h"
#include "tim.h"
#include "adc.h"
#include "dma.h"
#include "usart.h"

#include "test_functions.h"
#include "temperatures.h"
#include "dsp.h"
#include "pwm.h"

#include "filters_and_offsets.h"
#include "comms.h"

#include <stdio.h>
#include <string.h>


// Module Types Constants and Macros -------------------------------------------
typedef enum {
    MAIN_HARD_INIT,
    MAIN_RUNNING,
    MAIN_IN_OVERTEMP
    
} main_state_e;


// // Colors temp ticks
// // #define START_OF_PWM_DIMMER    16
// // #define START_OF_PWM_DIMMER    20
// #define START_OF_PWM_DIMMER    24    //well cleaning the minimum temp
// // #define START_OF_PWM_DIMMER    28    //well cleaning the minimum temp
// #define ANALOG_FOR_PWM_DIMMER    255
// #define START_OF_CONTINUOS_DIMMER    (START_OF_PWM_DIMMER + ANALOG_FOR_PWM_DIMMER)

// Externals -------------------------------------------------------------------
//   for timers
volatile unsigned short wait_ms_var = 0;
//   for dmx info 
volatile unsigned char last_ch_values [2] = { 0 };


// Globals ---------------------------------------------------------------------
//-- Timers globals ----------------------------------
volatile unsigned short timer_standby = 0;
volatile unsigned short timer_check_temp = 0;


// Module Private Functions ----------------------------------------------------
void TimingDelay_Decrement(void);
void SysTickError (void);


// Module Functions ------------------------------------------------------------
int main(void)
{
    //GPIO Configuration.
    GPIO_Config();

    //Systick Timer
    if (SysTick_Config(48000))
        SysTickError();

    // Hardware Tests
    // TF_Hardware_Tests ();
    
    // Hardware Inits. ---------------------------
    // Start of Complete Channel 2
    TIM_14_Init ();
    TIM_1_Init_pwm_neg_CH1_trig_CH2 ();
    
    // Start of Complete Channel 1
    TIM_16_Init ();
    TIM_3_Init_pwm_neg_CH1_trig_CH2 ();

    // Start Usart
    Usart1Config();
    
    // Init TIM 17 for Soft or Int init
    TIM_17_Init ();

    PWM_Soft_Set_Channels (1, 0);
    PWM_Soft_Set_Channels (2, 0);

    main_state_e main_state = MAIN_HARD_INIT;

    while (1)
    {
        switch (main_state)
        {
        case MAIN_HARD_INIT:
            FiltersAndOffsets_Filters_Reset ();

            // tim17 for soft pwm
            TIM17Enable();
            
            main_state++;
            break;

        case MAIN_RUNNING:
            if (!timer_standby)
            {
                timer_standby = 1;
                FiltersAndOffsets_Post_Mapping_SM (last_ch_values);
            }            
            break;

        case MAIN_IN_OVERTEMP:

            // if (!timer_check_temp)
            // {
            //     if (Temp_Channel < TEMP_RECONNECT)
            //         main_state = MAIN_HARD_INIT;
                
            //     timer_check_temp = 2000;    //check again in two seconds            
            // }
            break;

        default:
            main_state = MAIN_HARD_INIT;
            break;
        }

        Comms_Update();

    }    //end of while 1

    return 0;
}

//--- End of Main ---//


void TimingDelay_Decrement(void)
{
    if (wait_ms_var)
        wait_ms_var--;

    if (timer_standby)
        timer_standby--;

    if (timer_check_temp)
        timer_check_temp--;
    
}


void SysTickError (void)
{
    // Capture systick error...
    while (1)
    {
        if (LED)
            LED_OFF;
        else
            LED_ON;

        for (unsigned char i = 0; i < 255; i++)
        {
            asm ("nop \n\t"
                 "nop \n\t"
                 "nop \n\t" );
        }
    }
}

//--- end of file ---//

