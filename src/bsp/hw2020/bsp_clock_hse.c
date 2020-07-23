/**
  * @file    bsp_clock_hse.c
  * @brief   Configure MCU clock tree
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include "app.h"
#include "bsp.h"


/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 48000000
  *            HCLK(Hz)                       = 48000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 1
  *            HSE Frequency(Hz)              = 8000000
  *            PLLMUL                         = 6
  *            Flash Latency(WS)              = 1
  * @param  None
  * @retval None
  */
void
bsp_clock_config( void )
{
        RCC_OscInitTypeDef              osc     = {0};
        RCC_ClkInitTypeDef              clk     = {0};
        RCC_PeriphCLKInitTypeDef        pck     = {0};


        __HAL_RCC_SYSCFG_CLK_ENABLE();
        __HAL_RCC_PWR_CLK_ENABLE();
        __HAL_RCC_GPIOF_CLK_ENABLE();

        osc.OscillatorType              = RCC_OSCILLATORTYPE_HSI14 | RCC_OSCILLATORTYPE_HSE;
        osc.HSEState                    = RCC_HSE_ON;
        osc.HSI14State                  = RCC_HSI14_ON;
        osc.HSI14CalibrationValue       = 16;
        osc.PLL.PLLState                = RCC_PLL_ON;
        osc.PLL.PLLSource               = RCC_PLLSOURCE_HSE;
        osc.PLL.PLLMUL                  = RCC_PLL_MUL3;
        osc.PLL.PREDIV                  = RCC_PREDIV_DIV1;
        if( HAL_RCC_OscConfig( &osc ) != HAL_OK )
        {
                app_error_handler();
        }

        clk.ClockType                   = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1;
        clk.SYSCLKSource                = RCC_SYSCLKSOURCE_PLLCLK;
        clk.AHBCLKDivider               = RCC_SYSCLK_DIV1;
        clk.APB1CLKDivider              = RCC_HCLK_DIV1;
        if( HAL_RCC_ClockConfig( &clk, FLASH_LATENCY_1 ) != HAL_OK )
        {
                app_error_handler();
        }

        pck.PeriphClockSelection        = RCC_PERIPHCLK_USART1;
        pck.Usart1ClockSelection        = RCC_USART1CLKSOURCE_PCLK1;
        if( HAL_RCCEx_PeriphCLKConfig( &pck ) != HAL_OK )
        {
                app_error_handler();
        }
}
