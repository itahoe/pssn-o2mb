/**
  * @file    stm32_tim6.c
  * @brief   TIM6 Hardware Routins
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include "stm32.h"


/*******************************************************************************
* PRIVATE VARIABLES
*******************************************************************************/
static  TIM_HandleTypeDef       htim6;


/*******************************************************************************
* PRIVATE FUNCTIONS
*******************************************************************************/
static
int
init_timebase( void )
{
        TIM_ClockConfigTypeDef  clk     = {0};
        TIM_MasterConfigTypeDef cfg     = {0};


        __HAL_RCC_TIM6_FORCE_RESET();
        __HAL_RCC_TIM6_RELEASE_RESET();
        __HAL_RCC_TIM6_CLK_ENABLE();

        htim6.Instance                   = TIM6;
        htim6.Init.Prescaler             = 48000;
        htim6.Init.CounterMode           = TIM_COUNTERMODE_UP;
        //htim6.Init.Period                = 1000 / samplerate_sps;
        htim6.Init.ClockDivision         = TIM_CLOCKDIVISION_DIV1;
        htim6.Init.AutoReloadPreload     = TIM_AUTORELOAD_PRELOAD_DISABLE;
        if( HAL_TIM_Base_Init( &htim6 ) != HAL_OK )
        {
                return( -1 );
        }

        clk.ClockSource                 = TIM_CLOCKSOURCE_INTERNAL;
        if( HAL_TIM_ConfigClockSource( &htim6, &clk ) != HAL_OK )
        {
                return( -2 );
        }

        cfg.MasterOutputTrigger         = TIM_TRGO_UPDATE;
        cfg.MasterSlaveMode             = TIM_MASTERSLAVEMODE_DISABLE;
        if( HAL_TIMEx_MasterConfigSynchronization( &htim6, &cfg ) != HAL_OK )
        {
                return( -3 );
        }

        NVIC_SetPriority(       TIM6_IRQn,      STM32_NVIC_PRIO_TIM6    );
        NVIC_EnableIRQ(         TIM6_IRQn                               );


        return( 0 );
}


static
void
init_io( void )
{
}


static
void
init_dma( void )
{
}


static
int
init_pwm( void )
{
        TIM_MasterConfigTypeDef sMasterConfig   = {0};
        TIM_OC_InitTypeDef      sConfigOC       = {0};


        htim6.Instance = TIM6;
        htim6.Init.Prescaler = 48000;
        htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
        //htim6.Init.Period = 65535;
        htim6.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
        htim6.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
        if (HAL_TIM_PWM_Init(&htim6) != HAL_OK)
        {
                return( -1 );
        }

        if( HAL_TIM_OC_Init(&htim6) != HAL_OK )
        {
                return( -1 );
        }

        sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
        sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
        if( HAL_TIMEx_MasterConfigSynchronization( &htim6, &sMasterConfig ) != HAL_OK )
        {
                return( -1 );
        }


        sConfigOC.OCMode        = TIM_OCMODE_PWM1;
        sConfigOC.Pulse         = htim6.Init.Period / 2;
        sConfigOC.OCPolarity    = TIM_OCPOLARITY_HIGH;
        sConfigOC.OCFastMode    = TIM_OCFAST_DISABLE;

        if( HAL_TIM_PWM_ConfigChannel(&htim6, &sConfigOC, TIM_CHANNEL_1) != HAL_OK )
        {
                return( -1 );
        }

        if( HAL_TIM_PWM_ConfigChannel(&htim6, &sConfigOC, TIM_CHANNEL_2) != HAL_OK )
        {
                return( -1 );
        }

        sConfigOC.OCMode = TIM_OCMODE_TIMING;
        if( HAL_TIM_OC_ConfigChannel(&htim6, &sConfigOC, TIM_CHANNEL_4) != HAL_OK )
        {
                return( -1 );
        }

        return( 0 );
}


/*******************************************************************************
* PUBLIC FUNCTIONS
*******************************************************************************/
/**
  * @brief  
  * @param  None
  * @retval None
  */
int
stm32_tim6_init( void )
{
        init_timebase();
        init_io();
        //init_pwm();
        init_dma();

        return( 0 );
}


/**
  * @brief  
  * @param  None
  * @retval None
  */
void
stm32_tim6_config_period(               const   uint32_t        period )
{
        htim6.Init.Period       = 1000 / period;
}


/**
  * @brief  
  * @param  None
  * @retval None
  */
int
stm32_tim6_start( void )
{
        __HAL_TIM_ENABLE_IT( &htim6, TIM_IT_UPDATE );

        if( HAL_TIM_Base_Start( &htim6 ) != HAL_OK )
        {
                return( -1 );
        }

        return( 0 );
}


/*******************************************************************************
* Interrupt Service Routines
*******************************************************************************/
/**
  * @brief  
  * @param  None
  * @retval None
  */
int
stm32_tim6_isr_is_update( void )
{
        int     update  = 0;


        if( __HAL_TIM_GET_FLAG( &htim6, TIM_FLAG_UPDATE ) )
        {
                __HAL_TIM_CLEAR_FLAG( &htim6, TIM_FLAG_UPDATE );
                update  = -1;
        }

        return( update );
}


/**
  * @brief  
  * @param  None
  * @retval None
  */
void
stm32_tim6_isr( void )
{
        HAL_TIM_IRQHandler( &htim6 );
}


/**
  * @brief  
  * @param  None
  * @retval None
  */
void
stm32_tim6_dma_isr( void )
{
        HAL_DMA_IRQHandler( htim6.hdma[0] );
}
