/**
  * @file    stm32_usart2.c
  * @brief   Serial 2 port services
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include "app.h"
#include "stm32.h"


static  uint16_t        const   pin_tx  = GPIO_PIN_2;
static  uint16_t        const   pin_rx  = GPIO_PIN_3;
static  uint16_t        const   pin_de  = GPIO_PIN_1;

static  UART_HandleTypeDef      huart2;
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
	                                             .Alternate =    GPIO_AF1_USART2 };

        HAL_GPIO_Init( GPIOA, &uart_pins );
}


static
void
init_uart( void )
{
        huart2.Instance                          = USART2;
        //huart2.Init.BaudRate                     = baudrate;
        huart2.Init.WordLength                   = UART_WORDLENGTH_8B;
        huart2.Init.StopBits                     = UART_STOPBITS_1;
        huart2.Init.Parity                       = UART_PARITY_NONE;
        huart2.Init.Mode                         = UART_MODE_TX_RX;
        huart2.Init.HwFlowCtl                    = UART_HWCONTROL_NONE;
        huart2.Init.OverSampling                 = UART_OVERSAMPLING_16;
        huart2.Init.OneBitSampling               = UART_ONE_BIT_SAMPLE_DISABLE;
        huart2.AdvancedInit.AdvFeatureInit       = UART_ADVFEATURE_NO_INIT;


        __HAL_RCC_USART2_CLK_ENABLE();
        __HAL_RCC_USART2_FORCE_RESET();
        __HAL_RCC_USART2_RELEASE_RESET();

        if( HAL_RS485Ex_Init( &huart2, UART_DE_POLARITY_HIGH, 0, 0 ) != HAL_OK )
        {
                app_error_handler();
        }
}


static
void
init_dma( void )
{
        hdma_rx.Instance                        = DMA1_Channel5;
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

        __HAL_LINKDMA( &huart2, hdmarx, hdma_rx );


        hdma_tx.Instance                        = DMA1_Channel4;
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

        __HAL_LINKDMA( &huart2, hdmatx, hdma_tx );
}


/*******************************************************************************
* PUBLIC FUNCTIONS
*******************************************************************************/
int
stm32_usart2_init( void )
{
        init_uart();
        init_dma();
        init_io();

        NVIC_SetPriority(       USART2_IRQn,    STM32_NVIC_PRIO_USART2  );
        NVIC_EnableIRQ(         USART2_IRQn                             );

        __HAL_UART_CLEAR_IDLEFLAG( &huart2 );
        __HAL_UART_ENABLE_IT( &huart2, UART_IT_IDLE );
        //__HAL_UART_ENABLE_IT( &huart, UART_IT_RXNE );

        HAL_NVIC_SetPriority(   DMA1_Channel4_5_IRQn, 0, 0              );
        HAL_NVIC_EnableIRQ(     DMA1_Channel4_5_IRQn                    );

        return( 0 );
}


int
stm32_usart2_config_baudrate(           const   size_t          baudrate )
{
        huart2.Init.BaudRate    = baudrate;
        return( 0 );
}


/*******************************************************************************
* RECIEVE
*******************************************************************************/
int
stm32_usart2_recv_dma(                          uint8_t *       data,
                                        const   size_t          size    )
{
        HAL_UART_AbortReceive( &huart2 );
        __HAL_UART_CLEAR_OREFLAG( &huart2 );
        HAL_UART_Receive_DMA( &huart2, data, size );

        return( 0 );
}


uint32_t
stm32_usart2_dma_recv_get_ndtr( void )
{
        return( __HAL_DMA_GET_COUNTER( &hdma_rx ) );
}


/*******************************************************************************
* TRANSMIT
*******************************************************************************/
int
stm32_usart2_xmit_dma(                          uint8_t *       data,
                                        const   size_t          size    )
{
        if( HAL_UART_Transmit_DMA( &huart2, data, size )!= HAL_OK )
        {
                return( -1 );
        }

        return( 0 );
}


/*******************************************************************************
* INTERRUPT
*******************************************************************************/
int
stm32_usart2_isr( void )
{
	HAL_UART_IRQHandler( &huart2 );
        return(0);
}


int
stm32_usart2_isr_is_idle( void )
{
        int     idle_sts        = 0;


        if( __HAL_UART_GET_FLAG( &huart2, UART_FLAG_IDLE ) )
        {
                __HAL_UART_CLEAR_IDLEFLAG( &huart2 );
                idle_sts        = -1;
        }

	HAL_UART_IRQHandler( &huart2 );

        return( idle_sts );
}


void
stm32_usart2_dma_isr( void )
{
	HAL_DMA_IRQHandler( &hdma_rx );
	HAL_DMA_IRQHandler( &hdma_tx );
}
