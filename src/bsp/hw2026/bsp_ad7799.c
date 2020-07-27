/**
  * @file    bsp_ad7799.c
  * @brief   AD7799 low-level routins
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include "app.h"
#include "bsp.h"
#include "ad7799.h"


void    ad7799_x_init( void );
void    ad7799_x_enable( const bool );
uint8_t ad7799_x_xfer( uint8_t );


void
ad7799_x_init( void )
{
}


void
ad7799_x_enable(                        const   bool    enable )
{
        if( enable )
        {
                HAL_GPIO_WritePin( GPIOA, GPIO_PIN_4,  GPIO_PIN_RESET );
        }
        else
        {
                HAL_GPIO_WritePin( GPIOA, GPIO_PIN_4,  GPIO_PIN_SET );
        }
}


#define SPI_DELAY(ticks)        for( volatile int i = 0; i < ticks; i++ )
#define SPI_SCK_HIGH()          HAL_GPIO_WritePin( GPIOA, GPIO_PIN_5,  GPIO_PIN_SET )
#define SPI_SCK_LOW()           HAL_GPIO_WritePin( GPIOA, GPIO_PIN_5,  GPIO_PIN_RESET )
#define SPI_MISO_READ()         ( HAL_GPIO_ReadPin( GPIOA, GPIO_PIN_6 ) == GPIO_PIN_SET )
#define SPI_MOSI_WRITE( d )     HAL_GPIO_WritePin( GPIOA, GPIO_PIN_7, d ? GPIO_PIN_SET : GPIO_PIN_RESET )


uint8_t
ad7799_x_xfer( uint8_t data )
{
        uint8_t         i;


        for( i = 0x80; i > 0; i >>= 1 )
        {
                SPI_SCK_LOW();

                SPI_MOSI_WRITE( (data & i) );

                //SPI_DELAY( 100 );

                if( SPI_MISO_READ() )
                {
                        data    |= i;
                }
                else
                {
                        data    &= ~i;
                }

                SPI_SCK_HIGH();

                //SPI_DELAY( 100 );
        }

        return( data );
}
