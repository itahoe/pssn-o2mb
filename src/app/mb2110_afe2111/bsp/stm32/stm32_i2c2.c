/**
  * @file    stm32_i2c2.c
  * @brief   I2C2 routines
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include "stm32.h"


/*******************************************************************************
* PRIVATE VARIABLES
*******************************************************************************/
static  uint16_t        const   pin_scl     = GPIO_PIN_10;
static  uint16_t        const   pin_sda     = GPIO_PIN_11;

static  I2C_HandleTypeDef       hi2c2;
static  DMA_HandleTypeDef       hi2c2_dma_tx;
static  DMA_HandleTypeDef       hi2c2_dma_rx;


/*******************************************************************************
* PRIVATE FUNCTIONS
*******************************************************************************/
static
void
init_io( void )
{
        GPIO_InitTypeDef        pins    = {     .Pin            =    pin_scl | pin_sda,
	                                        .Mode           =    GPIO_MODE_AF_OD,
	                                        .Pull           =    GPIO_PULLUP,
	                                        .Speed          =    GPIO_SPEED_FREQ_HIGH,
	                                        .Alternate      =    GPIO_AF1_I2C2 };

        HAL_GPIO_Init( GPIOB, &pins );
}


static
int
init_i2c( void )
{
        hi2c2.Instance                  = I2C2;
        //hi2c2.Init.Timing                = 0x10950C1C;
        hi2c2.Init.Timing               = 0x20303E5D; //100kHz @ 48MHz sysclock
        //hi2c2.Init.Timing               = 0xFFFFFFFF;
        hi2c2.Init.OwnAddress1          = 0;
        hi2c2.Init.AddressingMode       = I2C_ADDRESSINGMODE_7BIT;
        hi2c2.Init.DualAddressMode      = I2C_DUALADDRESS_DISABLE;
        hi2c2.Init.OwnAddress2          = 0;
        hi2c2.Init.OwnAddress2Masks     = I2C_OA2_NOMASK;
        hi2c2.Init.GeneralCallMode      = I2C_GENERALCALL_DISABLE;
        hi2c2.Init.NoStretchMode        = I2C_NOSTRETCH_DISABLE;

        if( HAL_OK != HAL_I2C_Init( &hi2c2 ) )
        {
                return( -1 );
        }

        if( HAL_OK != HAL_I2CEx_ConfigAnalogFilter( &hi2c2, I2C_ANALOGFILTER_ENABLE ) )
        {
                return( -1 );
        }

        if( HAL_OK != HAL_I2CEx_ConfigDigitalFilter( &hi2c2, 0 ) )
        {
                return( -1 );
        }

        return( 0 );
}


/*******************************************************************************
* PUBLIC FUNCTIONS
*******************************************************************************/
int
stm32_i2c2_init( void )
{
        init_i2c();
        init_io();

        HAL_NVIC_SetPriority( I2C2_IRQn, 0, 0 );
        HAL_NVIC_EnableIRQ( I2C2_IRQn );

        return( 0 );
}


int
stm32_i2c2_config_xfer_speed_hz(        const   size_t          speed_hz )
{
        return( 0 );
}


/*******************************************************************************
* WRITE
*******************************************************************************/
int
stm32_i2c2_write(                       const   uint8_t         dev_addr,
                                        const   uint16_t        reg_addr,
                                                uint8_t *       data,
                                        const   uint16_t        size )
{
        HAL_StatusTypeDef       sts;
        uint32_t                timeout = 1000;


        sts     = HAL_I2C_Mem_Write(    &hi2c2,
                                        dev_addr,
                                        reg_addr,
                                        I2C_MEMADD_SIZE_8BIT,
                                        data,
                                        size,
                                        timeout ); 

        if( sts != HAL_OK )
        {
                return( -1 );
        }

        return( 0 );
}


int
stm32_i2c2_read(                        const   uint8_t         dev_addr,
                                        const   uint16_t        reg_addr,
                                                uint8_t *       data,
                                        const   uint16_t        size )
{
        HAL_StatusTypeDef       sts;
        uint32_t                timeout = 1000;


        for( int i = 0; i < size; i++ )
        {

                sts     = HAL_I2C_Mem_Read(     &hi2c2,
                                                dev_addr,
                                                reg_addr + i,
                                                I2C_MEMADD_SIZE_8BIT,
                                                data + i,
                                                //size,
                                                1,
                                                timeout );

                if( sts != HAL_OK )
                {
                        return( -1 );
                }
        }


        return( 0 );
}


/*******************************************************************************
* INTERRUPT
*******************************************************************************/
void
stm32_i2c2_isr( void )
{
        if( hi2c2.Instance->ISR & (I2C_FLAG_BERR | I2C_FLAG_ARLO | I2C_FLAG_OVR) )
        {
                HAL_I2C_ER_IRQHandler( &hi2c2 );
        }
        else
        {
                HAL_I2C_EV_IRQHandler( &hi2c2 );
        }
}
