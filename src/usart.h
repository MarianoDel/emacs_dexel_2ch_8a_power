//---------------------------------------------
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ## @CPU:    STM32F030
// ##
// #### USART.H ###############################
//---------------------------------------------
#ifndef _USART_H_
#define _USART_H_


// Module Exported Types Constants and Macros ----------------------------------
#define USARTx_RX_DISA    (USARTx->CR1 &= 0xfffffffb)
#define USARTx_RX_ENA    (USARTx->CR1 |= 0x04)

#define SIZEOF_RXDATA 128
#define SIZEOF_TXDATA 128



// Module Exported Functions ---------------------------------------------------
void Usart1Config(void);
void Usart1Send (char *);
void Usart1SendUnsigned(unsigned char *, unsigned char);
unsigned char Usart1ReadBuffer (char *, unsigned short);
unsigned char Usart1HaveData (void);
void Usart1HaveDataReset (void);
void USART1_IRQHandler(void);


#endif    /* _USART_H_ */

//--- end of file ---//

