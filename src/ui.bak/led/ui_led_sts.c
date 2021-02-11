/**
  * @file    ui_led_sts.c
  * @brief   User Interface, Status LED
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include	"ui_led.h"


	ui_led_t        ui_led_sts      =   {   .tick           =   0,
	                                        .set            =   stm32_led_sts_set,
                                                .get            =   stm32_led_sts_get };


/**
 * @brief       Initialisation
 */
void    ui_led_sts_init( void )
{
        stm32_led_sts_init();
}


/**
 * @brief       LED Hook
 */
bool ui_led_sts_hook( void )
{
	bool    rslt    =   ui_led_hook( &ui_led_sts );

	return( rslt );
}


/**
 * @brief LED Control.
 */
void    ui_led_sts_set(         const   bool                    mode )
{
        bsp_led_sts_set( mode );
}


/**
 * @brief LED Toggle.
 */
void ui_led_sts_toggle( void )
{
	ui_led_toggle( &ui_led_sts );
}


/**
 * @brief LED Flash.
 */
void ui_led_sts_flash(          const   size_t                  len )
{
	ui_led_flash( &ui_led_sts, len );
}
