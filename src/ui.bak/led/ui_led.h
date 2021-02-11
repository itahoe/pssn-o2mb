/**
  * @file    ui_led.h
  * @brief   User Interface, LED Services
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#ifndef  UI_LED_H
#define  UI_LED_H


#include <stdbool.h>
#include <string.h>
#include "config.h"
#include "bsp.h"


#define	UI_LED_FLSH_SHRT_TCKS   ( ((size_t) CFG_UI_LED_FLSH_SHRT_mSEC * BSP_SYSTICK_HZ) / 1000 )
#define	UI_LED_FLSH_LONG_TCKS   ( ((size_t) CFG_UI_LED_FLSH_LONG_mSEC * BSP_SYSTICK_HZ) / 1000 )


/**
  * @brief   Single LED
  */
typedef    bool ( * ui_led_get_t )( void );
typedef    void ( * ui_led_set_t )(     const   bool    color );


typedef	struct	ui_led_s
{
	size_t                  tick;
	ui_led_set_t            set;
	ui_led_get_t            get;
} ui_led_t;


/**
 * @brief LED Toggle.
 */
static
void ui_led_toggle(                     ui_led_t *              p )
{
	const   bool    led_sts =   p->get();

	p->set( led_sts ? false : true );
}


/**
 * @brief LED Flash.
 */
static
void ui_led_flash(                      ui_led_t *              p,
                                        size_t                  len )
{
	if( p->tick == 0 )
	{
		ui_led_toggle( p );
	}

	p->tick         =   len;
}


/**
 * @brief LED SysTick Hook.
 */
static
bool ui_led_hook(                       ui_led_t *              p )
{
	bool            resp            =   false;

	if( p->tick > 0 )
	{
		p->tick--;

		if( p->tick == 0 )
		{
			ui_led_toggle( p );
			resp    =   true;
		}
	}

	return( resp );
}


bool    ui_led_sts_hook( void );

void    ui_led_sts_toggle( void );

void    ui_led_sts_set(         const   bool                    mode );

void    ui_led_sts_flash(       const   size_t                  len );

void    ui_led_sts_init( void );


#endif	//UI_LED_H
