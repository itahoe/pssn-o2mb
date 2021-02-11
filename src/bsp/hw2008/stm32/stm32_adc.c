/**
  * @file    stm32_adc.c
  * @brief   ADC routines
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include "stm32.h"
#include "stm32f0xx_ll_adc.h"


/*******************************************************************************
* PRIVATE VARIABLES
*******************************************************************************/
static  TIM_HandleTypeDef       htim15;
static  ADC_HandleTypeDef       hadc;
static  DMA_HandleTypeDef       hdma;


/*******************************************************************************
* PRIVATE FUNCTIONS
*******************************************************************************/
static
void
init_io( void )
{
	GPIO_InitTypeDef        pin_adc         =   {    .Pin       =    GPIO_PIN_4,
	                                                 .Mode      =    GPIO_MODE_ANALOG,
	                                                 .Pull      =    GPIO_NOPULL,
	                                                 .Speed     =    GPIO_SPEED_FREQ_LOW };

	HAL_GPIO_Init( GPIOA, &pin_adc );
}


static
void
init_timebase( void )
{
        TIM_ClockConfigTypeDef  clk     = {0};
        TIM_MasterConfigTypeDef cfg     = {0};


        __HAL_RCC_TIM15_FORCE_RESET();
        __HAL_RCC_TIM15_RELEASE_RESET();
        __HAL_RCC_TIM15_CLK_ENABLE();

        htim15.Instance                   = TIM15;
        //htim15.Init.Prescaler             = 24000;
        htim15.Init.Prescaler             = 48000;
        htim15.Init.CounterMode           = TIM_COUNTERMODE_UP;
        //htim15.Init.Period                = 1000 / samplerate_sps;
        htim15.Init.ClockDivision         = TIM_CLOCKDIVISION_DIV1;
        htim15.Init.RepetitionCounter     = 0;
        htim15.Init.AutoReloadPreload     = TIM_AUTORELOAD_PRELOAD_DISABLE;
        HAL_TIM_Base_Init( &htim15 );

        clk.ClockSource                 = TIM_CLOCKSOURCE_INTERNAL;
        HAL_TIM_ConfigClockSource( &htim15, &clk );

        cfg.MasterOutputTrigger         = TIM_TRGO_UPDATE;
        cfg.MasterSlaveMode             = TIM_MASTERSLAVEMODE_DISABLE;
        HAL_TIMEx_MasterConfigSynchronization( &htim15, &cfg );
}


static
void
init_adc( void )
{
        ADC_ChannelConfTypeDef  cfg     = {0};


        __HAL_RCC_ADC1_CLK_ENABLE();

        hadc.Instance                           = ADC1;

        HAL_ADC_DeInit( &hadc );

        hadc.Init.ClockPrescaler                = ADC_CLOCK_SYNC_PCLK_DIV4;
        hadc.Init.Resolution                    = ADC_RESOLUTION_12B;
        hadc.Init.DataAlign                     = ADC_DATAALIGN_RIGHT;
        hadc.Init.ScanConvMode                  = ADC_SCAN_DIRECTION_FORWARD;
        hadc.Init.EOCSelection                  = ADC_EOC_SEQ_CONV;
        hadc.Init.LowPowerAutoWait              = DISABLE;
        hadc.Init.LowPowerAutoPowerOff          = DISABLE;
        hadc.Init.ContinuousConvMode            = ENABLE;
        hadc.Init.DiscontinuousConvMode         = DISABLE;
        //hadc.Init.ExternalTrigConv              = ADC_EXTERNALTRIGCONV_T15_TRGO;
        //hadc.Init.ExternalTrigConvEdge          = ADC_EXTERNALTRIGCONVEDGE_RISING;
        hadc.Init.ExternalTrigConv              = ADC_SOFTWARE_START;
        hadc.Init.ExternalTrigConvEdge          = ADC_EXTERNALTRIGCONVEDGE_NONE;
        hadc.Init.DMAContinuousRequests         = ENABLE;
        hadc.Init.Overrun                       = ADC_OVR_DATA_OVERWRITTEN;
        HAL_ADC_Init( &hadc );

        cfg.Channel                             = ADC_CHANNEL_4;
        cfg.Rank                                = ADC_RANK_CHANNEL_NUMBER;
        cfg.SamplingTime                        = ADC_SAMPLETIME_239CYCLES_5;
        HAL_ADC_ConfigChannel( &hadc, &cfg );

        cfg.Channel                             = ADC_CHANNEL_VREFINT;
        HAL_ADC_ConfigChannel( &hadc, &cfg );

        cfg.Channel                             = ADC_CHANNEL_TEMPSENSOR;
        HAL_ADC_ConfigChannel( &hadc, &cfg );
}


static
void
init_adc_dma( void )
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
        HAL_DMA_Init( &hdma );

        __HAL_LINKDMA( &hadc, DMA_Handle, hdma );
}


/*******************************************************************************
* PUBLIC FUNCTIONS
*******************************************************************************/
void
stm32_adc_init( void )
{
        init_io();
        init_timebase();
        init_adc();
        init_adc_dma();

        NVIC_SetPriority(       ADC1_IRQn,              STM32_NVIC_PRIO_SENS    );
        NVIC_EnableIRQ(         ADC1_IRQn                                       );

        NVIC_SetPriority(       DMA1_Channel1_IRQn,     STM32_NVIC_PRIO_SENS    );
        NVIC_EnableIRQ(         DMA1_Channel1_IRQn                              );  

        NVIC_SetPriority(       TIM15_IRQn,             STM32_NVIC_PRIO_SENS    );
        NVIC_EnableIRQ(         TIM15_IRQn                                      );
}


void
stm32_adc_config_sps(                   const   size_t  samplerate_sps )
{
        htim15.Init.Period                = 1000 / samplerate_sps;
}


/**
  * @brief  
  * @param  None
  * @retval None
  */
void
stm32_adc_start(                                uint16_t *      data,
                                        const   size_t          len )
{
        HAL_ADCEx_Calibration_Start( &hadc );
        HAL_ADC_Start_DMA( &hadc, (uint32_t *) data, len );
        //HAL_TIM_Base_Start( &htim15 );
}


/*******************************************************************************
* CONVERTION ROUTINES
*******************************************************************************/
/**
  * @brief  
  * @param  None
  * @retval None
  */
int32_t
stm32_adc_raw_to_vref_mV(               const   int32_t         smpl )
{
        return( __LL_ADC_CALC_VREFANALOG_VOLTAGE( smpl, LL_ADC_RESOLUTION_12B ) );
}


/**
  * @brief  
  * @param  None
  * @retval None
  */
int32_t
stm32_adc_raw_to_celsius(               const   int32_t         smpl,
                                        const   int32_t         vref_mv )
{
        return( __LL_ADC_CALC_TEMPERATURE( vref_mv, smpl, LL_ADC_RESOLUTION_12B ) );
}


/**
  * @brief  
  * @param  None
  * @retval None
  */
/*
void
bsp_oxgn_timebase_isr( void )
{
}
*/

/*******************************************************************************
* INTERRUPT SERVICE ROUTINES
*******************************************************************************/
/**
  * @brief  
  * @param  None
  * @retval None
  */
void
stm32_adc_isr( void )
{
        HAL_ADC_IRQHandler( &hadc );
}


/**
  * @brief  
  * @param  None
  * @retval None
  */
void
stm32_adc_dma_isr( void )
{
        HAL_DMA_IRQHandler( hadc.DMA_Handle );
}


/**
  * @brief  
  * @param  None
  * @retval None
  */
/*
void
stm32_adc_timebase_isr( void )
{
        HAL_TIM_IRQHandler( &htim15 );
}
*/
