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
        LED_ON;
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
            char current = *(buff_local + (sizeof("current config ") - 1)); 
            if (current == '4')
            {
                FiltersAndOffsets_Set_Current(4);
                PWM_Map_Post_Filter_Top_Multiplier(103);                
            }
            else if (current == '8')
            {
                FiltersAndOffsets_Set_Current(8);
                PWM_Map_Post_Filter_Top_Multiplier(94);
            }
            else
            {
            }
        }
            
        LED_OFF;
    }
}




// resp_e ParseCommsWithChannels (char * str, unsigned char channel)
// {
//     resp_e resp = resp_error;
//     char dummy_str [30] = { 0 };
    
//     //temp,055.00\r\n
//     if (!strncmp(str, (const char *)"temp", (sizeof("temp") - 1)))
//     {
//         if ((*(str + 4) == ',') &&
//             (*(str + 8) == '.') &&
//             (*(str + 11) == '\r'))
//         {
//             unsigned char temp_i = 0;
//             unsigned char temp_d = 0;

//             if (ParseCurrentTemp(str, &temp_i, &temp_d) == resp_ok)
//             {
//                 AntennaSetCurrentTemp (channel, temp_i, temp_d);

//                 sprintf(dummy_str, ",%d\r\n", channel);
//                 strcpy((str + 11), dummy_str);
//                 RpiSend(str);
//                 resp = resp_ok;
//             }
//         }
//     }

//     //ant0,012.27,087.90,001.80,065.00\r\n.
//     else if (!strncmp(str, (const char *)"ant", (sizeof("ant") - 1)))
//     {
//         sprintf(dummy_str, "new antenna ch%d\r\n", channel + 1);
//         RpiSend(dummy_str);
        
//         if ((*(str + 4) == ',') &&
//             (*(str + 11) == ',') &&
//             (*(str + 18) == ',') &&
//             (*(str + 25) == ','))
//         {
//             antenna_st antenna_aux;
//             if (ParseAntennaParams ((char *) buff, &antenna_aux) == resp_ok)
//             {
//                 AntennaSetParamsStruct (channel, &antenna_aux);
//                 resp = resp_ok;
//             }
//         }
//     }

//     //respuesta al keepalive
//     else if (!strncmp(str, (const char *)"ok", (sizeof("ok") - 1)))
//     {
//         AntennaIsAnswering(channel);
//         resp = resp_ok;
//     }

//     else if ((!strncmp(str, (const char *)"name:", sizeof("name:") - 1)))
//     {
//         AntennaSetName(channel, (str + (sizeof("name:") - 1)));
//         resp = resp_ok;
//     }

//     return resp;
// }



//---- end of file ----//
