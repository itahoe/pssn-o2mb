/**
  * @file    stm32_usart1.c
  * @brief   Board Support Package - Serial 1 port services
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include "app.h"
//#include "bsp.h"
#include "stm32.h"


static  uint16_t        const   pin_tx  = GPIO_PIN_9;
static  uint16_t        const   pin_rx  = GPIO_PIN_10;
static  uint16_t        const   pin_de  = GPIO_PIN_12;

static  UART_HandleTypeDef      huart1;
static  DMA_HandleTypeDef       hdma_rx;
static  DMA_HandleTypeDef       hdma_tx;


static
void
init_io( void )
{
        GPIO_InitTypeDef        uart_pins   =   {    .Pin       =    pin_tx | pin_rx | pin_de,
	                                             .Mode      =    GPIO_MODE_AF_PP,
	                                             .Pull      =    GPIO_NOPULL,
	                                             .Speed     =    GPIO_SPEED_FREQ_LOW,
	                                             .Alternate =    GPIO_AF1_USART1 };


        __HAL_RCC_GPIOA_CLK_ENABLE();

        HAL_GPIO_Init( GPIOA, &uart_pins );
}


static
void
init_uart( void )
{
        huart1.Instance                          = USART1;
        //huart1.Init.BaudRate                     = baudrate;
        huart1.Init.WordLength                   = UART_WORDLENGTH_8B;
        huart1.Init.StopBits                     = UART_STOPBITS_1;
        huart1.Init.Parity                       = UART_PARITY_NONE;
        huart1.Init.Mode                         = UART_MODE_TX_RX;
        huart1.Init.HwFlowCtl                    = UART_HWCONTROL_NONE;
        huart1.Init.OverSampling                 = UART_OVERSAMPLING_16;
        huart1.Init.OneBitSampling               = UART_ONE_BIT_SAMPLE_DISABLE;
        huart1.AdvancedInit.AdvFeatureInit       = UART_ADVFEATURE_NO_INIT;


        __HAL_RCC_USART1_CLK_ENABLE();
        __HAL_RCC_USART1_FORCE_RESET();
        __HAL_RCC_USART1_RELEASE_RESET();

        if( HAL_RS485Ex_Init( &huart1, UART_DE_POLARITY_HIGH, 0, 0 ) != HAL_OK )
        {
                app_error_handler();
        }
}


static
void
init_dma( void )
{
        __HAL_RCC_DMA1_CLK_ENABLE();


        hdma_rx.Instance                        = DMA1_Channel3;
        hdma_rx.Init.Direction                  = DMA_PERIPH_TO_MEMORY;
        hdma_rx.Init.PeriphInc                  = DMA_PINC_DISABLE;
        hdma_rx.Init.MemInc                     = DMA_MINC_ENABLE;
        hdma_rx.Init.PeriphDataAlignment        = DMA_PDATAALIGN_BYTE;
        hdma_rx.Init.MemDataAlignment           = DMA_MDATAALIGN_BYTE;
        hdma_rx.Init.Mode                       = DMA_NORMAL;
        hdma_rx.Init.Priority                   = DMA_PRIORITY_LOW;

        if( HAL_DMA_Init( &hdma_rx ) != HAL_OK )
        {
                app_error_handler();
        }

        __HAL_LINKDMA( &huart1, hdmarx, hdma_rx );


        hdma_tx.Instance                        = DMA1_Channel2;
        hdma_tx.Init.Direction                  = DMA_MEMORY_TO_PERIPH;
        hdma_tx.Init.PeriphInc                  = DMA_PINC_DISABLE;
        hdma_tx.Init.MemInc                     = DMA_MINC_ENABLE;
        hdma_tx.Init.PeriphDataAlignment        = DMA_PDATAALIGN_BYTE;
        hdma_tx.Init.MemDataAlignment           = DMA_MDATAALIGN_BYTE;
        hdma_tx.Init.Mode                       = DMA_NORMAL;
        hdma_tx.Init.Priority                   = DMA_PRIORITY_LOW;

        if( HAL_DMA_Init( &hdma_tx ) != HAL_OK )
        {
                app_error_handler();
        }

        __HAL_LINKDMA( &huart1, hdmatx, hdma_tx );
}


/*******************************************************************************
* PUBLIC FUNCTIONS
*******************************************************************************/
int
stm32_usart1_init( void )
{
        init_uart();
        init_dma();
        init_io();

        NVIC_SetPriority(       USART1_IRQn,    STM32_NVIC_PRIO_USART1  );
        NVIC_EnableIRQ(         USART1_IRQn                             );

        __HAL_UART_CLEAR_IDLEFLAG( &huart1 );
        __HAL_UART_ENABLE_IT( &huart1, UART_IT_IDLE );
        //__HAL_UART_ENABLE_IT( &huart, UART_IT_RXNE );

        HAL_NVIC_SetPriority(   DMA1_Channel2_3_IRQn, 0, 0              );
        HAL_NVIC_EnableIRQ(     DMA1_Channel2_3_IRQn                    );

        return( 0 );
}


void
stm32_usart1_config_baudrate(           const   size_t          baudrate )
{
        huart1.Init.BaudRate    = baudrate;
}


/*******************************************************************************
* RECIEVE
*******************************************************************************/
int
stm32_usart1_recv_dma(                          uint8_t *       data,
                                        const   size_t          size    )
{
        HAL_UART_AbortReceive( &huart1 );
        __HAL_UART_CLEAR_OREFLAG( &huart1 );
        HAL_UART_Receive_DMA( &huart1, data, size );

        return( 0 );
}


uint32_t
stm32_usart1_dma_recv_get_ndtr( void )
{
        return( __HAL_DMA_GET_COUNTER( &hdma_rx ) );
}


/*******************************************************************************
* TRANSMIT
*******************************************************************************/
int
stm32_usart1_xmit_dma(                          uint8_t *       data,
                                        const   size_t          size    )
{
        if( HAL_UART_Transmit_DMA( &huart1, data, size )!= HAL_OK )
        {
                return( -1 );
        }

        return( 0 );
}


/*******************************************************************************
* INTERRUPT
*******************************************************************************/
void
stm32_usart1_isr( void )
{
	HAL_UART_IRQHandler( &huart1 );
}


int
stm32_usart1_isr_is_idle( void )
{
        int     idle_sts        = 0;


        if( __HAL_UART_GET_FLAG( &huart1, UART_FLAG_IDLE ) )
        {
                __HAL_UART_CLEAR_IDLEFLAG( &huart1 );
                idle_sts        = -1;
        }

	HAL_UART_IRQHandler( &huart1 );

        return( idle_sts );
}


void
stm32_usart1_dma_isr( void )
{
	HAL_DMA_IRQHandler( &hdma_rx );
	HAL_DMA_IRQHandler( &hdma_tx );
}


/*
void
stm32_usart1_dma_rx_isr( void )
{
	HAL_DMA_IRQHandler( &hdma_rx );
}


void
stm32_usart1_dma_tx_isr( void )
{
	HAL_DMA_IRQHandler( &hdma_tx );
}
*/
