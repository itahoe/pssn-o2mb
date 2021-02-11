/**
  * @file    stm32_tim16.c
  * @brief   STM32 TIM16 routines
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include "stm32.h"


static  TIM_HandleTypeDef       htim16;


/*******************************************************************************
* PRIVATE FUNCTIONS
*******************************************************************************/
static
void
init_io( void )
{
	GPIO_InitTypeDef        pin_ofst        =   {    .Pin       = GPIO_PIN_6,
	                                                 .Mode      = GPIO_MODE_AF_PP,
	                                                 .Pull      = GPIO_NOPULL,
	                                                 .Speed     = GPIO_SPEED_FREQ_LOW,
                                                        .Alternate  = GPIO_AF5_TIM16, };


        __HAL_RCC_GPIOA_CLK_ENABLE();
	HAL_GPIO_Init( GPIOA, &pin_ofst );

        //HAL_GPIO_WritePin( GPIOA, GPIO_PIN_6,  GPIO_PIN_SET );
        //HAL_GPIO_WritePin( GPIOA, GPIO_PIN_6,  GPIO_PIN_RESET );
}


static
void
init_pwm( void )
{
        TIM_OC_InitTypeDef              cfg_oc          = {0};
        TIM_BreakDeadTimeConfigTypeDef  cfg_brk         = {0};


        __HAL_RCC_TIM16_FORCE_RESET();
        __HAL_RCC_TIM16_RELEASE_RESET();
        __HAL_RCC_TIM16_CLK_ENABLE();


        htim16.Instance                 = TIM16;
        htim16.Init.Prescaler           = 1;
        htim16.Init.CounterMode         = TIM_COUNTERMODE_UP;
        htim16.Init.Period              = 0xFFFF;
        htim16.Init.ClockDivision       = TIM_CLOCKDIVISION_DIV1;
        htim16.Init.RepetitionCounter   = 0;
        htim16.Init.AutoReloadPreload   = TIM_AUTORELOAD_PRELOAD_DISABLE;
        HAL_TIM_Base_Init( &htim16 );

        HAL_TIM_PWM_Init( &htim16 );

        cfg_oc.OCMode                   = TIM_OCMODE_PWM1;
        cfg_oc.Pulse                    = 0;
        cfg_oc.OCPolarity               = TIM_OCPOLARITY_HIGH;
        cfg_oc.OCNPolarity              = TIM_OCNPOLARITY_HIGH;
        //cfg_oc.OCFastMode               = TIM_OCFAST_ENABLE;
        cfg_oc.OCFastMode               = TIM_OCFAST_DISABLE;
        cfg_oc.OCIdleState              = TIM_OCIDLESTATE_RESET;
        cfg_oc.OCNIdleState             = TIM_OCNIDLESTATE_RESET;
        HAL_TIM_PWM_ConfigChannel( &htim16, &cfg_oc, TIM_CHANNEL_1 );

        cfg_brk.OffStateRunMode         = TIM_OSSR_DISABLE;
        cfg_brk.OffStateIDLEMode        = TIM_OSSI_DISABLE;
        cfg_brk.LockLevel               = TIM_LOCKLEVEL_OFF;
        cfg_brk.DeadTime                = 0;
        cfg_brk.BreakState              = TIM_BREAK_DISABLE;
        cfg_brk.BreakPolarity           = TIM_BREAKPOLARITY_HIGH;
        cfg_brk.AutomaticOutput         = TIM_AUTOMATICOUTPUT_DISABLE;
        HAL_TIMEx_ConfigBreakDeadTime( &htim16, &cfg_brk );
}


/*******************************************************************************
* PUBLIC FUNCTIONS
*******************************************************************************/
/**
  * @brief  
  * @param  None
  * @retval None
  */
void
stm32_tim16_init( void )
{
        init_pwm();
        init_io();
}


/**
  * @brief  
  * @param  None
  * @retval None
  */
int
stm32_tim16_run( void )
{
        if( HAL_TIM_Base_Start( &htim16 ) != HAL_OK )
        {
                return( -1 );
        }

        if( HAL_TIM_PWM_Start( &htim16, TIM_CHANNEL_1 ) != HAL_OK )
        {
                return( -2 );
        }

        return( 0 );
}


/**
  * @brief  
  * @param  None
  * @retval None
  */
void
stm32_tim16_set(                              const   uint16_t        raw16 )
{
        __HAL_TIM_SET_COMPARE( &htim16, TIM_CHANNEL_1, raw16 );
}


/**
  * @brief  
  * @param  None
  * @retval None
  */
uint16_t
stm32_tim16_get( void )
{
        return( __HAL_TIM_GET_COMPARE( &htim16, TIM_CHANNEL_1 ) );
}
