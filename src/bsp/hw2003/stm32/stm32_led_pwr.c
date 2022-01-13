/**
  * @file       stm32_led_pwr.c
  * @brief      LED POWER routines
  * @author     Igor T. <research.tahoe@gmail.com>
  */


#include "stm32.h"


/*******************************************************************************
* PRIVATE DEFINES
*******************************************************************************/
#define LED_PORT                GPIOB
#define LED_PIN                 GPIO_PIN_3


/*******************************************************************************
* PRIVATE VARIABLE
*******************************************************************************/
static  size_t          tick;


/*******************************************************************************
* PRIVATE FUNCTIONS
*******************************************************************************/
static
void
led_toggle( void )
{
	bool    sts     = stm32_led_pwr_get();

	stm32_led_pwr_set( sts ? false : true );
}


/*******************************************************************************
* PUBLIC FUNCTIONS
*******************************************************************************/
/**
 * @brief Initialization
 */
void
stm32_led_pwr_init( void )
{
	GPIO_InitTypeDef        pin     = {     .Pin       =    LED_PIN,
	                                        .Mode      =    GPIO_MODE_OUTPUT_PP,
	                                        .Pull      =    GPIO_NOPULL,
	                                        .Speed     =    GPIO_SPEED_FREQ_LOW };

	HAL_GPIO_Init( LED_PORT, &pin );
}


/**
 * @brief Get state
 */
bool
stm32_led_pwr_get( void )
{
	return( HAL_GPIO_ReadPin( LED_PORT, LED_PIN ) ? true : false );
}

/**
 * @brief Set state
 */
void
stm32_led_pwr_set(        const   bool                    val )
{
        HAL_GPIO_WritePin( LED_PORT, LED_PIN, val ? GPIO_PIN_SET : GPIO_PIN_RESET );
}


/**
 * @brief Tick Interrupt Hook
 */
void
stm32_led_pwr_hook( void )
{
	if( tick > 0 )
	{
		tick--;

		if( tick == 0 )
		{
			led_toggle();
		}
	}
}


void
stm32_led_pwr_flash(                    const   size_t          len )
{
	if( tick == 0 )
	{
                led_toggle();
	}

	tick    = len;
}

