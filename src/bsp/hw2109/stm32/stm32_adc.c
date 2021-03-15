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
static  ADC_HandleTypeDef       hadc;
static  DMA_HandleTypeDef       hdma;


/*******************************************************************************
* PRIVATE FUNCTIONS
*******************************************************************************/
static
void
init_io( void )
{
}


static
void
init_adc( void )
{
        ADC_ChannelConfTypeDef  cfg     = {0};


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
        hadc.Init.ExternalTrigConv              = ADC_SOFTWARE_START;
        hadc.Init.ExternalTrigConvEdge          = ADC_EXTERNALTRIGCONVEDGE_NONE;
        hadc.Init.DMAContinuousRequests         = ENABLE;
        //hadc.Init.Overrun                       = ADC_OVR_DATA_OVERWRITTEN;
        hadc.Init.Overrun                       = ADC_OVR_DATA_PRESERVED;

        HAL_ADC_Init( &hadc );

        cfg.SamplingTime                        = ADC_SAMPLETIME_239CYCLES_5;
        cfg.Rank                                = ADC_RANK_CHANNEL_NUMBER;

        //cfg.Channel                             = ADC_CHANNEL_4;
        //HAL_ADC_ConfigChannel( &hadc, &cfg );

        cfg.Channel                             = ADC_CHANNEL_VREFINT;
        HAL_ADC_ConfigChannel( &hadc, &cfg );

        cfg.Channel                             = ADC_CHANNEL_TEMPSENSOR;
        HAL_ADC_ConfigChannel( &hadc, &cfg );
}


static
void
init_dma( void )
{
        hdma.Instance                   = DMA1_Channel1;
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
/**
  * @brief  
  * @param  None
  * @retval None
  */
void
stm32_adc_init( void )
{
        init_adc();
        init_dma();
        init_io();

        NVIC_SetPriority(       ADC1_IRQn,              STM32_NVIC_PRIO_ADC     );
        NVIC_EnableIRQ(         ADC1_IRQn                                       );

        NVIC_SetPriority(       DMA1_Channel1_IRQn,     STM32_NVIC_PRIO_ADC     );
        NVIC_EnableIRQ(         DMA1_Channel1_IRQn                              );  
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


/*******************************************************************************
* INTERRUPT SERVICE ROUTINES
*******************************************************************************/
/**
  * @brief  
  * @param  None
  * @retval None
  */
int
stm32_adc_isr_is_eos( void )
{
        int     end_of_seq      = 0;

        if( __HAL_ADC_GET_FLAG( &hadc, ADC_FLAG_EOS ) )
        {
                __HAL_ADC_CLEAR_FLAG( &hadc, ADC_FLAG_EOS );

                end_of_seq      = -1;
        }

        return( end_of_seq );
}


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
