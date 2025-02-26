//----------------------------------------------
// #### DEXEL 2CH PROJECT - Custom Board ####
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### HARD.H #################################
//----------------------------------------------
#ifndef _HARD_H_
#define _HARD_H_

#include "stm32f0xx.h"


//-- Defines For Configuration -------------------
//---- Configuration for Hardware Versions -------
// #define HARDWARE_VERSION_2_1    // tant cap with connections
#define HARDWARE_VERSION_2_0    //micro F030K6T6, original production


#define SOFTWARE_VERSION_1_2    // starts with channels disable, avoid the startup pulse on leds
                                // soft pwm with always on
// #define SOFTWARE_VERSION_1_1    // current conf on decimals 4.0A
// #define SOFTWARE_VERSION_1_0    // original production


//---- Features Configuration -----------------

// -- Freq for the timers
// #define USE_PWM_16000_FREQ_1KHZ
#define USE_PWM_4000_FREQ_4KHZ
// #define USE_PWM_8000_FREQ_2KHZ
// #define USE_


//---- End of Features Configuration ----------


// Exported Pinout Names -------------------------------------------------------
// #ifdef HARDWARE_VERSION_1_0
//GPIOA pin0    
//GPIOA pin1    

//GPIOA pin2    Usart1 tx
//GPIOA pin3    Usart1 rx

//GPIOA pin4
//GPIOA pin5    
//GPIOA pin6    NC

//GPIOA pin7    TIM14_CH1

//GPIOB pin0
//GPIOB pin1

//GPIOA pin8    TIM1_CH1

//GPIOA pin9    TIM1_CH2

//GPIOA pin10
//GPIOA pin11    NC

//GPIOA pin12
#define LED    ((GPIOA->ODR & 0x1000) != 0)
#define LED_ON    (GPIOA->BSRR = 0x00001000)
#define LED_OFF    (GPIOA->BSRR = 0x10000000)

//GPIOA pin13
//GPIOA pin14    
//GPIOA pin15    NC

//GPIOB pin3    

//GPIOB pin4    TIM3_CH1

//GPIOB pin5    TIM3_CH2

//GPIOB pin6    TIM16_CH1N

//GPIOB pin7

// #endif    //HARDWARE_VER_1_0

// Exported Types & Macros -----------------------------------------------------


//--- Stringtify Utils -----------------------
#define str_macro(s) #s


// Module Exported Functions ---------------------------------------------------
void Led_On (void);
void Led_Off (void);
unsigned char Led_Is_On (void);
char * HARD_GetHardwareVersion (void);
char * HARD_GetSoftwareVersion (void);


#endif    /* _HARD_H_ */

//--- end of file ---//

