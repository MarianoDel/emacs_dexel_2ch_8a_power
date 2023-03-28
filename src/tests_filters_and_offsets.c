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
#include "filters_and_offsets.h"


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
extern unsigned short soft_saved_pwm_ch1;
extern unsigned short soft_saved_pwm_ch2;


// Globals ---------------------------------------------------------------------
unsigned short ch1_ena [VECTOR_LENGTH] = { 0 };
unsigned short ch2_ena [VECTOR_LENGTH] = { 0 };
unsigned short ch1_pwm [VECTOR_LENGTH] = { 0 };
unsigned short ch2_pwm [VECTOR_LENGTH] = { 0 };


int millis_cnt = 0;
int update_ch1_ena = 0;
int update_ch1_pwm = 0;
int update_ch2_ena = 0;
int update_ch2_pwm = 0;

// Tests Functions -------------------------------------------------------------
void Test_Filters_Step (void);
void Test_Filters_Ramp (void);


// Main Function to Test -------------------------------------------------------


// Module Auxiliary Functions for Tests ----------------------------------------
void Update_Vectors (void);


// Module Mock Functions -------------------------------------------------------
void Update_TIM16_CH1N (unsigned short new_pwm);
void Update_TIM14_CH1 (unsigned short new_pwm);



// Module Functions ------------------------------------------------------------
int main (int argc, char *argv[])
{
    printf("Start of filters tests...\n");

    // Test_Filters_Step ();
    Test_Filters_Ramp ();

    return 0;
}


void Test_Filters_Step (void)
{
    unsigned char dmx_vector [SIMULATION_MS] = { 0 };
    unsigned char dmx_inputs [2];
    
    printf("reseting filters\n");
    FiltersAndOffsets_Filters_Reset ();

    printf(" filters setting current\n");
    FiltersAndOffsets_Set_Current(16);    //eight amps
    
    printf(" filters step\n");
    for (int i = 0; i < SIMULATION_MS; i++)
        dmx_vector[i] = 255;
    
    for (int i = 0; i < SIMULATION_MS; i++)
    {
        dmx_inputs[0] = dmx_vector[i];
        dmx_inputs[1] = dmx_vector[i];
        
        FiltersAndOffsets_Post_Mapping_SM(dmx_inputs);
        Update_Vectors();
        
        millis_cnt++;
    }
    
    ShowVectorUShort("\nch1 ena:\n", ch1_ena, 16);
    ShowVectorUShort("\nch1 pwm:\n", ch1_pwm, 16);

    ///////////////////////////
    // Backup Data to a file //
    ///////////////////////////
    FILE * file = fopen("data.txt", "w");

    if (file == NULL)
    {
        printf("data file not created!\n");
        return;
    }

    Vector_UShort_To_File (file, "ch1_ena", ch1_ena, SIMULATION_MS);
    Vector_UShort_To_File (file, "ch1_pwm", ch1_pwm, SIMULATION_MS);    

    printf("\nRun by hand python3 simul_outputs.py\n");
    
}


void Test_Filters_Ramp (void)
{
    unsigned char dmx_vector [SIMULATION_MS] = { 0 };
    unsigned char dmx_inputs [2];
    unsigned short dmx_v[SIMULATION_MS] = { 0 };
    
    printf("reseting filters\n");
    FiltersAndOffsets_Filters_Reset ();

    printf(" filters setting current\n");
    // FiltersAndOffsets_Set_Current(8);    //eight amps
    // FiltersAndOffsets_Set_Current(7);    //seven amps
    FiltersAndOffsets_Set_Current(6);    //six amps
    // FiltersAndOffsets_Set_Current(4);    //four amps    
    // FiltersAndOffsets_Set_Current(3);    //three amps    
    // FiltersAndOffsets_Set_Current(4);    //two amps    
    
    printf(" filters ramp\n");
    int ramp_cnt = 0;
    for (int i = 0; i < SIMULATION_MS; i++)
    {
        if (!(i % 5))
        {
            if (ramp_cnt < 255)
                ramp_cnt++;
        }
        dmx_vector[i] = ramp_cnt;
        dmx_v[i] = dmx_vector[i];
    }
    
    for (int i = 0; i < SIMULATION_MS; i++)
    {
        dmx_inputs[0] = dmx_vector[i];
        dmx_inputs[1] = dmx_vector[i];
        
        FiltersAndOffsets_Post_Mapping_SM(dmx_inputs);
        Update_Vectors();
        
        millis_cnt++;
    }

    ShowVectorUShort("\nch1 ena:\n", ch1_ena, 16);
    ShowVectorUShort("\nch1 pwm:\n", ch1_pwm, 16);

    ///////////////////////////
    // Backup Data to a file //
    ///////////////////////////
    FILE * file = fopen("data.txt", "w");

    if (file == NULL)
    {
        printf("data file not created!\n");
        return;
    }

    Vector_UShort_To_File (file, "ch1_ena", ch1_ena, SIMULATION_MS);
    Vector_UShort_To_File (file, "ch1_pwm", ch1_pwm, SIMULATION_MS);
    Vector_UShort_To_File (file, "dmx_vector", dmx_v, SIMULATION_MS);    

    printf("\nRun by hand python3 simul_outputs.py\n");
    
}


void Update_Vectors (void)
{
    if (!millis_cnt)
        return;

    // ch1_ena updated at same time ch1_pwm
    if (update_ch1_pwm)
    {
        update_ch1_pwm = 0;
        ch1_ena[millis_cnt] = soft_saved_pwm_ch1;
    }
    else
    {
        ch1_pwm[millis_cnt] = ch1_pwm[millis_cnt - 1];
        ch1_ena[millis_cnt] = ch1_ena[millis_cnt - 1];
    }

    // ch2_ena updated at same time ch2_pwm
    if (update_ch2_pwm)
    {
        update_ch2_pwm = 0;
        ch2_ena[millis_cnt] = soft_saved_pwm_ch2;
    }
    else
    {
        ch2_pwm[millis_cnt] = ch2_pwm[millis_cnt - 1];
        ch2_ena[millis_cnt] = ch2_ena[millis_cnt - 1];
    }
    
}
// Module Mocked Functions -----------------------------------------------------
void Update_TIM16_CH1N (unsigned short new_pwm)
{
    ch1_pwm[millis_cnt] = new_pwm;
    update_ch1_pwm = 1;
}


void Update_TIM14_CH1 (unsigned short new_pwm)
{
    ch2_pwm[millis_cnt] = new_pwm;
    update_ch2_pwm = 1;
}


// void Update_TIM1_CH2 (unsigned short new_pwm)
// {
//     ch1_ena[millis_cnt] = new_pwm;
//     update_ch1_ena = 1;
// }


// void Update_TIM1_CH4 (unsigned short new_pwm)
// {
//     ch2_ena[millis_cnt] = new_pwm;
//     update_ch2_ena = 1;
// }


//--- end of file ---//


