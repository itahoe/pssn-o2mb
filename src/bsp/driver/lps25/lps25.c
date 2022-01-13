/**
  * @file    lps25.c
  * @brief   LPS25 driver
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include "lps25.h"


/*******************************************************************************
* CONVERTIONS
*******************************************************************************/
/**
  * @brief  None
  * @param  None
  * @retval None
  */
float
lps25_pressure_raw_to_hpa(              const   int32_t         raw )
{
        return( (float) raw / (float) 4096.0f );
/*
        float   f;


        f       = raw;
        f       /= (float) 4096.0f;

        //printf( "%f\n", f );

        return( f );
*/
}


/**
  * @brief  None
  * @param  None
  * @retval None
  */
float
lps25_pressure_raw_to_mbar(             const   int32_t         raw )
{
        return( (float) raw / 4096.0f );
}


/**
  * @brief  None
  * @param  None
  * @retval None
  */
float
lps25_temperature_raw_to_digc(          const   int16_t         raw )
{
        return( (float) raw / 480.0f + 42.5f );
}


/*******************************************************************************
*
*******************************************************************************/
/**
  * @brief  Initializes the LPS25.
  * @param  None
  * @retval error (true : false)
  */
/*
bool
lps25_init( void )
{ 
	bool    error   = false;


        lps25_x_init();

	if( lps25_get_id() != LPS25_ID )
	{
		error   = true;
	}

	return( error );
}
*/

/**
  * @brief  Software reset the LPS25.
  * @param  None
  * @retval error (true : false)
  */
void
lps25_reset( void )
{
}


/**
  * @brief  Read ID.
  * @param  None
  * @retval 0xBD if LPS25 is correct.
  */
uint8_t
lps25_get_id( void )
{
        uint8_t         id;


        lps25_x_read( LPS25_REG_WHO_AM_I, &id, 1 );

        return( id );
}


/**
  * @brief  None
  * @param  None
  * @retval None
  */
uint8_t
lps25_get_ctl(                          const   lps25_ctl_t     ctl )
{
        uint8_t         buf[ 1];
        uint8_t         rslt;


        switch( ctl )
        {
                case LPS25_CTL_POWER_DOWN:
                        lps25_x_read( (uint8_t) LPS25_REG_CTRL_REG1, buf, 1 );
                        rslt    = buf[ 0] >> 7;
                        break;


                default:
                        break;
        }

        return( rslt );
}


/**
  * @brief  None
  * @param  None
  * @retval None
  */
void
lps25_set_ctl(                          const   lps25_ctl_t     ctl,
                                        const   uint8_t         d )
{
        uint8_t         reg;


        switch( ctl )
        {
                case LPS25_CTL_POWER_DOWN:
                        lps25_x_read( (uint8_t) LPS25_REG_CTRL_REG1, &reg, 1 );
                        reg     &= ~(1 << 7);
                        reg     |= (d ? 0x80 : 0x00);
                        lps25_x_write( (uint8_t) LPS25_REG_CTRL_REG1, &reg, 1 );
                        break;

                default:
                        break;
        }
}


/**
  * @brief  None
  * @param  None
  * @retval None
  */
uint8_t
lps25_get_data_rate( void )
{
        uint8_t         buf[ 1];


        lps25_x_read( (uint8_t) LPS25_REG_CTRL_REG1, buf, 1 );


        return( (buf[ 0] >> 4) & 0x7 );
}


/**
  * @brief  None
  * @param  None
  * @retval None
  */
void
lps25_set_data_rate(            const   lps25_rate_t            rate )
{
        uint8_t         reg;


        lps25_x_read( (uint8_t) LPS25_REG_CTRL_REG1, &reg, 1 );
        reg     &= ~(0x07 << 4);
        reg     |=  (rate << 4);
        lps25_x_write( (uint8_t) LPS25_REG_CTRL_REG1, &reg, 1 );

}


/**
  * @brief  None
  * @param  None
  * @retval None
  */
int32_t
lps25_get_pressure_raw( void )
{
        uint8_t         buf[ 3];
        int32_t         raw     = 0;


        lps25_x_read( (uint8_t) LPS25_REG_PRESS_OUT_XL, buf, 3 );

        raw     =  (buf[ 0] <<  0);
        raw     |= (buf[ 1] <<  8);
        raw     |= (buf[ 2] << 16);

        return( raw );
}


/**
  * @brief  Read LPS25 temperature
  * @param  None
  * @retval temperature as raw
  */
int16_t
lps25_get_temperature_raw( void )
{
        uint8_t         buf[ 2];
        int16_t         raw     = 0;


        lps25_x_read( (uint8_t) LPS25_REG_TEMP_OUT_L, buf, 2 );

        //lps25_x_read( (uint8_t) LPS25_REG_TEMP_OUT_L, &buf[ 0], 1 );
        //lps25_x_read( (uint8_t) LPS25_REG_TEMP_OUT_H, &buf[ 1], 1 );

        raw     |= (buf[ 0] <<  0);
        raw     |= (buf[ 1] <<  8);

        return( raw );
}


/**
  * @brief  None
  * @param  None
  * @retval None
  */
uint16_t
lps25_get_pressure_offset_raw( void )
{
        uint8_t         buf[ 2];
        int16_t         raw     = 0;


        lps25_x_read( (uint8_t) LPS25_REG_RPDS_L, buf, 2 );
        raw     |= (buf[ 0] <<  0);
        raw     |= (buf[ 1] <<  8);

        return( raw );
}


/**
  * @brief  None
  * @param  None
  * @retval None
  */
void
lps25_set_pressure_offset_raw(          const   uint16_t        raw )
{
}
