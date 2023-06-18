//---------------------------------------------------------
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### COMMS.C ##################################
//---------------------------------------------------------

// Includes --------------------------------------------------------------------
#include "comms.h"
#include "usart.h"
#include "hard.h"
#include "utils.h"
#include "pwm.h"

#include "filters_and_offsets.h"

#include <string.h>
#include <stdio.h>


// Externals -------------------------------------------------------------------
extern volatile unsigned char last_ch_values [];


// Globals ---------------------------------------------------------------------


// Module Private Functions ----------------------------------------------------



// Module Functions ------------------------------------------------------------
void Comms_Update (void)
{
    unsigned char readed = 0;
    char buff_local [127] = { 0 };
    
    if (Usart1HaveData())
    {
        Led_On();
        Usart1HaveDataReset();
        readed = Usart1ReadBuffer(buff_local, 127);
        if (readed == sizeof("ch1 255 ch2 255 sum 511"))    //readed incluye \0
        {
            unsigned short ch1 = 0;
            unsigned short ch2 = 0;
            unsigned short sum = 0;
            int qtty = 0;

            qtty = StringIsANumber(buff_local + sizeof("ch1 ") - 1, &ch1);
            if (qtty == 3)
            {
                qtty = StringIsANumber(buff_local + sizeof("ch1 255 ch2 ") - 1, &ch2);
                if (qtty == 3)
                {
                    qtty = StringIsANumber(buff_local + sizeof("ch1 255 ch2 255 sum ") - 1, &sum);
                    if (qtty == 3)
                    {
                        if ((ch1 + ch2) == sum)
                        {
                            last_ch_values[0] = ch1;
                            last_ch_values[1] = ch2;

                        }
                        else
                            Usart1Send("err verif\n");
                    }
                    else
                        Usart1Send("err sum\n");
                }
                else
                    Usart1Send("err ch2\n");
            }
            else
                Usart1Send("err ch1\n");
        }

        if (!strncmp(buff_local, "current config ", sizeof("current config ") - 1))
        {
            char cint = *(buff_local + (sizeof("current config ") - 1));
            char cdec = *(buff_local + (sizeof("current config 8.") - 1));            
            if ((cint >= '1') && (cint <= '8') &&
                (cdec >= '0') && (cdec <= '9'))
            {
                FiltersAndOffsets_Set_Current(cint - '0', cdec - '0');
                if (cint == '8')
                {
                    PWM_Map_Post_Filter_Top_Multiplier(97, 16);    // for 100pts min
                    // PWM_Map_Post_Filter_Top_Multiplier(101, 16, 4);    // for 33pts min                    
                    PWM_Soft_Handler_Low_Freq_Roof_Set (256);
                }
                else if (cint == '7')
                {
                    PWM_Map_Post_Filter_Top_Multiplier(99, 14);    // for 100pts min
                    // PWM_Map_Post_Filter_Top_Multiplier(103, 14, 3);    // for 33pts min                    
                    PWM_Soft_Handler_Low_Freq_Roof_Set (256);
                }
                else if (cint == '6')
                {
                    PWM_Map_Post_Filter_Top_Multiplier(101, 12);    // for 100pts min
                    // PWM_Map_Post_Filter_Top_Multiplier(105, 12, 3);    // for 33pts min                    
                    PWM_Soft_Handler_Low_Freq_Roof_Set (256);
                }
                else if (cint == '5')
                {
                    PWM_Map_Post_Filter_Top_Multiplier(103, 10);    // for 100pts min
                    // PWM_Map_Post_Filter_Top_Multiplier(109, 10, 3);    // for 33pts min                    
                    PWM_Soft_Handler_Low_Freq_Roof_Set (256);
                }
                else if (cint == '4')
                {
                    PWM_Map_Post_Filter_Top_Multiplier(106, 8);    // for 100pts min
                    // PWM_Map_Post_Filter_Top_Multiplier(113, 8, 3);    // for 33pts min                    
                    PWM_Soft_Handler_Low_Freq_Roof_Set (256);
                }
                else if (cint == '3')
                {
                    PWM_Map_Post_Filter_Top_Multiplier(111, 6);    // for 100pts min
                    // PWM_Map_Post_Filter_Top_Multiplier(120, 6, 2);    // for 33pts min                    
                    PWM_Soft_Handler_Low_Freq_Roof_Set (256);
                }
                else if (cint == '2')
                {
                    PWM_Map_Post_Filter_Top_Multiplier(108, 4);    // for 100pts min
                    // PWM_Map_Post_Filter_Top_Multiplier(119, 4, 2);    // for 33pts min                    
                    PWM_Soft_Handler_Low_Freq_Roof_Set (128);
                }
                else if (cint == '1')
                {
                    PWM_Map_Post_Filter_Top_Multiplier(110, 2);    // for 100pts min
                    // PWM_Map_Post_Filter_Top_Multiplier(135, 2, 1);    // for 33pts min                    
                    PWM_Soft_Handler_Low_Freq_Roof_Set (64);
                }

                Usart1Send("ok\n");
            }
            else
            {
            }
        }

        if (!strncmp(buff_local, "version", sizeof("version") - 1))
        {
            sprintf(buff_local,"%s %s\n", HARD_GetHardwareVersion(), HARD_GetSoftwareVersion());
            Usart1Send(buff_local);
        }
            
        Led_Off();;
    }
}


//---- end of file ----//
