/**
  * @file    ad7799.c
  * @brief   AD7799 driver
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include "ad7799.h"


/*******************************************************************************
*
*******************************************************************************/
static
uint32_t
ad7799_read(                            const   ad7799_reg_t    addr,
                                        const   uint8_t         size )
{
	uint32_t        data    = 0;	


	ad7799_x_enable( true );

        ad7799_x_xfer( AD7799_COMM_READ | (addr << 3) );

        for( int i = 0; i < size; i++ )
        {
                data    <<= 8;
                data    += ad7799_x_xfer( 0x00 );
        }

	ad7799_x_enable( false );

        return( data );
}


static
void
ad7799_write(                           const   ad7799_reg_t    addr,
                                                uint32_t        data,
                                                uint8_t         size )
{
	ad7799_x_enable( true );	    

        ad7799_x_xfer( AD7799_COMM_WRITE | (addr << 3) );

        while( size-- )
        {
                ad7799_x_xfer( data >> (8*size) );
        }

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
	int     error   = 0;


        ad7799_x_init();

	if(( ad7799_read( AD7799_REG_ID, 1 ) & 0x0F) != AD7799_ID )
	{
		error   = -1;
	}

	return( error );
}


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


/*******************************************************************************
* REG STATUS
*******************************************************************************/
bool
ad7799_sts_ready( void )
{
        uint8_t ready   = AD7799_STS_READY & ad7799_read( AD7799_REG_STS, 1 );   

        return( ready ? false : true );
}


bool
ad7799_sts_error( void )
{
        uint8_t error   = AD7799_STS_ERROR & ad7799_read( AD7799_REG_STS, 1 );

        return( error ? true : false );
}


bool
ad7799_sts_noref( void )
{
        uint8_t noref   = AD7799_STS_NOREF & ad7799_read( AD7799_REG_STS, 1 );

        return( noref ? true : false );
}


/*******************************************************************************
* REG MODE
*******************************************************************************/
void
ad7799_set_mode(                        const   ad7799_mode_t   mode )
{
        uint16_t        reg     = ad7799_read( AD7799_REG_MODE, 2 );


        reg     &= ~( 0x7 << 13 );
        reg     |= ( mode << 13 );

        ad7799_write( AD7799_REG_MODE, reg, 2 );
}


void
ad7799_set_pwr_swtch(                   const   bool            psw )
{
        uint16_t        reg     = ad7799_read( AD7799_REG_MODE, 2 );


        reg     &= ~(1 << 12);
        reg     |= ( (psw ? 1 : 0) << 12 );

        ad7799_write( AD7799_REG_MODE, reg, 2 );
}


void
ad7799_set_rate(                        const   ad7799_rate_t      rate )
{
        uint16_t        reg     = ad7799_read( AD7799_REG_MODE, 2 );


        reg     &= ~( 0x000F );
        reg     |= (uint16_t) rate;

        ad7799_write( AD7799_REG_MODE, reg, 2 );
}


/*******************************************************************************
* REG CONFIGURATION
*******************************************************************************/
void
ad7799_set_burnout(                     const   bool            burnout )
{
        uint16_t        reg     = ad7799_read( AD7799_REG_CONF, 2 );


        reg     &= ~(1 << 13);
        reg     |= ( (burnout ? 1 : 0) << 13 );

        ad7799_write( AD7799_REG_CONF, reg, 2 );
}


void
ad7799_set_unipolar(                    const   bool            unipolar )
{
        uint16_t        reg     = ad7799_read( AD7799_REG_CONF, 2 );


        reg     &= ~(1 << 12);
        reg     |= ( (unipolar ? 1 : 0) << 12 );

        ad7799_write( AD7799_REG_CONF, reg, 2 );
}


void
ad7799_set_gain(                        const   ad7799_gain_t   gain )
{
        uint16_t        reg     = ad7799_read( AD7799_REG_CONF, 2 );


        reg     &= ~( 0x7 << 8 );
        reg     |= ( gain << 8 );

        ad7799_write( AD7799_REG_CONF, reg, 2 );
}


void
ad7799_set_refdet(                      const   bool            refdet )
{
        uint16_t        reg     = ad7799_read( AD7799_REG_CONF, 2 );


        reg     &= ~(1 << 5);
        reg     |= ( (refdet ? 1 : 0) << 5 );

        ad7799_write( AD7799_REG_CONF, reg, 2 );
}


void
ad7799_set_buffered(                    const   bool            buf )
{
        uint16_t        reg     = ad7799_read( AD7799_REG_CONF, 2 );


        reg     &= ~(1 << 4);
        reg     |= ( (buf ? 1 : 0) << 4 );

        ad7799_write( AD7799_REG_CONF, reg, 2 );
}


void
ad7799_set_channel(                     const   ad7799_chnl_t   chnl )
{
        uint16_t        reg     = ad7799_read( AD7799_REG_CONF, 2 );


        reg     &= ~( 0x7 << 0 );
        reg     |= ( chnl << 0 );

        ad7799_write( AD7799_REG_CONF, reg, 2 );
}


/*******************************************************************************
* REG DATA
*******************************************************************************/
uint32_t
ad7799_get_data( void )
{
        return( ad7799_read( AD7799_REG_DATA, 3 ) );
}


/*******************************************************************************
* REG DATA
*******************************************************************************/
int32_t
ad7799_read_single( void )
{
        ad7799_set_mode( AD7799_MODE_SINGLE );
        while( !ad7799_sts_ready() );

        return( ad7799_get_data() );
}


/*******************************************************************************
* REG ID
*******************************************************************************/
uint8_t
ad7799_get_id( void )
{
        return( ad7799_read( AD7799_REG_ID, 1 ) );
}


/*******************************************************************************
* REG IO
*******************************************************************************/
uint8_t
ad7799_get_io( void )
{
        return( ad7799_read( AD7799_REG_IO, 1 ) );
}


void
ad7799_set_io(                          const   uint8_t         data )
{
        ad7799_write( AD7799_REG_IO, data, 1 );
}


/*******************************************************************************
* REG OFFSET
*******************************************************************************/
uint32_t
ad7799_get_offset( void )
{
        return( ad7799_read( AD7799_REG_OFFSET, 3 ) );
}


void
ad7799_set_offset(                      const   uint32_t        data )
{
        ad7799_write( AD7799_REG_OFFSET, data, 3 );
}


/*******************************************************************************
* REG FULL-SCALE
*******************************************************************************/
uint32_t
ad7799_get_fullscale( void )
{
        return( ad7799_read( AD7799_REG_FULLSALE, 3 ) );
}


void
ad7799_set_fullscale(                   const   uint32_t        data )
{
        ad7799_write( AD7799_REG_FULLSALE, data, 3 );
}
