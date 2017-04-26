/* Includes ------------------------------------------------------------------*/
#include "uart2.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* User can use this section to tailor USARTx/UARTx instance used and associated 
resources */
UART_HandleTypeDef UartHandle;
/* Definition for USARTx clock resources */
#define USARTx                           USART2
#define USARTx_CLK_ENABLE()              __HAL_RCC_USART2_CLK_ENABLE();
#define USARTx_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()
#define USARTx_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE() 

#define USARTx_FORCE_RESET()             __HAL_RCC_USART2_FORCE_RESET()
#define USARTx_RELEASE_RESET()           __HAL_RCC_USART2_RELEASE_RESET()

/* Definition for USARTx Pins */
#define USARTx_TX_PIN                    GPIO_PIN_2
#define USARTx_TX_GPIO_PORT              GPIOA  
#define USARTx_TX_AF                     GPIO_AF7_USART2
#define USARTx_RX_PIN                    GPIO_PIN_3
#define USARTx_RX_GPIO_PORT              GPIOA 
#define USARTx_RX_AF                     GPIO_AF7_USART2

/* Definition for USARTx's NVIC */
#define USARTx_IRQn                      USART2_IRQn
#define USARTx_IRQHandler                USART2_IRQHandler

/* Size of Transmission buffer */
#define TXBUFFERSIZE                      (COUNTOF(aTxBuffer) - 1)
/* Size of Reception buffer */
#define RXBUFFERSIZE                      TXBUFFERSIZE
  
/* Exported macro ------------------------------------------------------------*/
#define COUNTOF(__BUFFER__)   (sizeof(__BUFFER__) / sizeof(*(__BUFFER__)))
/* Exported functions ------------------------------------------------------- */


USART_DATA	g_usart2;

void USART2_IRQHandler(void)
{
	// Received completed interrupt(RXNE) if RXNEIE=1 in the USARTx_CR1 register
	if((USART2->SR & USART_SR_RXNE) && (USART2->CR1&USART_CR1_RXNEIE ))
	{
		// Auto cleared by a read to the DR
		g_usart2.rxBuff[g_usart2.rxEnd++] = USART2->DR;
	}
	// Transmit completed interrupt(TC) if TCIE=1 in the USARTx_CR1 register
	else if((USART2->SR & USART_SR_TC) && (USART2->CR1 & USART_CR1_TCIE))
	{
		if(g_usart2.txStart!=g_usart2.txEnd)
		{
			// Auto cleared by a write to the DR
			USART2->DR= g_usart2.txBuff[g_usart2.txStart++];
		}
		else
		{
			USART2_PreReceive();
		}
	}
	else
	{
		// Note: STM32F411 can be cleared by a read to the DR
		g_usart2.rxBuff[g_usart2.rxEnd++] = USART2->DR;
	}
}


int USART2_Init(void)
{
	HAL_StatusTypeDef status;
	
	GPIO_InitTypeDef  GPIO_InitStruct;
  
  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* Enable GPIO TX/RX clock */
  USARTx_TX_GPIO_CLK_ENABLE();
  USARTx_RX_GPIO_CLK_ENABLE();
  /* Enable USART2 clock */
  USARTx_CLK_ENABLE(); 
  
  /*##-2- Configure peripheral GPIO ##########################################*/  
  /* UART TX GPIO pin configuration  */
  GPIO_InitStruct.Pin       = USARTx_TX_PIN;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_NOPULL;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
  GPIO_InitStruct.Alternate = USARTx_TX_AF;
  
  HAL_GPIO_Init(USARTx_TX_GPIO_PORT, &GPIO_InitStruct);
    
  /* UART RX GPIO pin configuration  */
  GPIO_InitStruct.Pin = USARTx_RX_PIN;
  GPIO_InitStruct.Alternate = USARTx_RX_AF;
    
  HAL_GPIO_Init(USARTx_RX_GPIO_PORT, &GPIO_InitStruct);
	
	UartHandle.Instance          = USARTx;
  
  UartHandle.Init.BaudRate     = 115200;
  UartHandle.Init.WordLength   = UART_WORDLENGTH_8B;
  UartHandle.Init.StopBits     = UART_STOPBITS_1;
  UartHandle.Init.Parity       = UART_PARITY_NONE;
  UartHandle.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
  UartHandle.Init.Mode         = UART_MODE_TX_RX;
  UartHandle.Init.OverSampling = UART_OVERSAMPLING_16;
    
	status = HAL_UART_Init(&UartHandle);
  if(status != HAL_OK)
  {
    return status;
  }
	
	// Enable USART2_IRQn
	HAL_NVIC_SetPriority(USART2_IRQn, 0, 1);
	HAL_NVIC_EnableIRQ(USART2_IRQn);
	USART2_PreReceive();
	return HAL_OK;
}

void USART2_OffReceive(void)
{
	USART2->CR1 &= ~USART_CR1_RE;                  // Receiver disable
}

void USART2_PreReceive(void)
{
	// note: STM32F411 can be cleared by writing a '0'
	USART2->SR &=  ~USART_SR_TC;                   // clear TC interrupt flag
	USART2->SR &=  ~USART_SR_RXNE;                 // clear RXNE interrupt flag
	USART2->CR1 &= ~USART_CR1_TCIE;                // disable TC interrupt
	USART2->CR1 |= USART_CR1_RXNEIE;               // enable RXNE interrupt
	USART2->CR1 |= USART_CR1_RE;                   // Receiver Enable

	g_usart2.rxStart= 0;                           // clear buffer and set receive state
	g_usart2.rxEnd  = 0;
	g_usart2.status = USART_RX;                    // set usart state for receving
}

void USART2_PreTransmit(void)
{
	USART2->SR &=  ~USART_SR_TC;                   // clear TC interrupt flag
	USART2->SR &=  ~USART_SR_RXNE;                 // clear RXNE interrupt flag
	USART2->CR1 &= ~USART_CR1_RXNEIE;              // disable RXNE interrupt
	USART2->CR1 |= USART_CR1_TCIE;                 // enable TC interrupt
	USART2->CR1 &= ~USART_CR1_RE;                  // Receiver disable

	g_usart2.txStart  = 0;                         // clear buffer and set transmit state
	g_usart2.txEnd    = 0;                         // note: do not clear end 
	g_usart2.status = USART_TX;                    // set usart state for transmitting
}

void USART2_StartSend(void)
{
	// Transmit first data to DR
	USART2->DR = g_usart2.txBuff[g_usart2.txStart++];
}

uint8_t USART2_GetByte(uint8_t* pData)
{
	uint8_t size=0;
	if(g_usart2.rxStart != g_usart2.rxEnd)
	{
		pData[size++] = g_usart2.rxBuff[g_usart2.rxStart++];
	}
	return size;
}

uint8_t USART2_Write(uint8_t* pData, uint8_t len)
{
	if(len < 1)
		return 0;
	while(len--)
	{
		g_usart2.txBuff[g_usart2.txEnd++] = *pData++;
	}
	USART2_PreTransmit();
	USART2_StartSend();
	return g_usart2.txEnd;
}

uint8_t USART2_GetStatus(void)
{
	return g_usart2.status;
}


