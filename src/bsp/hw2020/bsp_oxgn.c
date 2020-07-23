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


static
void
bsp_oxgn_init_io( void )
{
	GPIO_InitTypeDef        pin_adc_lo      =   {    .Pin       =    GPIO_PIN_4,
	                                                 .Mode      =    GPIO_MODE_ANALOG,
	                                                 .Pull      =    GPIO_NOPULL,
	                                                 .Speed     =    GPIO_SPEED_FREQ_LOW };

	GPIO_InitTypeDef        pin_adc_mi      =   {    .Pin       =    GPIO_PIN_6,
	                                                 .Mode      =    GPIO_MODE_ANALOG,
	                                                 .Pull      =    GPIO_NOPULL,
	                                                 .Speed     =    GPIO_SPEED_FREQ_LOW };
/*
	GPIO_InitTypeDef        pin_adc_hi      =   {    .Pin       =    GPIO_PIN_4,
	                                                 .Mode      =    GPIO_MODE_ANALOG,
	                                                 .Pull      =    GPIO_NOPULL,
	                                                 .Speed     =    GPIO_SPEED_FREQ_LOW };
*/

        __HAL_RCC_GPIOA_CLK_ENABLE();

	HAL_GPIO_Init( GPIOA, &pin_adc_lo );
	HAL_GPIO_Init( GPIOA, &pin_adc_mi );
	//HAL_GPIO_Init( GPIOA, &pin_adc_hi );
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

        cfg.Channel                             = ADC_CHANNEL_6;
        cfg.Rank                                = ADC_RANK_CHANNEL_NUMBER;
        cfg.SamplingTime                        = ADC_SAMPLETIME_239CYCLES_5;
        if( HAL_ADC_ConfigChannel( &hadc, &cfg ) != HAL_OK )
        {
                app_error_handler();
        }

        cfg.Channel                             = ADC_CHANNEL_8;
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
