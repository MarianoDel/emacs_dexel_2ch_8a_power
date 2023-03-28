//---------------------------------------------
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ## @CPU:    STM32F030
// ##
// #### UART.C ################################
//---------------------------------------------

// Includes --------------------------------------------------------------------
#include "usart.h"
#include "hard.h"

#include <string.h>


// Module Private Types Constants and Macros -----------------------------------
// Module Configs --------------------------------------------------------------
#define USART1_CLK (RCC->APB2ENR & 0x00004000)
#define USART1_CLK_ON RCC->APB2ENR |= 0x00004000
#define USART1_CLK_OFF RCC->APB2ENR &= ~0x00004000

#define USART_9600		5000
#define USART_115200		416
#define USART_250000		192


// Externals -------------------------------------------------------------------


// Globals ---------------------------------------------------------------------
volatile unsigned char usart1_have_data = 0;
volatile unsigned char * ptx1;
volatile unsigned char * ptx1_pckt_index;
volatile unsigned char * prx1;
volatile unsigned char tx1buff[SIZEOF_TXDATA];
volatile unsigned char rx1buff[SIZEOF_RXDATA];




// Module Private Functions ----------------------------------------------------


// Module Functions ------------------------------------------------------------
unsigned char Usart1ReadBuffer (char * bout, unsigned short max_len)
{
    unsigned int len;

    len = prx1 - rx1buff;

    if (len < max_len)
    {
        *prx1 = '\0';    //buffer from int isnt ended with '\0' do it now
        len += 1;    //space for '\0'
    }
    else
    {
        *(bout + max_len - 1) = '\0';
        len = max_len - 1;
    }

    memcpy(bout, (unsigned char *) rx1buff, len);

    //pointer adjust after copy
    prx1 = rx1buff;
    return (unsigned char) len;
}


void USART1_IRQHandler(void)
{
    unsigned char dummy;

    // USART in Receive mode -------------------------------------------------
    if (USART1->ISR & USART_ISR_RXNE)
    {
        dummy = USART1->RDR & 0x0FF;

        if (prx1 < &rx1buff[SIZEOF_RXDATA - 1])
        {
            //al /r no le doy bola
            if (dummy == '\r')
            {
            }            
            else if ((dummy == '\n') || (dummy == 26))		//26 es CTRL-Z
            {
                *prx1 = '\0';
                usart1_have_data = 1;
            }
            else
            {
                *prx1 = dummy;
                prx1++;
            }
        }
        else
            prx1 = rx1buff;    //soluciona problema bloqueo con garbage

    }

    // USART in Transmit mode -------------------------------------------------
    if (USART1->CR1 & USART_CR1_TXEIE)
    {
        if (USART1->ISR & USART_ISR_TXE)
        {
            if ((ptx1 < &tx1buff[SIZEOF_TXDATA]) && (ptx1 < ptx1_pckt_index))
            {
                USART1->TDR = *ptx1;
                ptx1++;
            }
            else
            {
                ptx1 = tx1buff;
                ptx1_pckt_index = tx1buff;
                USART1->CR1 &= ~USART_CR1_TXEIE;
            }
        }
    }

    if ((USART1->ISR & USART_ISR_ORE) || (USART1->ISR & USART_ISR_NE) || (USART1->ISR & USART_ISR_FE))
    {
        USART1->ICR |= 0x0e;
        dummy = USART1->RDR;
    }
}


void Usart1Send (char * send)
{
    unsigned char i;

    i = strlen(send);
    Usart1SendUnsigned((unsigned char *) send, i);
}


void Usart1SendUnsigned(unsigned char * send, unsigned char size)
{
    if ((ptx1_pckt_index + size) < &tx1buff[SIZEOF_TXDATA])
    {
        memcpy((unsigned char *)ptx1_pckt_index, send, size);
        ptx1_pckt_index += size;
        USART1->CR1 |= USART_CR1_TXEIE;
    }
}


void Usart1SendSingle(unsigned char tosend)
{
    Usart1SendUnsigned(&tosend, 1);
}


void Usart1Config(void)
{
    if (!USART1_CLK)
        USART1_CLK_ON;

    ptx1 = tx1buff;
    ptx1_pckt_index = tx1buff;
    prx1 = rx1buff;

    USART1->BRR = USART_9600;
    USART1->CR2 = 0;
    // USART1->CR1 = USART_CR1_RE | USART_CR1_TE | USART_CR1_UE;
    // USART1->CR1 = USART_CR1_RXNEIE | USART_CR1_RE | USART_CR1_UE;	//no TX
    USART1->CR1 = USART_CR1_RXNEIE | USART_CR1_TE | USART_CR1_RE | USART_CR1_UE;    //TX and RX

    unsigned int temp;
    temp = GPIOA->AFR[0];
    temp &= 0xFFFF00FF;
    temp |= 0x00001100;    //PA3 -> AF1 PA2 -> AF1
    GPIOA->AFR[0] = temp;

    NVIC_EnableIRQ(USART1_IRQn);
    NVIC_SetPriority(USART1_IRQn, 7);
}


unsigned char Usart1HaveData (void)
{
    return usart1_have_data;
}


void Usart1HaveDataReset (void)
{
    usart1_have_data = 0;
}


//--- end of file ---//
