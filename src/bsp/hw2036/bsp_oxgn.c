/**
  * @file    bsp_oxgn.c
  * @brief   Oxigen Sensor ADC Hardware Routins
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include "app.h"
#include "bsp.h"
#include "ad7799.h"


/*
static  SPI_HandleTypeDef       hspi;
*/

static  ADC_HandleTypeDef       hadc;
static  DMA_HandleTypeDef       hdma_adc, hdma_rx, hdma_tx;
static  TIM_HandleTypeDef       htim;


static
void
bsp_oxgn_init_timebase(                 const   size_t          samplerate_sps )
{
        TIM_ClockConfigTypeDef  clk     = {0};
        TIM_MasterConfigTypeDef cfg     = {0};


        __HAL_RCC_TIM3_FORCE_RESET();
        __HAL_RCC_TIM3_RELEASE_RESET();
        __HAL_RCC_TIM3_CLK_ENABLE();

        htim.Instance                   = TIM3;
        htim.Init.Prescaler             = 48000;
        htim.Init.CounterMode           = TIM_COUNTERMODE_UP;
        htim.Init.Period                = 1000 / samplerate_sps;
        htim.Init.ClockDivision         = TIM_CLOCKDIVISION_DIV1;
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

        NVIC_SetPriority(       TIM3_IRQn,      BSP_NVIC_PRIO_SENS      );
        NVIC_EnableIRQ(         TIM3_IRQn                               );
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
        hadc.Init.ExternalTrigConv              = ADC_SOFTWARE_START;
        //hadc.Init.ExternalTrigConv              = ADC_EXTERNALTRIGCONV_T3_TRGO;
        //hadc.Init.ExternalTrigConvEdge          = ADC_EXTERNALTRIGCONVEDGE_RISING;
        hadc.Init.DMAContinuousRequests         = ENABLE;
        hadc.Init.Overrun                       = ADC_OVR_DATA_OVERWRITTEN;
        if( HAL_ADC_Init( &hadc ) != HAL_OK )
        {
                app_error_handler();
        }

        cfg.Channel                             = ADC_CHANNEL_VREFINT;
        cfg.Rank                                = ADC_RANK_CHANNEL_NUMBER;
        cfg.SamplingTime                        = ADC_SAMPLETIME_239CYCLES_5;
        if( HAL_ADC_ConfigChannel( &hadc, &cfg ) != HAL_OK )
        {
                app_error_handler();
        }

        cfg.Channel                             = ADC_CHANNEL_TEMPSENSOR;
        cfg.Rank                                = ADC_RANK_CHANNEL_NUMBER;
        cfg.SamplingTime                        = ADC_SAMPLETIME_239CYCLES_5;
        if( HAL_ADC_ConfigChannel( &hadc, &cfg ) != HAL_OK      )
        {
                app_error_handler();
        }
}


static
void
bsp_oxgn_init_adc_io( void )
{
}


static
void
bsp_oxgn_init_adc_dma( void )
{
        hdma_adc.Instance                       = DMA1_Channel1;
        hdma_adc.Init.Direction                 = DMA_PERIPH_TO_MEMORY;
        hdma_adc.Init.PeriphInc                 = DMA_PINC_DISABLE;
        hdma_adc.Init.MemInc                    = DMA_MINC_ENABLE;
        hdma_adc.Init.PeriphDataAlignment       = DMA_PDATAALIGN_HALFWORD;
        hdma_adc.Init.MemDataAlignment          = DMA_MDATAALIGN_HALFWORD;
        //hdma_adc.Init.Mode                      = DMA_NORMAL;
        hdma_adc.Init.Mode                      = DMA_CIRCULAR;
        hdma_adc.Init.Priority                  = DMA_PRIORITY_MEDIUM;

        if( HAL_DMA_Init( &hdma_adc ) != HAL_OK )
        {
                app_error_handler();
        }

        __HAL_LINKDMA( &hadc, DMA_Handle, hdma_adc );

        //__HAL_ADC_ENABLE_IT( &hadc, ADC_FLAG_EOS );
}



void
bsp_oxgn_init(                          const   size_t          samplerate_sps )
{
        bsp_oxgn_init_timebase( samplerate_sps );

        bsp_oxgn_init_adc();
        bsp_oxgn_init_adc_io();
        bsp_oxgn_init_adc_dma();

        //bsp_oxgn_init_spi();
        //bsp_oxgn_init_spi_io();
        //bsp_oxgn_init_spi_dma();


        NVIC_SetPriority(       ADC1_IRQn,              BSP_NVIC_PRIO_SENS      );
        NVIC_EnableIRQ(         ADC1_IRQn                                       );

        //NVIC_SetPriority(       DMA1_Channel1_IRQn,     BSP_NVIC_PRIO_SENS      );
        //NVIC_EnableIRQ(         DMA1_Channel1_IRQn                              );  

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


        __HAL_TIM_ENABLE_IT( &htim, TIM_IT_UPDATE );

        if( HAL_TIM_Base_Start( &htim ) != HAL_OK )
        {
                app_error_handler();
        }
}


int32_t
bsp_oxgn_read( void )
{
        //HAL_ADC_Start_IT( &hadc );

        ad7799_set_mode( AD7799_MODE_SINGLE );
        while( !ad7799_sts_ready() );

        return( ad7799_get_data() );
}


/**
  * @brief  
  * @param  None
  * @retval None
  */
void
bsp_oxgn_timebase_isr( void )
{
        //HAL_TIM_IRQHandler( &htim );

        if( __HAL_TIM_GET_FLAG( &htim, TIM_FLAG_UPDATE ) )
        {
                __HAL_TIM_CLEAR_FLAG( &htim, TIM_FLAG_UPDATE );
        }
}


/**
  * @brief  
  * @param  None
  * @retval None
  */
bool
bsp_oxgn_adc_isr( void )
{
        bool    end_of_seq      = false;

        if( __HAL_ADC_GET_FLAG( &hadc, ADC_FLAG_EOS ) )
        {
                __HAL_ADC_CLEAR_FLAG( &hadc, ADC_FLAG_EOS );

                end_of_seq      = true;
        }


        HAL_ADC_IRQHandler( &hadc );

        return( end_of_seq );
}


/**
  * @brief  
  * @param  None
  * @retval None
  */
void
bsp_oxgn_adc_dma_isr( void )
{
        HAL_DMA_IRQHandler( hadc.DMA_Handle );
}


/*******************************************************************************
* OXGN OFST
*******************************************************************************/
/**
  * @brief  
  * @param  None
  * @retval None
  */
void
bsp_oxgn_ofst_init(                     const   size_t          samplerate_sps )
{
}


/**
  * @brief  
  * @param  None
  * @retval None
  */
void
bsp_oxgn_ofst_run( void )
{
}


/**
  * @brief  
  * @param  None
  * @retval None
  */
void
bsp_oxgn_ofst_set(                              const   uint16_t        raw16 )
{
}


/**
  * @brief  
  * @param  None
  * @retval None
  */
uint16_t
bsp_oxgn_ofst_get( void )
{
        return( 0 );
}
