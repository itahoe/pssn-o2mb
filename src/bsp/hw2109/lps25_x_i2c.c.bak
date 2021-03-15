/**
  * @file    bsp_lps25.c
  * @brief   Temperature Sensor Hardware Routins
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include "app.h"
#include "bsp.h"


/*******************************************************************************
*
*******************************************************************************/
static  uint16_t        const   pin_scl         = GPIO_PIN_10;
static  uint16_t        const   pin_sda         = GPIO_PIN_11;
static  I2C_HandleTypeDef       hi2c;
static  DMA_HandleTypeDef       hdma_tx;
static  DMA_HandleTypeDef       hdma_rx;


/*******************************************************************************
* FUNCTION PROTOTYPES
*******************************************************************************/
void
lps25_x_init( void );

uint8_t
lps25_x_read(                           const   uint8_t         dev_addr,
                                        const   uint8_t         reg_addr );

void
lps25_x_write(                          const   uint8_t         dev_addr,
                                        const   uint8_t         reg_addr,
                                                uint8_t         data );


/*******************************************************************************
*
*******************************************************************************/
static
void
bsp_lps25_init_io( void )
{
        GPIO_InitTypeDef        pins_i2c    =   {    .Pin       =    pin_scl | pin_sda,
	                                             .Mode      =    GPIO_MODE_AF_OD,
	                                             .Pull      =    GPIO_PULLUP,
	                                             .Speed     =    GPIO_SPEED_FREQ_LOW,
	                                             .Alternate =    GPIO_AF1_I2C2 };

        GPIO_InitTypeDef        pin_sa0     =   {    .Pin       =    GPIO_PIN_2,
	                                             .Mode      =    GPIO_MODE_OUTPUT_PP,
	                                             .Pull      =    GPIO_PULLDOWN,
	                                             .Speed     =    GPIO_SPEED_FREQ_LOW };

        GPIO_InitTypeDef        pin_cs      =   {    .Pin       =    GPIO_PIN_8,
	                                             .Mode      =    GPIO_MODE_OUTPUT_PP,
	                                             .Pull      =    GPIO_PULLUP,
	                                             .Speed     =    GPIO_SPEED_FREQ_LOW };
/*
        GPIO_InitTypeDef        pin_scl     =   {    .Pin       =    GPIO_PIN_10,
	                                             .Mode      =    GPIO_MODE_OUTPUT_PP,
	                                             .Pull      =    GPIO_PULLDOWN,
	                                             .Speed     =    GPIO_SPEED_FREQ_LOW };

        GPIO_InitTypeDef        pin_sda     =   {    .Pin       =    GPIO_PIN_11,
	                                             .Mode      =    GPIO_MODE_OUTPUT_PP,
	                                             .Pull      =    GPIO_PULLDOWN,
	                                             .Speed     =    GPIO_SPEED_FREQ_LOW };
*/


        __HAL_RCC_GPIOA_CLK_ENABLE();
        __HAL_RCC_GPIOB_CLK_ENABLE();

        HAL_GPIO_Init( GPIOB, &pins_i2c );
        HAL_GPIO_Init( GPIOA, &pin_sa0  );
        HAL_GPIO_Init( GPIOA, &pin_cs   );

        HAL_GPIO_WritePin( GPIOA, GPIO_PIN_8,  GPIO_PIN_RESET ); //pin_sa0
        HAL_GPIO_WritePin( GPIOA, GPIO_PIN_2,  GPIO_PIN_SET   ); //pin_cs


/*
        HAL_GPIO_Init( GPIOB, &pin_scl  );
        HAL_GPIO_Init( GPIOB, &pin_sda  );

        HAL_GPIO_WritePin( GPIOB, GPIO_PIN_10, GPIO_PIN_SET   ); //pin_scl
        HAL_GPIO_WritePin( GPIOB, GPIO_PIN_10, GPIO_PIN_RESET ); //pin_scl

        HAL_GPIO_WritePin( GPIOB, GPIO_PIN_11, GPIO_PIN_SET   ); //pin_sda
        HAL_GPIO_WritePin( GPIOB, GPIO_PIN_11, GPIO_PIN_RESET ); //pin_sda

        HAL_GPIO_WritePin( GPIOA, GPIO_PIN_2,  GPIO_PIN_SET   ); //pin_sa0
        HAL_GPIO_WritePin( GPIOA, GPIO_PIN_2,  GPIO_PIN_RESET ); //pin_sa0

        HAL_GPIO_WritePin( GPIOA, GPIO_PIN_8,  GPIO_PIN_SET   ); //pin_cs
        HAL_GPIO_WritePin( GPIOA, GPIO_PIN_8,  GPIO_PIN_RESET ); //pin_cs
*/
}


static
void
bsp_lps25_init_i2c( void )
{
        __HAL_RCC_I2C2_CLK_ENABLE();

        hi2c.Instance                   = I2C2;
        //hi2c.Init.Timing                = 0x10950C1C;
        hi2c.Init.Timing                = 0xFFFFFFFF;
        //hi2c.Init.Timing                = 0x00000000;
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


/*******************************************************************************
* INIT
*******************************************************************************/
void
lps25_x_init( void )
{
        bsp_lps25_init_i2c();
        //bsp_lm75_init_dma();
        bsp_lps25_init_io();
        //bsp_lm75_init_timebase();

        //HAL_NVIC_SetPriority(DMA1_Channel4_5_IRQn, 0, 0);
        //HAL_NVIC_EnableIRQ(DMA1_Channel4_5_IRQn);
}


/*******************************************************************************
* READ
*******************************************************************************/
uint8_t
lps25_x_read(                           const   uint8_t         reg_addr )
{
        HAL_StatusTypeDef       sts     = HAL_OK;
        uint8_t                 data    = 0;
  

        sts     = HAL_I2C_Mem_Read(                     &hi2c,
                                                        CFG_LPS25_I2C_ADDR,
                                                        reg_addr | 0x80,
                                                        I2C_MEMADD_SIZE_8BIT,
                                                        &data,
                                                        1,
                                                        100 );
  
        if( sts != HAL_OK )
        {
                //app_error_handler();
        }

        return( data );   
}


/**
  * @brief  
  * @param  None
  * @retval None
  */
void
lps25_x_write(                          const   uint8_t         reg_addr,
                                                uint8_t         data )
{
        HAL_StatusTypeDef       sts;


        sts     = HAL_I2C_Mem_Write(                    &hi2c,
                                                        CFG_LPS25_I2C_ADDR,
                                             (uint16_t) reg_addr,
                                                        I2C_MEMADD_SIZE_8BIT,
                                                        &data,
                                                        1,
                                                        100 ); 

        if( sts != HAL_OK )
        {
                app_error_handler();
        }
}
