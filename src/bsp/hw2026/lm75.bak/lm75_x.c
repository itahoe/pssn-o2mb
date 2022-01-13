/**
  * @file    lm75_x.c
  * @brief   LM75 Temperature Sensor Hardware Routins
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include "app.h"
#include "bsp.h"
#include "lm75.h"


static  uint16_t        const   pin_scl     = GPIO_PIN_10;
static  uint16_t        const   pin_sda     = GPIO_PIN_11;

static  I2C_HandleTypeDef       hi2c;
static  DMA_HandleTypeDef       hdma_tx;
static  DMA_HandleTypeDef       hdma_rx;


static
void
lm75_x_init_io( void )
{
        GPIO_InitTypeDef        i2c_pins    =   {    .Pin       =    pin_scl | pin_sda,
	                                             .Mode      =    GPIO_MODE_AF_OD,
	                                             .Pull      =    GPIO_PULLUP,
	                                             .Speed     =    GPIO_SPEED_FREQ_LOW,
	                                             .Alternate =    GPIO_AF1_I2C2 };


        __HAL_RCC_GPIOB_CLK_ENABLE();
        HAL_GPIO_Init( GPIOB, &i2c_pins );
}


static
void
lm75_x_init_i2c( void )
{
        __HAL_RCC_I2C2_CLK_ENABLE();

        hi2c.Instance                   = I2C2;
        hi2c.Init.Timing                = 0x10950C1C;
        hi2c.Init.OwnAddress1           = 0;
        hi2c.Init.AddressingMode        = I2C_ADDRESSINGMODE_7BIT;
        hi2c.Init.DualAddressMode       = I2C_DUALADDRESS_DISABLE;
        hi2c.Init.OwnAddress2           = 0;
        hi2c.Init.OwnAddress2Masks      = I2C_OA2_NOMASK;
        hi2c.Init.GeneralCallMode       = I2C_GENERALCALL_DISABLE;
        hi2c.Init.NoStretchMode         = I2C_NOSTRETCH_DISABLE;
        if( HAL_OK != HAL_I2C_Init( &hi2c ) )
        {
                app_error_handler();
        }

        if( HAL_OK != HAL_I2CEx_ConfigAnalogFilter( &hi2c, I2C_ANALOGFILTER_ENABLE ) )
        {
                app_error_handler();
        }

        if( HAL_OK != HAL_I2CEx_ConfigDigitalFilter( &hi2c, 0 ) )
        {
                app_error_handler();
        }
}


/*
static
void    bsp_sens_init_timebase( void )
{
        TIM_MasterConfigTypeDef cfg;


        __HAL_RCC_TIM3_FORCE_RESET();
        __HAL_RCC_TIM3_RELEASE_RESET();
        __HAL_RCC_TIM3_CLK_ENABLE();

        htim.Instance                   = TIM3;
        htim.Init.Period                = 3600;
        //htim.Init.Prescaler             = 0;
        htim.Init.Prescaler             = 16000;
        htim.Init.ClockDivision         = TIM_CLOCKDIVISION_DIV1;
        htim.Init.CounterMode           = TIM_COUNTERMODE_UP;
        htim.Init.RepetitionCounter     = 0x0;
        htim.Init.AutoReloadPreload     = TIM_AUTORELOAD_PRELOAD_DISABLE;

        if( HAL_TIM_Base_Init( &htim ) != HAL_OK )
        {
                app_error( __FILE__, __LINE__ );
        }

        cfg.MasterOutputTrigger         = TIM_TRGO_UPDATE;
        cfg.MasterSlaveMode             = TIM_MASTERSLAVEMODE_DISABLE;

        if( HAL_TIMEx_MasterConfigSynchronization( &htim, &cfg ) != HAL_OK )
        {
                app_error( __FILE__, __LINE__ );
        }
}
*/

/*
static
void    bsp_lm75_init_dma( void )
{
        hdma_rx.Instance                        = DMA1_Channel5;
        //hdma_rx.Init.Request                    = DMA_REQUEST_6;
        hdma_rx.Init.Direction                  = DMA_PERIPH_TO_MEMORY;
        hdma_rx.Init.PeriphInc                  = DMA_PINC_DISABLE;
        hdma_rx.Init.MemInc                     = DMA_MINC_ENABLE;
        hdma_rx.Init.PeriphDataAlignment        = DMA_PDATAALIGN_BYTE;
        hdma_rx.Init.MemDataAlignment           = DMA_MDATAALIGN_BYTE;
        hdma_rx.Init.Mode                       = DMA_NORMAL;
        hdma_rx.Init.Priority                   = DMA_PRIORITY_LOW;
        if( HAL_OK != HAL_DMA_Init( &hdma_rx ) )
        {
                app_error( __FILE__, __LINE__ );
        }
        __HAL_LINKDMA( hi2c, hdmarx, hdma_rx );


        hdma_tx.Instance                        = DMA1_Channel4;
        hdma_tx.Init.Request                    = DMA_REQUEST_6;
        hdma_tx.Init.Direction                  = DMA_MEMORY_TO_PERIPH;
        hdma_tx.Init.PeriphInc                  = DMA_PINC_DISABLE;
        hdma_tx.Init.MemInc                     = DMA_MINC_ENABLE;
        hdma_tx.Init.PeriphDataAlignment        = DMA_PDATAALIGN_BYTE;
        hdma_tx.Init.MemDataAlignment           = DMA_MDATAALIGN_BYTE;
        hdma_tx.Init.Mode                       = DMA_NORMAL;
        hdma_tx.Init.Priority                   = DMA_PRIORITY_LOW;
        if( HAL_DMA_Init( &hdma_tx ) != HAL_OK )
        {
                app_error( __FILE__, __LINE__ );
        }

        __HAL_LINKDMA( hi2c, hdmatx, hdma_tx );
}
*/


void
lm75_x_init( void )
{
        lm75_x_init_i2c();
        //bsp_lm75_init_dma();
        lm75_x_init_io();
        //bsp_lm75_init_timebase();

        //HAL_NVIC_SetPriority(DMA1_Channel4_5_IRQn, 0, 0);
        //HAL_NVIC_EnableIRQ(DMA1_Channel4_5_IRQn);
}


void
lm75_x_write(                           const   uint8_t         addr,
                                        const   uint8_t         reg,
                                                uint8_t         value )
{
        HAL_StatusTypeDef       sts     = HAL_OK;

        sts     = HAL_I2C_Mem_Write(    &hi2c, addr, (uint16_t) reg, I2C_MEMADD_SIZE_8BIT, &value, 1, 100 ); 

        if( sts != HAL_OK )
        {
                app_error_handler();
        }
}


uint8_t
lm75_x_read(                            const   uint8_t         addr,
                                        const   uint8_t         reg )
{
        HAL_StatusTypeDef       sts     = HAL_OK;
        uint8_t                 value   = 0;
  

        sts     = HAL_I2C_Mem_Read( &hi2c, addr, reg, I2C_MEMADD_SIZE_8BIT, &value, 1, 1000);
  
        if( sts != HAL_OK )
        {
                //app_error_handler();
        }

        return value;   
}
