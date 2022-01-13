/**
  * @file    stm32_spi1.c
  * @brief   Board Support Package - SPI1 routines
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include "stm32.h"


#define SPI_DELAY(ticks)        for( volatile int i = 0; i < ticks; i++ )
#define SPI_SCK_LOW()           HAL_GPIO_WritePin( GPIOA, GPIO_PIN_6,  GPIO_PIN_RESET )
#define SPI_SCK_HIGH()          HAL_GPIO_WritePin( GPIOA, GPIO_PIN_6,  GPIO_PIN_SET )
#define SPI_MOSI_WRITE( d )     HAL_GPIO_WritePin( GPIOA, GPIO_PIN_7, (d) ? GPIO_PIN_SET : GPIO_PIN_RESET )
//#define SPI_MISO_READ()         ( HAL_GPIO_ReadPin( GPIOA, GPIO_PIN_5 ) == GPIO_PIN_SET )
#define SPI_MISO_READ()         HAL_GPIO_ReadPin( GPIOA, GPIO_PIN_5 )


static
void
init_io( void )
{
        GPIO_InitTypeDef        pins;


        pins.Pin        = GPIO_PIN_4 | GPIO_PIN_6 | GPIO_PIN_7;
	pins.Mode       = GPIO_MODE_OUTPUT_PP;
	pins.Pull       = GPIO_NOPULL;
	pins.Speed      = GPIO_SPEED_FREQ_HIGH;
	pins.Alternate  = GPIO_AF0_SPI1;
        HAL_GPIO_Init( GPIOA, &pins );

        pins.Pin        = GPIO_PIN_5;
	pins.Mode       = GPIO_MODE_INPUT;
        HAL_GPIO_Init( GPIOA, &pins );
}


static
int
init_spi( void )
{
        return(0);
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
        SPI_SCK_HIGH();

        return( 0 );
}


/*******************************************************************************
* TRANSFER
*******************************************************************************/
void
stm32_spi1_nss_force( void )
{
        HAL_GPIO_WritePin( GPIOA, GPIO_PIN_4,  GPIO_PIN_RESET );
        SPI_DELAY( 10 );
}


void
stm32_spi1_nss_release( void )
{
        HAL_GPIO_WritePin( GPIOA, GPIO_PIN_4,  GPIO_PIN_SET );
        SPI_DELAY( 10 );
}


int
stm32_spi1_xfer(                                uint8_t *       data_xmit,
                                                uint8_t *       data_recv,
                                        const   size_t          size    )
{
//0001 0100     0x14
//0000 1001     0x09
//0000 1000     0x08

/*
00
14
ad7799_init() = -1
00
A9
04
00
*/

        uint8_t         i, recv;


        for( i = 0x80; i > 0; i >>= 1 )
        {
                SPI_DELAY( 5 );
                SPI_SCK_LOW();

                if( SPI_MISO_READ() )
                {
                        recv    |= i;
                }
                else
                {
                        recv    &= ~i;
                }

                SPI_MOSI_WRITE( ((*data_xmit) & i) & 0xFF );

                SPI_DELAY( 5 );
                SPI_SCK_HIGH();
        }


        *data_recv      = recv;

        //printf( "%02X %02X\n", *data_xmit, recv );


        return( 0 );
}


/*******************************************************************************
* INTERRUPT
*******************************************************************************/
void
stm32_spi1_isr( void )
{
        //HAL_SPI_IRQHandler( &hspi1 );
}
