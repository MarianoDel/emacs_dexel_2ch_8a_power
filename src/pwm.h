//----------------------------------------------------------
// #### DEXEL 2CH PROJECT - Custom Board ####
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### PWM.H ##############################################
//----------------------------------------------------------

// Prevent recursive inclusion -------------------------------------------------
#ifndef __PWM_H_
#define __PWM_H_

// #include "tim.h"

// Exported Types --------------------------------------------------------------



// Exported Constants or Macros ------------------------------------------------
// #define POWER_CONTROL_GENERAL_THRESHOLD    512    // este es un valor total de dmx no pwm

#define EnablePreload_PWM1    EnablePreload_TIM3_CH1
#define EnablePreload_PWM2    EnablePreload_TIM3_CH2

#define DisablePreload_PWM1    DisablePreload_TIM3_CH1
#define DisablePreload_PWM2    DisablePreload_TIM3_CH2

#define PWM_Update_CH1(X)    Update_TIM16_CH1N(X)
#define PWM_Update_CH2(X)    Update_TIM14_CH1(X)

#define EnablePreload_ENA1    EnablePreload_TIM1_CH2
#define EnablePreload_ENA2    EnablePreload_TIM1_CH4

#define DisablePreload_ENA1    DisablePreload_TIM1_CH2
#define DisablePreload_ENA2    DisablePreload_TIM1_CH4

#define PWM_Update_ENA1(X)    Update_TIM1_CH2(X)
#define PWM_Update_ENA2(X)    Update_TIM1_CH4(X)



// Exported Functions ----------------------------------------------------------
void PWMChannelsReset (void);
void PWM_Map_Post_Filter (unsigned short dmx_filtered, unsigned short * pwm_ena, unsigned short * pwm_ch);
void PWM_Map_Post_Filter_Top_Multiplier (unsigned char top_multiplier,
                                         unsigned char min_curr);

void PWM_Soft_Handler_Low_Freq_Roof_Set (unsigned short new_roof);
void PWM_Soft_Set_Channels (unsigned char ch, unsigned short value);
void PWM_Int_Handler (void);
void PWM_Int_Handler_2 (void);
void PWM_Int_Handler_2_255 (void);
void PWM_Soft_Handler_Low_Freq (void);

void PWM_Soft_Reset_Output_Ch1 (void);
void PWM_Soft_Reset_Output_Ch2 (void);
void PWM_Soft_Set_Output_Ch1 (void);
void PWM_Soft_Set_Output_Ch2 (void);

void PWM_Int_Handler_3_Start (void);
void PWM_Int_Handler_3_Update (void);
void PWM_Int_Handler_3 (void);

#endif    /* __PWM_H_ */

//--- end of file ---//

