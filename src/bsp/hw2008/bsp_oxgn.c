/**
  * @file    bsp_oxgn.c
  * @brief   Oxigen Sensor ADC Hardware Routins
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include "app.h"
#include "bsp.h"


static  TIM_HandleTypeDef       htim;
static  ADC_HandleTypeDef       hadc;
static  DMA_HandleTypeDef       hdma;
static  TIM_HandleTypeDef       htim_ofst;


static
void
bsp_oxgn_init_io( void )
{
	GPIO_InitTypeDef        pin_adc         =   {    .Pin       =    GPIO_PIN_4,
	                                                 .Mode      =    GPIO_MODE_ANALOG,
	                                                 .Pull      =    GPIO_NOPULL,
	                                                 .Speed     =    GPIO_SPEED_FREQ_LOW };


        __HAL_RCC_GPIOA_CLK_ENABLE();
	HAL_GPIO_Init( GPIOA, &pin_adc );
}


static
void
bsp_oxgn_init_timebase(                 const   size_t          samplerate_sps )
{
        TIM_ClockConfigTypeDef  clk     = {0};
        TIM_MasterConfigTypeDef cfg     = {0};


        __HAL_RCC_TIM15_FORCE_RESET();
        __HAL_RCC_TIM15_RELEASE_RESET();
        __HAL_RCC_TIM15_CLK_ENABLE();

        htim.Instance                   = TIM15;
        //htim.Init.Prescaler             = 24000;
        htim.Init.Prescaler             = 48000;
        htim.Init.CounterMode           = TIM_COUNTERMODE_UP;
        htim.Init.Period                = 1000 / samplerate_sps;
        htim.Init.ClockDivision         = TIM_CLOCKDIVISION_DIV1;
        htim.Init.RepetitionCounter     = 0;
        htim.Init.AutoReloadPreload     = TIM_AUTORELOAD_PRELOAD_DISABLE;
        if( HAL_TIM_Base_Init( &htim ) != HAL_OK )
        {
                app_error_handler();
        }

        clk.ClockSource                 = TIM_CLOCKSOURCE_INTERNAL;
        if( HAL_TIM_ConfigClockSource( &htim, &clk ) != HAL_OK )
        {
                app_error_handler();
        }

        cfg.MasterOutputTrigger         = TIM_TRGO_UPDATE;
        cfg.MasterSlaveMode             = TIM_MASTERSLAVEMODE_DISABLE;
        if( HAL_TIMEx_MasterConfigSynchronization( &htim, &cfg ) != HAL_OK )
        {
                app_error_handler();
        }
}


static
void
bsp_oxgn_init_adc( void )
{
        ADC_ChannelConfTypeDef  cfg     = {0};


        __HAL_RCC_ADC1_CLK_ENABLE();

        hadc.Instance                           = ADC1;

        if( HAL_ADC_DeInit( &hadc ) != HAL_OK )
        {
                app_error_handler();
        }

        hadc.Init.ClockPrescaler                = ADC_CLOCK_ASYNC_DIV1;
        hadc.Init.Resolution                    = ADC_RESOLUTION_12B;
        hadc.Init.DataAlign                     = ADC_DATAALIGN_RIGHT;
        hadc.Init.ScanConvMode                  = ADC_SCAN_DIRECTION_FORWARD;
        hadc.Init.EOCSelection                  = ADC_EOC_SEQ_CONV;
        hadc.Init.LowPowerAutoWait              = DISABLE;
        hadc.Init.LowPowerAutoPowerOff          = DISABLE;
        hadc.Init.ContinuousConvMode            = DISABLE;
        hadc.Init.DiscontinuousConvMode         = ENABLE;
        hadc.Init.ExternalTrigConv              = ADC_EXTERNALTRIGCONV_T15_TRGO;
        hadc.Init.ExternalTrigConvEdge          = ADC_EXTERNALTRIGCONVEDGE_RISING;
        hadc.Init.DMAContinuousRequests         = ENABLE;
        hadc.Init.Overrun                       = ADC_OVR_DATA_OVERWRITTEN;
        if( HAL_ADC_Init( &hadc ) != HAL_OK )
        {
                app_error_handler();
        }

        cfg.Channel                             = ADC_CHANNEL_4;
        cfg.Rank                                = ADC_RANK_CHANNEL_NUMBER;
        cfg.SamplingTime                        = ADC_SAMPLETIME_239CYCLES_5;
        if( HAL_ADC_ConfigChannel( &hadc, &cfg ) != HAL_OK )
        {
                app_error_handler();
        }

        cfg.Channel                             = ADC_CHANNEL_VREFINT;
        if( HAL_ADC_ConfigChannel( &hadc, &cfg ) != HAL_OK )
        {
                app_error_handler();
        }

        cfg.Channel                             = ADC_CHANNEL_TEMPSENSOR;
        if( HAL_ADC_ConfigChannel( &hadc, &cfg ) != HAL_OK      )
        {
                app_error_handler();
        }
}


static
void
bsp_oxgn_init_adc_dma( void )
{
        hdma.Instance                   = DMA1_Channel1;
        //hdma.Init.Request               = DMA_REQUEST_0;
        hdma.Init.Direction             = DMA_PERIPH_TO_MEMORY;
        hdma.Init.PeriphInc             = DMA_PINC_DISABLE;
        hdma.Init.MemInc                = DMA_MINC_ENABLE;
        hdma.Init.PeriphDataAlignment   = DMA_PDATAALIGN_HALFWORD;
        hdma.Init.MemDataAlignment      = DMA_MDATAALIGN_HALFWORD;
        hdma.Init.Mode                  = DMA_CIRCULAR;
        hdma.Init.Priority              = DMA_PRIORITY_MEDIUM;
        if( HAL_DMA_Init( &hdma ) != HAL_OK )
        {
                app_error_handler();
        }

        __HAL_LINKDMA( &hadc, DMA_Handle, hdma );
}


void
bsp_oxgn_init(                          const   size_t          samplerate_sps )
{
        bsp_oxgn_init_io();
        bsp_oxgn_init_timebase( samplerate_sps );
        bsp_oxgn_init_adc();
        bsp_oxgn_init_adc_dma();

        NVIC_SetPriority(       ADC1_IRQn,              BSP_NVIC_PRIO_SENS      );
        NVIC_EnableIRQ(         ADC1_IRQn                                       );

        NVIC_SetPriority(       DMA1_Channel1_IRQn,     BSP_NVIC_PRIO_SENS      );
        NVIC_EnableIRQ(         DMA1_Channel1_IRQn                              );  

        //NVIC_SetPriority(       TIM15_IRQn,      BSP_NVIC_PRIO_SENS      );
        //NVIC_EnableIRQ(         TIM15_IRQn                               );
}


/**
  * @brief  
  * @param  None
  * @retval None
  */
void
bsp_oxgn_run(                           const   uint16_t *      data,
                                        const   size_t          len )
{
        if( HAL_ADCEx_Calibration_Start( &hadc ) != HAL_OK )
        {
                app_error_handler();
        }

        if( HAL_ADC_Start_DMA( &hadc, (uint32_t *) data, len ) != HAL_OK )
        {
                app_error_handler();
        }

        if( HAL_TIM_Base_Start( &htim ) != HAL_OK )
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
bsp_oxgn_timebase_isr( void )
{
}


/**
  * @brief  
  * @param  None
  * @retval None
  */
void
bsp_oxgn_adc_isr( void )
{
        HAL_ADC_IRQHandler( &hadc );
}


/**
  * @brief  
  * @param  None
  * @retval None
  */
void
bsp_oxgn_dma_isr( void )
{
        HAL_DMA_IRQHandler( hadc.DMA_Handle );
}


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
        bsp_oxgn_ofst_init_pwm();
        bsp_oxgn_ofst_init_io();
}


/**
  * @brief  
  * @param  None
  * @retval None
  */
void
bsp_oxgn_ofst_run( void )
{
        if( HAL_TIM_Base_Start( &htim_ofst ) != HAL_OK )
        {
                app_error_handler();
        }

        if( HAL_TIM_PWM_Start( &htim_ofst, TIM_CHANNEL_1 ) != HAL_OK )
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
bsp_oxgn_ofst_set(                              const   uint16_t        raw16 )
{
        __HAL_TIM_SET_COMPARE( &htim_ofst, TIM_CHANNEL_1, raw16 );
}


/**
  * @brief  
  * @param  None
  * @retval None
  */
uint16_t
bsp_oxgn_ofst_get( void )
{
        return( __HAL_TIM_GET_COMPARE( &htim_ofst, TIM_CHANNEL_1 ) );
}
