//------------------------------------------------
// #### PROJECT: O3 TREATMENT - Custom Board #####
// ## Internal Test Functions Module
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### TEST_FUNCTIONS.C #########################
//------------------------------------------------

// Includes --------------------------------------------------------------------
#include "test_functions.h"
#include "hard.h"
#include "tim.h"
#include "pwm.h"
#include "adc.h"
#include "dma.h"
#include "temperatures.h"
#include "usart.h"
#include "utils.h"

#include "dsp.h"

#include <stdio.h>


// Externals -------------------------------------------------------------------
extern volatile unsigned short adc_ch [];

extern ma32_u16_data_obj_t pote_1_filter;
extern ma32_u16_data_obj_t pote_2_filter;

// extern volatile unsigned char f_channel_2_int;
// extern volatile unsigned char f_channel_4_int;
// Globals ---------------------------------------------------------------------

// Module Private Types & Macros -----------------------------------------------


// Module Private Functions ----------------------------------------------------
void TF_Led (void);
void TF_TIM16_Pwm_CH1N (void);
void TF_TIM14_Pwm_CH1 (void);
void TF_TIM3_CH1_ConstantOff_TIM3_CH2_TriggerInput (void);
void TF_TIM1_CH1_ConstantOff_TIM1_CH2_TriggerInput (void);
void TF_Two_Complete_Channels_Hardware (void);

void TF_Two_Complete_Channels_Hardware_With_Offset (void);
void TF_TIM17_Interrupt (void);
void TF_TIM17_Interrupt_Soft_Pwm (void);
void TF_Two_Complete_Channels_Hardware_With_Offset_Soft_PWM (void);

void TF_Usart1_Tx (void);
void TF_Usart1_Tx_Rx_Int (void);
void TF_Two_Complete_Channels_Usart (void);

// Module Functions ------------------------------------------------------------
void TF_Hardware_Tests (void)
{
    // Begin Hardware Tests - check test_functions module
    // TF_Led();    //simple led functionality
    // TF_TIM16_Pwm_CH1N ();
    // TF_TIM14_Pwm_CH1 ();
    // TF_TIM3_CH1_ConstantOff_TIM3_CH2_TriggerInput ();
    // TF_TIM1_CH1_ConstantOff_TIM1_CH2_TriggerInput ();
    // TF_Two_Complete_Channels_Hardware ();

    // TF_Usart1_Tx ();
    // TF_Usart1_Tx_Rx_Int ();
    TF_Two_Complete_Channels_Usart ();
    
    // TF_Two_Complete_Channels_Hardware_With_Offset ();
    // TF_TIM17_Interrupt ();
    // TF_TIM17_Interrupt_Soft_Pwm ();
    // TF_Two_Complete_Channels_Hardware_With_Offset_Soft_PWM ();
    // End Hardware Tests -------------------------------
}


void TF_Led (void)
{
    while (1)
    {
        if (LED)
            LED_OFF;
        else
            LED_ON;

        Wait_ms(1000);
    }
}


void TF_TIM16_Pwm_CH1N (void)
{
    TIM_16_Init ();
    Update_TIM16_CH1N (2048);

    while (1);
    
}


void TF_TIM14_Pwm_CH1 (void)
{
    TIM_14_Init ();
    Update_TIM14_CH1 (2048);

    while (1);
    
}


void TF_TIM3_CH1_ConstantOff_TIM3_CH2_TriggerInput (void)
{
    TIM_16_Init ();
    Update_TIM16_CH1N (4095);
    TIM_3_Init_pwm_neg_CH1_trig_CH2 ();

    while (1)
    {
        Wait_ms(1);
    }
}


void TF_TIM1_CH1_ConstantOff_TIM1_CH2_TriggerInput (void)
{
    TIM_14_Init ();
    Update_TIM14_CH1 (4095);
    TIM_1_Init_pwm_neg_CH1_trig_CH2 ();

    while (1)
    {
        Wait_ms(1);
    }
}


void TF_Two_Complete_Channels_Hardware (void)
{
    // Start of Complete Channel 2
    TIM_14_Init ();
    TIM_1_Init_pwm_neg_CH1_trig_CH2 ();
    
    // Start of Complete Channel 1
    TIM_16_Init ();
    TIM_3_Init_pwm_neg_CH1_trig_CH2 ();    

    Update_TIM14_CH1 (200);
    Update_TIM16_CH1N (200);
    
    while (1);
    
}


void TF_TIM17_Interrupt (void)
{
    TIM_17_Init ();

    TIM17Enable();
    while (1);
    
}


void TF_TIM17_Interrupt_Soft_Pwm (void)
{
    TIM_17_Init ();

    // Start of Complete Pote Channel 1    
    TIM_14_Init ();
    Update_TIM14_CH1 (125);    
    TIM_1_Init_pwm_neg_CH1_trig_CH2 ();

    // Start of Complete Pote Channel 2
    TIM_16_Init ();
    Update_TIM16_CH1N (125);
    TIM_3_Init_pwm_neg_CH1_trig_CH2 ();    
    
    PWM_Soft_Set_Channels (1, 16);
    PWM_Soft_Set_Channels (2, 0);
    
    TIM17Enable();
    while (1);
    
}


void TF_Two_Complete_Channels_Hardware_With_Offset_Soft_PWM (void)
{
    // Init ADC and DMA
    AdcConfig();
    DMAConfig();
    DMA1_Channel1->CCR |= DMA_CCR_EN;
    ADC1->CR |= ADC_CR_ADSTART;

    // Start of Complete Pote Channel 1
    TIM_14_Init ();
    TIM_1_Init_pwm_neg_CH1_trig_CH2 ();
    
    // Start of Complete Pote Channel 2
    TIM_16_Init ();
    TIM_3_Init_pwm_neg_CH1_trig_CH2 ();

    TIM_17_Init ();

    PWM_Soft_Set_Channels (1, 0);
    PWM_Soft_Set_Channels (2, 0);
    
    TIM17Enable();
    
    unsigned short ch1_input_filtered = 0;
    unsigned short ch2_input_filtered = 0;

    unsigned short bright = 0;
    unsigned short temp0 = 0;
    unsigned short temp1 = 0;

    unsigned int calc = 0;
    

    MA32_U16Circular_Reset (&pote_1_filter);
    MA32_U16Circular_Reset (&pote_2_filter);    
    while (1)
    {
        Wait_ms(5);

        ch1_input_filtered = MA32_U16Circular (&pote_1_filter, Pote_Channel_1);
        ch2_input_filtered = MA32_U16Circular (&pote_2_filter, Pote_Channel_2);

        //Mezcla de colores
        bright = ch1_input_filtered;
        temp0 = 4095 - ch2_input_filtered;
        temp1 = 4095 - temp0;
        
        calc = temp0 * bright;
        calc >>= 12;    // to 4095
        ch1_input_filtered = (unsigned short) calc;
        
        calc = temp1 * bright;
        calc >>= 12;    // to 4095
        ch2_input_filtered = (unsigned short) calc;
        // end of Mezcla colores
        

        if (ch1_input_filtered > 271)
        {
            PWM_Soft_Set_Channels (1, 256);
            Update_TIM14_CH1 (ch1_input_filtered - 16);
        }
        else if (ch1_input_filtered > 15)
        {
            ch1_input_filtered -= 16;
            PWM_Soft_Set_Channels (1, ch1_input_filtered);
            Update_TIM14_CH1 (255);
        }
        else
        {
            PWM_Soft_Set_Channels (1, 0);
            Update_TIM14_CH1 (255);
        }

        
        if (ch2_input_filtered > 271)
        {
            PWM_Soft_Set_Channels (2, 256);
            Update_TIM16_CH1N (ch2_input_filtered - 16);
        }
        else if (ch2_input_filtered > 15)
        {
            ch2_input_filtered -= 16;
            PWM_Soft_Set_Channels (2, ch2_input_filtered);
            Update_TIM16_CH1N (255);
        }
        else
        {
            PWM_Soft_Set_Channels (2, 0);
            Update_TIM16_CH1N (255);
        }
    }
}


void TF_Usart1_Tx (void)
{
    Usart1Config();

    Usart1Send("Test single string send on 2 secs.\n");
    while (1)
    {
        Usart1Send("Mariano\n");
        Wait_ms(2000);
    }
}


void TF_Usart1_Tx_Rx_Int (void)
{
    // start usart1 and loop rx -> tx after 3 secs
    Usart1Config();
    char buff_local [128] = { 0 };
    unsigned char readed = 0;

    Usart1Send("Test string loop. Answers every 3 secs.\n");    
    while(1)
    {
        Wait_ms(2800);
        if (Usart1HaveData())
        {
            LED_ON;
            Usart1HaveDataReset();
            readed = Usart1ReadBuffer(buff_local, 127);
            *(buff_local + readed) = '\n';    //cambio el '\0' por '\n' antes de enviar
            *(buff_local + readed + 1) = '\0';    //ajusto el '\0'
            Usart1Send(buff_local);
            Wait_ms(200);
            LED_OFF;
        }
    }    
}


void TF_Two_Complete_Channels_Usart (void)
{
    // Start of Complete Channel 2
    TIM_14_Init ();
    TIM_1_Init_pwm_neg_CH1_trig_CH2 ();
    
    // Start of Complete Channel 1
    TIM_16_Init ();
    TIM_3_Init_pwm_neg_CH1_trig_CH2 ();    

    Update_TIM14_CH1 (40);
    Update_TIM16_CH1N (40);

    // Start Usart
    Usart1Config();
    char buff_local [128] = { 0 };
    unsigned char readed = 0;

    Usart1Send("Test usart string to pwm channels\n");

    // usart channels values
    unsigned char channel1 = 0;
    unsigned char channel2 = 0;

    while (1)
    {
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
                                channel1 = ch1;
                                channel2 = ch2;

                                // always have a minimun for ints
                                Update_TIM14_CH1 ((channel2 << 4) + 40);
                                Update_TIM16_CH1N ((channel1 << 4) + 40);
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
            else
                Usart1Send("err number of bytes\n");
            
            LED_OFF;
        }
    }
}

//--- end of file ---//
