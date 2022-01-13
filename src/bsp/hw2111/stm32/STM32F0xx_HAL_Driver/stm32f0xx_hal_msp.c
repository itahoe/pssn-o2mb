/**
  * @file    stm32f0xx_hal_msp.h
  * @brief   HAL MSP module
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include "stm32f0xx_hal.h"


/*******************************************************************************
* PUBLIC FUNCTIONS
*******************************************************************************/
/**
  * @brief  Initializes the Global MSP
  * @param  None
  * @retval None
  */
void
HAL_MspInit( void )
{
        __HAL_RCC_GPIOA_CLK_ENABLE();
        __HAL_RCC_GPIOB_CLK_ENABLE();
        __HAL_RCC_GPIOC_CLK_ENABLE();
        __HAL_RCC_GPIOF_CLK_ENABLE();

        __HAL_RCC_SYSCFG_CLK_ENABLE();
        __HAL_RCC_PWR_CLK_ENABLE();
        __HAL_RCC_DMA1_CLK_ENABLE();
        __HAL_RCC_ADC1_CLK_ENABLE();
        __HAL_RCC_SPI1_CLK_ENABLE();
        //__HAL_RCC_I2C1_CLK_ENABLE();
        __HAL_RCC_I2C2_CLK_ENABLE();
        __HAL_RCC_TIM3_CLK_ENABLE();
        __HAL_RCC_TIM6_CLK_ENABLE();
}


/**
  * @brief  DeInitializes the Global MSP. 
  * @param  None
  * @retval None
  */
void
HAL_MspDeInit( void )
{

}


/**
  * @brief  Initializes the PPP MSP.
  * @param  None
  * @retval None
  */
void
HAL_PPP_MspInit( void )
{

}


/**
  * @brief  DeInitializes the PPP MSP. 
  * @param  None
  * @retval None
  */
void
HAL_PPP_MspDeInit( void )
{

}
