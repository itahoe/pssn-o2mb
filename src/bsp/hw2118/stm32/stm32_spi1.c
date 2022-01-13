/**
  * @file    stm32_spi1.c
  * @brief   Board Support Package - SPI1 routines
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include "stm32.h"


static  SPI_HandleTypeDef       hspi1;


static
void
init_io( void )
{
        GPIO_InitTypeDef        pins;


        pins.Pin        = GPIO_PIN_7 | GPIO_PIN_6 | GPIO_PIN_5;
	pins.Mode       = GPIO_MODE_AF_PP;
	pins.Pull       = GPIO_NOPULL;
	pins.Speed      = GPIO_SPEED_FREQ_HIGH;
	pins.Alternate  = GPIO_AF0_SPI1;
        HAL_GPIO_Init( GPIOA, &pins     );

        pins.Pin        = GPIO_PIN_4;
	pins.Mode       = GPIO_MODE_OUTPUT_PP;
        HAL_GPIO_Init( GPIOA, &pins     );
}


static
int
init_spi( void )
{
        int     resp    = 0;

        hspi1.Instance                          = SPI1;
        hspi1.Init.Mode                         = SPI_MODE_MASTER;
        hspi1.Init.Direction                    = SPI_DIRECTION_2LINES;
        hspi1.Init.DataSize                     = SPI_DATASIZE_8BIT;
        hspi1.Init.CLKPolarity                  = SPI_POLARITY_HIGH;
        hspi1.Init.CLKPhase                     = SPI_PHASE_2EDGE;
        hspi1.Init.NSS                          = SPI_NSS_SOFT;
        //hspi1.Init.BaudRatePrescaler            = SPI_BAUDRATEPRESCALER_16;
        hspi1.Init.BaudRatePrescaler            = SPI_BAUDRATEPRESCALER_256;
        hspi1.Init.FirstBit                     = SPI_FIRSTBIT_MSB;
        hspi1.Init.TIMode                       = SPI_TIMODE_DISABLE;
        hspi1.Init.CRCCalculation               = SPI_CRCCALCULATION_DISABLE;
        hspi1.Init.CRCPolynomial                = 7;
        hspi1.Init.CRCLength                    = SPI_CRC_LENGTH_DATASIZE;
        hspi1.Init.NSSPMode                     = SPI_NSS_PULSE_ENABLE;

        __HAL_RCC_SPI1_FORCE_RESET();
        __HAL_RCC_SPI1_RELEASE_RESET();

        if( HAL_SPI_Init( &hspi1 ) != HAL_OK )
        {
                resp    = -1;
        }

        return( resp );
}


/*******************************************************************************
* PUBLIC FUNCTIONS
*******************************************************************************/
int
stm32_spi1_init( void )
{
        init_spi();
        init_io();
        stm32_spi1_nss_release();

        NVIC_SetPriority(       SPI1_IRQn,      STM32_NVIC_PRIO_SPI1    );
        NVIC_EnableIRQ(         SPI1_IRQn                               );

        return( 0 );
}


/*******************************************************************************
* TRANSFER
*******************************************************************************/
void
stm32_spi1_nss_force( void )
{
        HAL_GPIO_WritePin( GPIOA, GPIO_PIN_4,  GPIO_PIN_RESET );
}


void
stm32_spi1_nss_release( void )
{
        HAL_GPIO_WritePin( GPIOA, GPIO_PIN_4,  GPIO_PIN_SET );
}


int
stm32_spi1_xfer(                                uint8_t *       data_xmit,
                                                uint8_t *       data_recv,
                                        const   size_t          size    )
{
        HAL_StatusTypeDef       err;
        uint32_t                timeout_msec    = 5000;


        err     = HAL_SPI_TransmitReceive( &hspi1, data_xmit, data_recv, size, timeout_msec );

        return( (int) err );
}


/*******************************************************************************
* INTERRUPT
*******************************************************************************/
void
stm32_spi1_isr( void )
{
        HAL_SPI_IRQHandler( &hspi1 );
}
