/**
  * @file    stm32_tim3.c
  * @brief   TIM3 Hardware Routins
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include "stm32.h"


/*******************************************************************************
* PRIVATE VARIABLES
*******************************************************************************/
static  uint16_t        const   pin_vref        = GPIO_PIN_1;
static  uint16_t        const   pin_avcc_neg    = GPIO_PIN_4;
static  uint16_t        const   pin_avcc_pos    = GPIO_PIN_5;

static  TIM_HandleTypeDef       htim3;


/*******************************************************************************
* PRIVATE FUNCTIONS
*******************************************************************************/
static
int
init_timebase( void )
{
        TIM_ClockConfigTypeDef  clk     = {0};
        TIM_MasterConfigTypeDef cfg     = {0};


        __HAL_RCC_TIM3_FORCE_RESET();
        __HAL_RCC_TIM3_RELEASE_RESET();
        __HAL_RCC_TIM3_CLK_ENABLE();

        htim3.Instance                   = TIM3;
        htim3.Init.Prescaler             = 5;
        //htim3.Init.Period                = 1000 / samplerate_sps;
        htim3.Init.CounterMode           = TIM_COUNTERMODE_UP;
        htim3.Init.ClockDivision         = TIM_CLOCKDIVISION_DIV1;
        htim3.Init.AutoReloadPreload     = TIM_AUTORELOAD_PRELOAD_DISABLE;
        if( HAL_TIM_Base_Init( &htim3 ) != HAL_OK )
        {
                return( -1 );
        }

        clk.ClockSource                 = TIM_CLOCKSOURCE_INTERNAL;
        if( HAL_TIM_ConfigClockSource( &htim3, &clk ) != HAL_OK )
        {
                return( -2 );
        }

        cfg.MasterOutputTrigger         = TIM_TRGO_UPDATE;
        cfg.MasterSlaveMode             = TIM_MASTERSLAVEMODE_DISABLE;
        if( HAL_TIMEx_MasterConfigSynchronization( &htim3, &cfg ) != HAL_OK )
        {
                return( -3 );
        }

        //NVIC_SetPriority(       TIM3_IRQn,      STM32_NVIC_PRIO_TIM3    );
        //NVIC_EnableIRQ(         TIM3_IRQn                               );


        return( 0 );
}


static
void
init_io( void )
{
        GPIO_InitTypeDef        pins    = {     .Pin            =    pin_vref | pin_avcc_neg | pin_avcc_pos,
	                                        .Mode           =    GPIO_MODE_AF_PP,
	                                        .Pull           =    GPIO_NOPULL,
	                                        .Speed          =    GPIO_SPEED_FREQ_LOW,
	                                        .Alternate      =    GPIO_AF1_TIM3 };

        HAL_GPIO_Init( GPIOB, &pins );
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

        /*
        htim3.Instance = TIM3;
        htim3.Init.Prescaler = 0;
        htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
        //htim3.Init.Period = 65535;
        htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
        htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
        */
        if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
        {
                return( -1 );
        }

        if( HAL_TIM_OC_Init(&htim3) != HAL_OK )
        {
                return( -1 );
        }

        sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
        sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
        if( HAL_TIMEx_MasterConfigSynchronization( &htim3, &sMasterConfig ) != HAL_OK )
        {
                return( -1 );
        }


        sConfigOC.OCMode        = TIM_OCMODE_PWM1;
        sConfigOC.Pulse         = (htim3.Init.Period + 1) / 2;
        sConfigOC.OCPolarity    = TIM_OCPOLARITY_HIGH;
        sConfigOC.OCFastMode    = TIM_OCFAST_DISABLE;

        if( HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1) != HAL_OK )
        {
                return( -1 );
        }

        if( HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_2) != HAL_OK )
        {
                return( -1 );
        }

        sConfigOC.OCMode = TIM_OCMODE_TIMING;
        if( HAL_TIM_OC_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_4) != HAL_OK )
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
stm32_tim3_init( void )
{
        init_timebase();
        init_io();
        init_pwm();
        init_dma();

        return( 0 );
}


/**
  * @brief  
  * @param  None
  * @retval None
  */
void
stm32_tim3_config_period(               const   uint32_t        period )
{
        //htim3.Init.Period       = 1000 / period;
        htim3.Init.Period       = period - 1;
}


/**
  * @brief  
  * @param  None
  * @retval None
  */
int
stm32_tim3_start( void )
{
        __HAL_TIM_ENABLE_IT( &htim3, TIM_IT_UPDATE );


        if( HAL_TIM_PWM_Start( &htim3, TIM_CHANNEL_1 ) != HAL_OK )
        {
                return( -1 );
        }

        if( HAL_TIM_PWM_Start( &htim3, TIM_CHANNEL_2 ) != HAL_OK )
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
stm32_tim3_isr_is_update( void )
{
        int     update  = 0;


        if( __HAL_TIM_GET_FLAG( &htim3, TIM_FLAG_UPDATE ) )
        {
                __HAL_TIM_CLEAR_FLAG( &htim3, TIM_FLAG_UPDATE );
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
stm32_tim3_isr( void )
{
        HAL_TIM_IRQHandler( &htim3 );
}


/**
  * @brief  
  * @param  None
  * @retval None
  */
void
stm32_tim3_dma_isr( void )
{
        HAL_DMA_IRQHandler( htim3.hdma[0] );
}
