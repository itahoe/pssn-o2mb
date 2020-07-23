/**
  * @file    ad7799.c
  * @brief   AD7799 driver
  * @author  Igor T. <research.tahoe@gmail.com>
  */


/******************************************************************************/
/* Include Files                                                              */
/******************************************************************************/
#include "AD7799.h"				// AD7799 definitions.
#include "bsp.h"


#define SPI_DELAY(ticks)        for( volatile int i = 0; i < ticks; i++ )
#define AD7799_CS_LOW           do{ HAL_GPIO_WritePin( GPIOA, GPIO_PIN_4,  GPIO_PIN_RESET ); SPI_DELAY(500); } while(0)
#define AD7799_CS_HIGH          do{ SPI_DELAY(500); HAL_GPIO_WritePin( GPIOA, GPIO_PIN_4,  GPIO_PIN_SET ); } while(0)

#define SPI_SCK_HIGH()          HAL_GPIO_WritePin( GPIOA, GPIO_PIN_5,  GPIO_PIN_SET )
#define SPI_SCK_LOW()           HAL_GPIO_WritePin( GPIOA, GPIO_PIN_5,  GPIO_PIN_RESET )
#define SPI_MISO_READ()         ( HAL_GPIO_ReadPin( GPIOA, GPIO_PIN_6 ) == GPIO_PIN_SET )
#define SPI_MOSI_WRITE( d )     HAL_GPIO_WritePin( GPIOA, GPIO_PIN_7, d ? GPIO_PIN_SET : GPIO_PIN_RESET )


static
uint8_t
bsp_spi_xfer_08(                                uint8_t         data )
{
        uint8_t         i;


        for( i = 0x80; i > 0; i >>= 1 )
        {
                SPI_SCK_LOW();

                SPI_MOSI_WRITE( (data & i) );

                SPI_DELAY( 100 );

                if( SPI_MISO_READ() )
                {
                        data    |= i;
                }
                else
                {
                        data    &= ~i;
                }

                SPI_SCK_HIGH();

                SPI_DELAY( 100 );
        }

        return( data );
}


/***************************************************************************//**
 * @brief Initializes the AD7799 and checks if the device is present.
 *
 * @param None.
 *
 * @return status - Result of the initialization procedure.
 *                  Example: 1 - if initialization was successful (ID is 0x0B).
 *                           0 - if initialization was unsuccessful.
*******************************************************************************/
uint8_t
ad7799_init( void )
{ 
	uint8_t         sts     = 0x1;


	if(( ad7799_reg_read( AD7799_REG_ID, 1 ) & 0x0F) != AD7799_ID )
	{
		sts     = 0x0;
	}
	
	return( sts );
}


/***************************************************************************//**
 * @brief Sends 32 consecutive 1's on SPI in order to reset the part.
 *
 * @param None.
 *
 * @return  None.    
*******************************************************************************/
void
ad7799_reset( void )
{
	uint8_t         data[4] = {0xff, 0xff, 0xff, 0xff};

	AD7799_CS_LOW;	    

	//SPI_Write( data, 4 );
        bsp_spi_xfer_08( data[ 0] );
        bsp_spi_xfer_08( data[ 1] );
        bsp_spi_xfer_08( data[ 2] );
        bsp_spi_xfer_08( data[ 3] );

	AD7799_CS_HIGH;	
}


/***************************************************************************//**
 * @brief Reads the value of the selected register
 *
 * @param regAddress - The address of the register to read.
 * @param size - The size of the register to read.
 *
 * @return data - The value of the selected register register.
*******************************************************************************/
uint32_t
ad7799_reg_read(                const   uint8_t         addr,
                                        const   uint8_t         size )
{
	uint32_t        data32          = 0;	
	uint8_t         data[ 4]        = {0x00, 0x00, 0x00, 0x00};
        //uint8_t         cmd             = AD7799_COMM_READ | AD7799_COMM_ADDR( addr );


	AD7799_CS_LOW;

        //bsp_spi_xfer_08( cmd );
        bsp_spi_xfer_08( AD7799_COMM_READ | AD7799_COMM_ADDR( addr ) );

        for( int i = 0; i < size; i++ )
        {
                data32  <<= 8;
                data32  += bsp_spi_xfer_08( data[ i] );
        }

	AD7799_CS_HIGH;

        return( data32 );
}


/***************************************************************************//**
 * @brief Writes the value to the register
 *
 * @param -  regAddress - The address of the register to write to.
 * @param -  regValue - The value to write to the register.
 * @param -  size - The size of the register to write.
 *
 * @return  None.    
*******************************************************************************/
void
ad7799_reg_write(                const   uint8_t         addr,
					const   uint32_t        data,
					const   uint8_t         size )
{
	uint8_t         buf[ 4] = {0x00, 0x00, 0x00, 0x00};	


        uint8_t         cmd             = AD7799_COMM_WRITE |  AD7799_COMM_ADDR( addr );

        if( size == 1 )
        {
                buf[ 0]         = ( uint8_t )   data;
        }
        if( size == 2 )
        {
		buf[ 1]         = ( uint8_t ) ((data & 0x0000FF) >>  0);
                buf[ 0]         = ( uint8_t ) ((data & 0x00FF00) >>  8);
        }
        if( size == 3 )
        {
		buf[ 2]         = ( uint8_t ) ((data & 0x0000FF) >>  0);
		buf[ 1]         = ( uint8_t ) ((data & 0x00FF00) >>  8);
                buf[ 0]         = ( uint8_t ) ((data & 0xFF0000) >> 16);
        }

	AD7799_CS_LOW;	    


        cmd     = bsp_spi_xfer_08( cmd );

        for( int i = 0; i < size; i++ )
        {
                buf[ i]         = bsp_spi_xfer_08( buf[ i] );
        }

	AD7799_CS_HIGH;
}


/***************************************************************************//**
 * @brief Reads /RDY bit of status reg.
 *
 * @param None.
 *
 * @return rdy	- 0 if RDY is 1.
 *              - 1 if RDY is 0.
*******************************************************************************/
uint8_t
ad7799_Ready( void )
{
        uint8_t         rdy     = 0;


        rdy     = ( ad7799_reg_read( AD7799_REG_STAT,1) & 0x80 );   
        return( !rdy );
}


/***************************************************************************//**
 * @brief Sets the operating mode of AD7799.
 *
 * @param mode - Mode of operation.
 *
 * @return  None.    
*******************************************************************************/
void
ad7799_SetMode(                         const   uint32_t        mode )
{
        uint32_t        cmd     = ad7799_reg_read( AD7799_REG_MODE, 2 );


        cmd     &= ~AD7799_MODE_SEL( 0xFF );
        cmd     |= AD7799_MODE_SEL( mode );

        ad7799_reg_write( AD7799_REG_MODE, cmd, 2 );
}


/***************************************************************************//**
 * @brief Selects the channel of AD7799.
 *
 * @param  channel - ADC channel selection.
 *
 * @return  None.    
*******************************************************************************/
void
ad7799_SetChannel(                      const   uint32_t        chnl )
{
        uint32_t        cmd     = ad7799_reg_read( AD7799_REG_CONF, 2 );


        cmd     &= ~AD7799_CONF_CHAN(0xFF);
        cmd     |= AD7799_CONF_CHAN( chnl );

        ad7799_reg_write( AD7799_REG_CONF, cmd, 2 );
}


/***************************************************************************//**
 * @brief  Sets the gain of the In-Amp.
 *
 * @param  gain - Gain.
 *
 * @return  None.    
*******************************************************************************/
void
ad7799_SetGain(                         const   uint32_t        gain )
{
    uint32_t    cmd;


        cmd     = ad7799_reg_read( AD7799_REG_CONF, 2 );
        cmd     &= ~AD7799_CONF_GAIN( 0xFF );
        cmd     |= AD7799_CONF_GAIN( gain );

        ad7799_reg_write( AD7799_REG_CONF, cmd, 2 );
}


/***************************************************************************//**
 * @brief Enables or disables the reference detect function.
 *
 * @param state - State of the reference detect function.
 *               Example: 0	- Reference detect disabled.
 *                        1	- Reference detect enabled.
 *
 * @return None.    
*******************************************************************************/
void
ad7799_SetReference(                    const   uint8_t         state )
{
        uint32_t        cmd     = ad7799_reg_read( AD7799_REG_CONF, 2 );


        cmd     &= ~AD7799_CONF_REFDET( 1 );
        cmd     |= AD7799_CONF_REFDET( state );

        ad7799_reg_write( AD7799_REG_CONF, cmd, 2);
}
