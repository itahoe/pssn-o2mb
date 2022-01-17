/**
  * @file    ad7799.c
  * @brief   AD7799 driver
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include "ad7799.h"


/*******************************************************************************
* REG READ
*******************************************************************************/
uint16_t
ad7799_read(                    const   ad7799_addr_t           addr )
{
        uint8_t         data_hi, data_lo;

	ad7799_x_enable( true );
        ad7799_x_xfer( AD7799_COMM_READ | (addr << 3) );
        data_hi = ad7799_x_xfer( 0x00 );
        data_lo = ad7799_x_xfer( 0x00 );
	ad7799_x_enable( false );

        return( (data_hi << 8) | (data_lo & 0xFF) );
}

/*******************************************************************************
* REG WRITE
*******************************************************************************/
void
ad7799_write(                   const   ad7799_addr_t           addr,
                                const   uint16_t                data )
{
	ad7799_x_enable( true );	    
        ad7799_x_xfer( AD7799_COMM_WRITE | (addr << 3) );
        ad7799_x_xfer( data >> 8 );
        ad7799_x_xfer( data & 0xFF );
	ad7799_x_enable( false );
}

/*******************************************************************************
* REG ID
*******************************************************************************/
uint8_t
ad7799_get_id( void )
{
        uint8_t         data;

	ad7799_x_enable( true );
        ad7799_x_xfer( AD7799_COMM_READ | (AD7799_REG_ID << 3) );
        data    = ad7799_x_xfer( 0x00 );
	ad7799_x_enable( false );	    

        return( data );
}


/*******************************************************************************
*
*******************************************************************************/
/**
  * @brief  Sends 32 consecutive 1's on SPI in order to reset the part.  
  * @param  None
  * @retval None
  */
void
ad7799_reset( void )
{
	ad7799_x_enable( true );	    
        ad7799_x_xfer( 0xFF );
        ad7799_x_xfer( 0xFF );
        ad7799_x_xfer( 0xFF );
        ad7799_x_xfer( 0xFF );
	ad7799_x_enable( false );	
}


/**
  * @brief  Initializes the AD7799 and checks if the device is present.
  * @param  None
  * @retval error (true : false)
  */
int
ad7799_init( void )
{
        uint8_t         id;


        ad7799_reset();
        id      = ad7799_get_id();
        return( AD7799_ID != (0x0F & id) );
}


/*******************************************************************************
* REG DATA
*******************************************************************************/
uint32_t
ad7799_get_sample( void )
{
        ad7799_reg_t            reg;
        uint8_t                 data;


        reg.mode.u16    = ad7799_read( AD7799_REG_MODE );
        reg.mode.mode   = AD7799_MODE_SINGLE;
        ad7799_write( AD7799_REG_MODE, reg.mode.u16 );


	ad7799_x_enable( true );

        do
        {
                ad7799_x_xfer( AD7799_COMM_READ | (AD7799_REG_STS << 3) );
                data    = ad7799_x_xfer( 0x00 );
        } while(  data & AD7799_STS_READY );

	ad7799_x_enable( false );


        uint32_t        data32  = 0;

	ad7799_x_enable( true );
        ad7799_x_xfer( AD7799_COMM_READ | (AD7799_REG_DATA << 3) );
        data32  |= ad7799_x_xfer( 0x00 );
        data32  <<= 8;
        data32  |= ad7799_x_xfer( 0x00 );
        data32  <<= 8;
        data32  |= ad7799_x_xfer( 0x00 );
	ad7799_x_enable( false );

        return( data32 );
}
