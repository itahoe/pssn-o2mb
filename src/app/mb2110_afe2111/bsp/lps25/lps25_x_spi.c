/**
  * @file    lps25_x.c
  * @brief   LPS25 Pressure & Temperature Sensor Hardware Routins
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include "lps25.h"
#include "app.h"
//#include "bsp.h"
#include "stm32.h"


/*******************************************************************************
*
*******************************************************************************/
#define NCS_PORT                GPIOA
#define NCS_PIN                 GPIO_PIN_8

#define SCK_PORT                GPIOB
#define SCK_PIN                 GPIO_PIN_10

#define MOSI_PORT               GPIOB
#define MOSI_PIN                GPIO_PIN_11

#define MISO_PORT               GPIOA
#define MISO_PIN                GPIO_PIN_2

/*
#define MISO_PORT               GPIOB
#define MISO_PIN                GPIO_PIN_11

#define MOSI_PORT               GPIOA
#define MOSI_PIN                GPIO_PIN_2
*/

#define SPI_DELAY(ticks)        for( volatile int i = 0; i < ticks; i++ )
#define SPI_NCS_HIGH()          HAL_GPIO_WritePin( NCS_PORT,  NCS_PIN,  GPIO_PIN_SET )
#define SPI_NCS_LOW()           HAL_GPIO_WritePin( NCS_PORT,  NCS_PIN,  GPIO_PIN_RESET )
#define SPI_SCK_HIGH()          HAL_GPIO_WritePin( SCK_PORT,  SCK_PIN,  GPIO_PIN_SET )
#define SPI_SCK_LOW()           HAL_GPIO_WritePin( SCK_PORT,  SCK_PIN,  GPIO_PIN_RESET )
#define SPI_MOSI_WRITE( d )     HAL_GPIO_WritePin( MOSI_PORT, MOSI_PIN, d ? GPIO_PIN_SET : GPIO_PIN_RESET )
#define SPI_MISO_READ()         ( HAL_GPIO_ReadPin( MISO_PORT, MISO_PIN ) == GPIO_PIN_SET )


/*******************************************************************************
*
*******************************************************************************/
static  I2C_HandleTypeDef       hi2c;
static  DMA_HandleTypeDef       hdma_tx;
static  DMA_HandleTypeDef       hdma_rx;


/*******************************************************************************
*
*******************************************************************************/
/**
  * @brief  
  * @param  None
  * @retval None
  */
static
void
bsp_lps25_init_io( void )
{
        GPIO_InitTypeDef        sck         =   {    .Pin       =    SCK_PIN,
	                                             .Mode      =    GPIO_MODE_OUTPUT_PP,
	                                             .Pull      =    GPIO_PULLDOWN,
	                                             .Speed     =    GPIO_SPEED_FREQ_LOW };

        GPIO_InitTypeDef        mosi        =   {    .Pin       =    MOSI_PIN,
	                                             .Mode      =    GPIO_MODE_OUTPUT_PP,
	                                             .Pull      =    GPIO_PULLDOWN,
	                                             .Speed     =    GPIO_SPEED_FREQ_LOW };

        GPIO_InitTypeDef        miso        =   {    .Pin       =    MISO_PIN,
	                                             .Mode      =    GPIO_MODE_INPUT,
	                                             .Pull      =    GPIO_NOPULL,
	                                             .Speed     =    GPIO_SPEED_FREQ_LOW };

        GPIO_InitTypeDef        ncs         =   {    .Pin       =    NCS_PIN,
	                                             .Mode      =    GPIO_MODE_OUTPUT_PP,
	                                             .Pull      =    GPIO_PULLUP,
	                                             .Speed     =    GPIO_SPEED_FREQ_LOW };


        __HAL_RCC_GPIOA_CLK_ENABLE();
        __HAL_RCC_GPIOB_CLK_ENABLE();

        HAL_GPIO_Init( SCK_PORT,    &sck );
        HAL_GPIO_Init( MOSI_PORT,   &mosi );
        HAL_GPIO_Init( MISO_PORT,   &miso );
        HAL_GPIO_Init( NCS_PORT,    &ncs   );
}


/**
  * @brief  
  * @param  None
  * @retval None
  */
void
lps25_x_init( void )
{
        bsp_lps25_init_io();

        SPI_NCS_HIGH();
        SPI_SCK_HIGH();
}


/**
  * @brief  
  * @param  None
  * @retval None
  */
uint8_t
lps25_x_xfer(                                   uint8_t         data )
{
        uint8_t         i;


        for( i = 0x80; i > 0; i >>= 1 )
        {
                SPI_SCK_LOW();
                SPI_MOSI_WRITE( (data & i) );
                SPI_SCK_HIGH();

                if( SPI_MISO_READ() )
                {
                        data    |= i;
                }
                else
                {
                        data    &= ~i;
                }
        }

        return( data );
}


/**
  * @brief  
  * @param  None
  * @retval None
  */
void
lps25_x_read(                           const   uint8_t         addr,
                                                uint8_t *       data,
                                                size_t          cnt )
{
        SPI_NCS_LOW();

        lps25_x_xfer( addr | 0x80 | 0x40 );

        for( size_t i = 0; i < cnt; i++ )
        {
                data[i] = lps25_x_xfer( 0xFF );
        }

        SPI_NCS_HIGH();
}


/**
  * @brief  
  * @param  None
  * @retval None
  */
void
lps25_x_write(                          const   uint8_t         addr,
                                        const   uint8_t *       data,
                                                uint8_t         cnt )
{
        SPI_NCS_LOW();

        lps25_x_xfer( addr | 0x40 );

        for( size_t i = 0; i < cnt; i++ )
        {
                lps25_x_xfer( data[i] );
        }

        SPI_NCS_HIGH();
}
