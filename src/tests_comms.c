//---------------------------------------------
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ## @CPU:    TEST PLATFORM FOR FIRMWARE
// ##
// #### TESTS.C ###############################
//---------------------------------------------

// Includes Modules for tests --------------------------------------------------
#include "comms.h"

// helper modules
#include "tests_ok.h"
#include "tests_mock_usart.h"

#include <stdio.h>
#include <string.h>

// Types Constants and Macros --------------------------------------------------


// Externals -------------------------------------------------------------------

// Globals -- Externals in Module ----------------------------------------------
volatile unsigned char last_ch_values [2] = { 0 };


// Globals ---------------------------------------------------------------------
int cb_usart_value = 0;



// Module Functions to Test ----------------------------------------------------
void Test_Comms_Dmx (void);
void Test_Comms_Version (void);
void Test_Comms_Current_Old_String (void);
void Test_Comms_Current_New_String (void);


// Module Auxiliary Functions --------------------------------------------------
void CB_Usart (char * s);


// Tests Module Auxiliary or General Functions ---------------------------------


// Module Functions ------------------------------------------------------------
int main (int argc, char *argv[])
{

    // Test_Comms_Dmx ();

    // Test_Comms_Version ();

    Test_Comms_Current_Old_String ();

    // Test_Comms_Current_New_String ();    

    return 0;
}


void Test_Comms_Dmx (void)
{
    // set callback on usart1
    Usart1Callback(CB_Usart);

    cb_usart_value = 0;
    Usart1FillRxBuffer("loco\r\n");    
    Comms_Update ();

    printf("Test no answer string: ");
    if (cb_usart_value == 0)
        PrintOK();
    else
        PrintERR();

    printf("\n");
    cb_usart_value = 0;    
    Usart1FillRxBuffer("ch1 aaa ch2 255 sum 511 ");
    Comms_Update ();
    
    printf("Test error string ch1: ");
    if (cb_usart_value == 1)
        PrintOK();
    else
        PrintERR();

    printf("\n");
    cb_usart_value = 0;    
    Usart1FillRxBuffer("ch1 255 ch2 aaa sum 511 ");
    Comms_Update ();
    
    printf("Test error string ch2: ");
    if (cb_usart_value == 2)
        PrintOK();
    else
        PrintERR();

    printf("\n");
    cb_usart_value = 0;    
    Usart1FillRxBuffer("ch1 255 ch2 255 sum 511 ");
    Comms_Update ();
    
    printf("Test error string verif: ");
    if (cb_usart_value == 3)
        PrintOK();
    else
        PrintERR();

    printf("\n");
    cb_usart_value = 0;    
    Usart1FillRxBuffer("ch1 255 ch2 255 sum 510 ");
    Comms_Update ();
    
    printf("Test good string: ");
    if (cb_usart_value == 0)
        PrintOK();
    else
        PrintERR();
    
}


void Test_Comms_Current_New_String (void)
{
    // set callback on usart1
    Usart1Callback(CB_Usart);

    printf("\n");
    cb_usart_value = 0;
    Usart1FillRxBuffer("current config 4.3\r\n");
    Comms_Update ();

    printf("Test current string: ");
    if (cb_usart_value == 4)
        PrintOK();
    else
        PrintERR();

}


void Test_Comms_Current_Old_String (void)
{
    // set callback on usart1
    Usart1Callback(CB_Usart);

    printf("\n");
    cb_usart_value = 0;
    Usart1FillRxBuffer("current config 4 5\r\n");
    Comms_Update ();

    printf("Test current string: ");
    if (cb_usart_value == 4)
        PrintOK();
    else
        PrintERR();

    // printf("\n");
    // cb_usart_value = 0;
    // Usart1FillRxBuffer("current config 8\r\n");    
    // Comms_Update ();

    // printf("Test current string: ");
    // if (cb_usart_value == 4)
    //     PrintOK();
    // else
    //     PrintERR();
    
}


void Test_Comms_Version (void)
{
    // set callback on usart1
    Usart1Callback(CB_Usart);

    printf("\n");
    cb_usart_value = 0;
    Usart1FillRxBuffer("version\r\n");
    Comms_Update ();

    printf("Test version string: ");
    if (cb_usart_value == 5)
        PrintOK();
    else
        PrintERR();
}


// Module Auxiliary Functions --------------------------------------------------
void CB_Usart (char * s)
{
    if (strncmp(s, "err ch1", sizeof("err ch1") - 1) == 0)
        cb_usart_value = 1;

    if (strncmp(s, "err ch2", sizeof("err ch2") - 1) == 0)
        cb_usart_value = 2;

    if (strncmp(s, "err verif", sizeof("err verif") - 1) == 0)
        cb_usart_value = 3;
    
    if (strncmp(s, "ok", sizeof("ok") - 1) == 0)
        cb_usart_value = 4;

    if (strncmp(s, "Hrd 2.0 Soft 1.1", sizeof("Hrd 2.0 Soft 1.1") - 1) == 0)
        cb_usart_value = 5;
}


// Module Mocked Functions -----------------------------------------------------
void Led_On (void)
{
    printf("Led is on\n");
}


void Led_Off (void)
{
    printf("Led is off\n");
}


void FiltersAndOffsets_Set_Current (unsigned char cint, unsigned char cdec)
{
    printf("current setted to: %01d.%01dA\n", cint, cdec);
}


void PWM_Map_Post_Filter_Top_Multiplier (unsigned char top_multiplier,
                                         unsigned char min_curr)
{
    printf("pwm new settings top mult: %d min curr: %d\n",
           top_multiplier,
           min_curr);
    
}

void PWM_Soft_Handler_Low_Freq_Roof_Set (unsigned short new_roof)
{
    printf("pwm soft step to: %d\n", new_roof);
}


char hard_ver [] = {"Hrd 2.0"};
char * HARD_GetHardwareVersion (void)
{
    return hard_ver;
}


char soft_ver [] = {"Soft 1.1"};
char * HARD_GetSoftwareVersion (void)
{
    return soft_ver;
}
//--- end of file ---//


