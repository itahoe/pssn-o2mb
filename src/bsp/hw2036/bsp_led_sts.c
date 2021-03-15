/**
  * @file    bsp_led_sts.h
  * @brief   Board Support Package - LED routines
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include	"bsp.h"


/**
 * @brief LED pin initialization
 */
void bsp_led_sts_init( void )
{
	GPIO_InitTypeDef        gpio_led_sts    =   {    .Pin       =    GPIO_PIN_4,
	                                                 .Mode      =    GPIO_MODE_OUTPUT_PP,
	                                                 .Pull      =    GPIO_NOPULL,
	                                                 .Speed     =    GPIO_SPEED_FREQ_LOW };

	__HAL_RCC_GPIOB_CLK_ENABLE();
	HAL_GPIO_Init( GPIOB, &gpio_led_sts );
}


/**
 * @brief LED state get.
 */
bool    bsp_led_sts_get( void )
{
	return( HAL_GPIO_ReadPin( GPIOB, GPIO_PIN_4 ) ? true : false );
}


/**
 * @brief LED pin set
 */
void	bsp_led_sts_set(                 const   bool           val )
{
        HAL_GPIO_WritePin( GPIOB, GPIO_PIN_4,  val ? GPIO_PIN_SET : GPIO_PIN_RESET );
}
