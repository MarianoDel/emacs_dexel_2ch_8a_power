//-------------------------------------------------
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ## @CPU:    TEST PLATFORM FOR FIRMWARE
// ##
// #### TESTS_PWM_MAPPING.C #######################
//-------------------------------------------------

// Includes Modules for tests --------------------------------------------------
#include "pwm.h"


// Includes tests helper modules for the tests ---------------------------------
#include "tests_vector_utils.h"


#include <stdio.h>
#include <math.h>
#include <stdint.h>


// Types Constants and Macros --------------------------------------------------
// Select simulation lenght in mseconds
#define SIMULATION_MS    2000

#define VECTOR_LENGTH    SIMULATION_MS
// #define VECTOR_LENGTH    ((int)(SAMPLING_FREQ * SIMULATION_LENGTH))


// Externals -------------------------------------------------------------------


// Globals ---------------------------------------------------------------------
unsigned short dmx_data_ch1 [VECTOR_LENGTH] = { 0 };
unsigned short dmx_data_ch2 [VECTOR_LENGTH] = { 0 };
unsigned short pwm_data_ch1 [VECTOR_LENGTH] = { 0 };
unsigned short pwm_data_ch2 [VECTOR_LENGTH] = { 0 };



// Tests Functions -------------------------------------------------------------
void Test_Dmx_Map_Pre (void);
void Test_Dmx_Map_Post_Ramp (void);
void Test_Dmx_Map_Post_Step (void);
void Test_Dmx_Map_Post_Individual (void);


// Main Function to Test -------------------------------------------------------


// Module Auxiliary Functions for Tests ----------------------------------------


// Module Mock Functions -------------------------------------------------------
void Update_TIM16_CH1N (unsigned short new_pwm);
void Update_TIM14_CH1 (unsigned short new_pwm);



// Module Functions ------------------------------------------------------------
int main (int argc, char *argv[])
{
    printf("Start of PWM mapping...\n");

    // Test_Dmx_Map_Pre ();
    // Test_Dmx_Map_Post_Ramp ();
    // Test_Dmx_Map_Post_Step ();
    Test_Dmx_Map_Post_Individual ();

    return 0;
}


void Test_Dmx_Map_Post_Step (void)
{
    printf("test dmx -> pwm conversion for pwm_ena & pwm_ch\n");

    unsigned short pwm_ena [4096] = { 0 };
    unsigned short pwm_ch [4096] = { 0 };
    
    for (int i = 0; i < 4096; i++)
    {
        // PWM_Map_Post_Filter(2047, &pwm_ena[i], &pwm_ch[i]);
        PWM_Map_Post_Filter(4095, &pwm_ena[i], &pwm_ch[i]);        
    }
    
    ShowVectorUShort("\nVector pwm ena:\n", pwm_ena, 16);
    ShowVectorUShort("\nVector pwm ch:\n", pwm_ch, 16);

    ///////////////////////////
    // Backup Data to a file //
    ///////////////////////////
    FILE * file = fopen("data.txt", "w");

    if (file == NULL)
    {
        printf("data file not created!\n");
        return;
    }

    Vector_UShort_To_File (file, "pwm_ena", pwm_ena, 4096);
    Vector_UShort_To_File (file, "pwm_ch", pwm_ch, 4096);    

    printf("\nRun by hand python3 simul_outputs.py\n");
    
}


void Test_Dmx_Map_Post_Ramp (void)
{
    printf("test dmx -> pwm conversion for pwm_ena & pwm_ch\n");

    unsigned short pwm_ena [4096] = { 0 };
    unsigned short pwm_ch [4096] = { 0 };
    
    for (int i = 0; i < 4096; i++)
    {
        PWM_Map_Post_Filter(i, &pwm_ena[i], &pwm_ch[i]);
    }
    
    ShowVectorUShort("\nVector pwm ena:\n", pwm_ena, 255);
    ShowVectorUShort("\nVector pwm ch:\n", pwm_ch, 255);

    ///////////////////////////
    // Backup Data to a file //
    ///////////////////////////
    FILE * file = fopen("data.txt", "w");

    if (file == NULL)
    {
        printf("data file not created!\n");
        return;
    }

    Vector_UShort_To_File (file, "pwm_ena", pwm_ena, 4096);
    Vector_UShort_To_File (file, "pwm_ch", pwm_ch, 4096);    

    printf("\nRun by hand python3 simul_outputs.py\n");
    
}


void Test_Dmx_Map_Pre (void)
{
    printf("test dmx -> pwm conversion for pwm_ena & pwm_ch\n");

    unsigned short pwm_ena [256] = { 0 };
    unsigned short pwm_ch [256] = { 0 };
    
    for (int i = 0; i < 256; i++)
        PWM_Map_Pre_Filter(i, &pwm_ena[i], &pwm_ch[i]);
    
    ShowVectorUShort("\nVector pwm ena:\n", pwm_ena, 256);
    ShowVectorUShort("\nVector pwm ch:\n", pwm_ch, 256);

    ///////////////////////////
    // Backup Data to a file //
    ///////////////////////////
    FILE * file = fopen("data.txt", "w");

    if (file == NULL)
    {
        printf("data file not created!\n");
        return;
    }

    Vector_UShort_To_File (file, "pwm_ena", pwm_ena, 256);
    Vector_UShort_To_File (file, "pwm_ch", pwm_ch, 256);    

    printf("\nRun by hand python3 simul_outputs.py\n");
    
}


extern unsigned char top_min_curr;
extern unsigned char top_min_curr_bits;
void Test_Dmx_Map_Post_Individual (void)
{
    unsigned short dmx = 1;
    unsigned short dmx_curr_adj = 0;
    unsigned short pwm_ena = 0;
    unsigned short pwm_ch = 0;
    
    printf("\nchanging current from 8 to 1 amps\n");

    // for (int i = 8; i > 0; i--)
    // {
    //     top_curr = i;
    //     PWM_Map_Post_Filter (dmx * top_curr, &pwm_ena, &pwm_ch);
    //     printf("curr: %d, dmx: %d, pwm_ena: %d pwm_ch: %d\n", top_curr, dmx, pwm_ena, pwm_ch);
    // }

    unsigned short roof = 256;
    unsigned char curr_mult = 8;
    float eq_pwm = 0.0;
    float eq_pwm_v [1000] = { 0.0 };
    
    // printf("\n set to 8 amps\n");
    // curr_mult = 16;
    // // PWM_Soft_Handler_Low_Freq_Roof_Set(roof);
    // PWM_Map_Post_Filter_Top_Multiplier(102, curr_mult, 4);

    // for (int i = 0; i < 256 * curr_mult; i += curr_mult)
    // {
    //     dmx = i;
    //     PWM_Map_Post_Filter (dmx, &pwm_ena, &pwm_ch);
    //     eq = pwm_ena * pwm_ch / roof;
    //     eq_pwm = (unsigned short) eq;
    //     printf("curr: %d, dmx: %d, pwm_ena: %d pwm_ch: %d eq_pwm: %d\n",
    //            top_min_curr >> 1,
    //            dmx,
    //            pwm_ena,
    //            pwm_ch,
    //            eq_pwm);
    // }

    // printf("\n set to 4 amps\n");
    // unsigned short points = 50;
    // curr_mult = 8;
    // // PWM_Soft_Handler_Low_Freq_Roof_Set(roof);
    // PWM_Map_Post_Filter_Top_Multiplier(113, curr_mult, 3);    

    // for (int i = 0; i < points; i++)
    // {
    //     dmx = i;
    //     dmx_curr_adj = dmx * curr_mult;
    //     PWM_Map_Post_Filter (dmx_curr_adj, &pwm_ena, &pwm_ch);
    //     eq_pwm = (float) pwm_ena * (float) pwm_ch / (float) roof;
    //     printf("dmx_in: %d, curr_adj: %d, pwm_ena: %d pwm_ch: %d eq_pwm: %.2f\n",
    //            dmx,
    //            dmx_curr_adj,
    //            pwm_ena,
    //            pwm_ch,
    //            eq_pwm);
    //     eq_pwm_v[i] = eq_pwm;
    // }
    
    // printf("\n set to 2 amps\n");
    // curr_mult = 4;    
    // // PWM_Soft_Handler_Low_Freq_Roof_Set(roof);
    // PWM_Map_Post_Filter_Top_Multiplier(133, curr_mult, 2);
    // for (int i = 0; i < 256 * curr_mult; i += curr_mult)
    // {
    //     dmx = i;
    //     PWM_Map_Post_Filter (dmx, &pwm_ena, &pwm_ch);
    //     eq = pwm_ena * pwm_ch / roof;
    //     eq_pwm = (unsigned short) eq;
    //     printf("curr: %d, dmx: %d, pwm_ena: %d pwm_ch: %d eq_pwm: %d\n",
    //            top_min_curr >> 1,
    //            dmx,
    //            pwm_ena,
    //            pwm_ch,
    //            eq_pwm);
    // }
    
    printf("\n set to 1 amps\n");
    unsigned short points = 50;
    curr_mult = 2;
    // PWM_Soft_Handler_Low_Freq_Roof_Set(roof);
    PWM_Map_Post_Filter_Top_Multiplier(135, curr_mult, 1);

    for (int i = 0; i < points; i++)
    {
        dmx = i;
        dmx_curr_adj = dmx * curr_mult;
        PWM_Map_Post_Filter (dmx_curr_adj, &pwm_ena, &pwm_ch);
        eq_pwm = (float) pwm_ena * (float) pwm_ch / (float) roof;
        printf("dmx_in: %d, curr_adj: %d, pwm_ena: %d pwm_ch: %d eq_pwm: %.2f\n",
               dmx,
               dmx_curr_adj,
               pwm_ena,
               pwm_ch,
               eq_pwm);
        eq_pwm_v[i] = eq_pwm;
    }

    ///////////////////////////
    // Backup Data to a file //
    ///////////////////////////
    FILE * file = fopen("data.txt", "w");

    if (file == NULL)
    {
        printf("data file not created!\n");
        return;
    }

    Vector_Float_To_File (file, "eq_pwm_v", eq_pwm_v, points);    

    printf("\nRun by hand python3 simul_outputs.py\n");
    
}

// Module Mocked Functions -----------------------------------------------------
void Update_TIM16_CH1N (unsigned short new_pwm)
{
    // pwm_data_ch1[pwm_cntr] = new_pwm;
    // updated = 1;
}


void Update_TIM14_CH1 (unsigned short new_pwm)
{
    // pwm_data_ch2[pwm_cntr] = new_pwm;
    // updated = 1;
}


//--- end of file ---//


