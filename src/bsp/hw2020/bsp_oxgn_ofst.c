/**
  * @file    bsp_oxgn_ofst.c
  * @brief   Oxigen Sensor Offset Hardware Routins
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include "app.h"
#include "bsp.h"


static  TIM_HandleTypeDef       htim_ofst;


/*******************************************************************************
* OXGN OFST
*******************************************************************************/
static
void
bsp_oxgn_ofst_init_io( void )
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
bsp_oxgn_ofst_init_pwm( void )
{
        TIM_OC_InitTypeDef              cfg_oc          = {0};
        TIM_BreakDeadTimeConfigTypeDef  cfg_brk         = {0};


        __HAL_RCC_TIM16_FORCE_RESET();
        __HAL_RCC_TIM16_RELEASE_RESET();
        __HAL_RCC_TIM16_CLK_ENABLE();


        htim_ofst.Instance                      = TIM16;
        htim_ofst.Init.Prescaler                = 1;
        htim_ofst.Init.CounterMode              = TIM_COUNTERMODE_UP;
        htim_ofst.Init.Period                   = 0xFFFF;
        htim_ofst.Init.ClockDivision            = TIM_CLOCKDIVISION_DIV1;
        htim_ofst.Init.RepetitionCounter        = 0;
        htim_ofst.Init.AutoReloadPreload        = TIM_AUTORELOAD_PRELOAD_DISABLE;
        if( HAL_TIM_Base_Init( &htim_ofst ) != HAL_OK )
        {
                app_error_handler();
        }

        if( HAL_TIM_PWM_Init( &htim_ofst ) != HAL_OK )
        {
                app_error_handler();
        }

        cfg_oc.OCMode                           = TIM_OCMODE_PWM1;
        cfg_oc.Pulse                            = 0;
        cfg_oc.OCPolarity                       = TIM_OCPOLARITY_HIGH;
        cfg_oc.OCNPolarity                      = TIM_OCNPOLARITY_HIGH;
        //cfg_oc.OCFastMode                       = TIM_OCFAST_ENABLE;
        cfg_oc.OCFastMode                       = TIM_OCFAST_DISABLE;
        cfg_oc.OCIdleState                      = TIM_OCIDLESTATE_RESET;
        cfg_oc.OCNIdleState                     = TIM_OCNIDLESTATE_RESET;
        if( HAL_TIM_PWM_ConfigChannel( &htim_ofst, &cfg_oc, TIM_CHANNEL_1 ) != HAL_OK )
        {
                app_error_handler();
        }

        cfg_brk.OffStateRunMode                 = TIM_OSSR_DISABLE;
        cfg_brk.OffStateIDLEMode                = TIM_OSSI_DISABLE;
        cfg_brk.LockLevel                       = TIM_LOCKLEVEL_OFF;
        cfg_brk.DeadTime                        = 0;
        cfg_brk.BreakState                      = TIM_BREAK_DISABLE;
        cfg_brk.BreakPolarity                   = TIM_BREAKPOLARITY_HIGH;
        cfg_brk.AutomaticOutput                 = TIM_AUTOMATICOUTPUT_DISABLE;
        if( HAL_TIMEx_ConfigBreakDeadTime( &htim_ofst, &cfg_brk ) != HAL_OK )
        {
                app_error_handler();
        }
}


/**
  * @brief  
  * @param  None
  * @retval None
  */
void
bsp_oxgn_ofst_init(                     const   size_t          samplerate_sps )
{
        //bsp_oxgn_ofst_init_pwm();
        //bsp_oxgn_ofst_init_io();
}


/**
  * @brief  
  * @param  None
  * @retval None
  */
void
bsp_oxgn_ofst_run( void )
{
/*
        if( HAL_TIM_Base_Start( &htim_ofst ) != HAL_OK )
        {
                app_error_handler();
        }

        if( HAL_TIM_PWM_Start( &htim_ofst, TIM_CHANNEL_1 ) != HAL_OK )
        {
                app_error_handler();
        }
*/
}


/**
  * @brief  
  * @param  None
  * @retval None
  */
void
bsp_oxgn_ofst_set(                              const   uint16_t        raw16 )
{
        //__HAL_TIM_SET_COMPARE( &htim_ofst, TIM_CHANNEL_1, raw16 );
}


/**
  * @brief  
  * @param  None
  * @retval None
  */
uint16_t
bsp_oxgn_ofst_get( void )
{
        //return( __HAL_TIM_GET_COMPARE( &htim_ofst, TIM_CHANNEL_1 ) );
        return( 0 );
}
