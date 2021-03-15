/**
  * @file    bsp_ser1.c
  * @brief   Board Support Package - Serial 1 port services
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include "app.h"
#include "bsp.h"


static  uint16_t        const   pin_tx  = GPIO_PIN_9;
static  uint16_t        const   pin_rx  = GPIO_PIN_10;
static  uint16_t        const   pin_de  = GPIO_PIN_12;

static  UART_HandleTypeDef      huart;
static  DMA_HandleTypeDef       hdma_rx;
static  DMA_HandleTypeDef       hdma_tx;


static
void
bsp_ser1_init_io( void )
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
bsp_ser1_init_uart(             const   size_t          baudrate    )
{
        huart.Instance                          = USART1;
        huart.Init.BaudRate                     = baudrate;
        huart.Init.WordLength                   = UART_WORDLENGTH_8B;
        huart.Init.StopBits                     = UART_STOPBITS_1;
        huart.Init.Parity                       = UART_PARITY_NONE;
        huart.Init.Mode                         = UART_MODE_TX_RX;
        huart.Init.HwFlowCtl                    = UART_HWCONTROL_NONE;
        huart.Init.OverSampling                 = UART_OVERSAMPLING_16;
        huart.Init.OneBitSampling               = UART_ONE_BIT_SAMPLE_DISABLE;
        huart.AdvancedInit.AdvFeatureInit       = UART_ADVFEATURE_NO_INIT;


        __HAL_RCC_USART1_CLK_ENABLE();
        __HAL_RCC_USART1_FORCE_RESET();
        __HAL_RCC_USART1_RELEASE_RESET();

        if( HAL_RS485Ex_Init( &huart, UART_DE_POLARITY_HIGH, 0, 0 ) != HAL_OK )
        {
                app_error_handler();
        }
}


static
void    bsp_ser1_init_dma( void )
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

        __HAL_LINKDMA( &huart, hdmarx, hdma_rx );


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

        __HAL_LINKDMA( &huart, hdmatx, hdma_tx );
}


void    bsp_ser1_init(                  const   size_t          baud )
{
        bsp_ser1_init_uart( baud );
        bsp_ser1_init_dma();
        bsp_ser1_init_io();

        NVIC_SetPriority(       USART1_IRQn,    BSP_NVIC_PRIO_SER1_UART );
        NVIC_EnableIRQ(         USART1_IRQn                             );

        __HAL_UART_CLEAR_IDLEFLAG( &huart );
        __HAL_UART_ENABLE_IT( &huart, UART_IT_IDLE );
        //__HAL_UART_ENABLE_IT( &huart, UART_IT_RXNE );

        HAL_NVIC_SetPriority(   DMA1_Channel2_3_IRQn, 0, 0              );
        HAL_NVIC_EnableIRQ(     DMA1_Channel2_3_IRQn                    );
}


/*******************************************************************************
* RECIEVE
*******************************************************************************/
void
bsp_ser1_recv_start(                            uint8_t *       data,
                                        const   size_t          size    )
{
        HAL_UART_AbortReceive( &huart );
        __HAL_UART_CLEAR_OREFLAG( &huart );
        HAL_UART_Receive_DMA( &huart, data, size );
}


uint32_t bsp_ser1_dma_recv_get_ndtr( void )
{
        return( __HAL_DMA_GET_COUNTER( &hdma_rx ) );
}


/*******************************************************************************
* TRANSMIT
*******************************************************************************/
void
bsp_ser1_xmit(                                  uint8_t *       data,
                                        const   size_t          size    )
{
        if( HAL_UART_Transmit_DMA( &huart, data, size )!= HAL_OK )
        {
                //app_error_handler();
        }
}


/*******************************************************************************
* INTERRUPT
*******************************************************************************/
bool
bsp_ser1_uart_isr( void )
{
        bool    sts             = false;


        if( __HAL_UART_GET_FLAG( &huart, UART_FLAG_IDLE ) )
        {
                __HAL_UART_CLEAR_IDLEFLAG( &huart );
                sts     = true;
        }

	HAL_UART_IRQHandler( &huart );

        return( sts );
}


void
bsp_ser1_dma_isr( void )
{
	HAL_DMA_IRQHandler( &hdma_rx );
	HAL_DMA_IRQHandler( &hdma_tx );
}
