/**
  * @file    stm32.h
  * @brief   Board Support Package
  * @author  Igor T. <research.tahoe@gmail.com>
  */

#ifndef  STM32_H
#define  STM32_H


#include <stdbool.h>
#include "stm32f0xx_hal.h"


/*******************************************************************************
*
*******************************************************************************/
#define STM32_SYSTICK_HZ                        1000ul


#define STM32_NVIC_PRIO_GENERIC                 13

#define STM32_NVIC_PRIO_SENS                    5

#define STM32_NVIC_PRIO_USART1                  STM32_NVIC_PRIO_GENERIC
#define STM32_NVIC_PRIO_USART1_DMA_RX           STM32_NVIC_PRIO_GENERIC
#define STM32_NVIC_PRIO_USART1_DMA_TX           STM32_NVIC_PRIO_GENERIC

//#define STM32_NVIC_PRIO_USART2                  STM32_NVIC_PRIO_GENERIC
//#define STM32_NVIC_PRIO_USART2_DMA_RX           STM32_NVIC_PRIO_GENERIC
//#define STM32_NVIC_PRIO_USART2_DMA_TX           STM32_NVIC_PRIO_GENERIC

#define STM32_NVIC_PRIO_UART4                   5
#define STM32_NVIC_PRIO_UART4_DMA_RX            5
#define STM32_NVIC_PRIO_UART4_DMA_TX            5

#define STM32_NVIC_PRIO_UART7                   5
#define STM32_NVIC_PRIO_UART7_DMA_RX            5
#define STM32_NVIC_PRIO_UART7_DMA_TX            5


/*******************************************************************************
* HAL TIMEBASE
*******************************************************************************/
void    stm32fxx_hal_timebase_tim_isr( void );

/*******************************************************************************
* ADC
*******************************************************************************/
void    stm32_adc_init( void );
void    stm32_adc_config_sps( const size_t );
void    stm32_adc_start( uint16_t *, const size_t );

int32_t stm32_adc_raw_to_vref_mV( const int32_t );
int32_t stm32_adc_raw_to_celsius( const int32_t, const int32_t );

void    stm32_adc_isr( void );
void    stm32_adc_dma_isr( void );
//void    stm32_adc_timebase_isr( void );

/*******************************************************************************
* CLOCK
*******************************************************************************/
int     stm32_clock_init( void );


/*******************************************************************************
* CORE
*******************************************************************************/

/*******************************************************************************
* DEVICE UID
*******************************************************************************/
int     stm32_device_uid( uint32_t *, size_t );
int     stm32_device_uid_str( char *, size_t );

/*******************************************************************************
* FMC
*******************************************************************************/
int     stm32_fmc_init( void );
void    stm32_fmc_isr( void );

/*******************************************************************************
* LED
*******************************************************************************/
void    stm32_led_sts_init( void );
bool    stm32_led_sts_get( void );
void    stm32_led_sts_set( const bool );
void    stm32_led_sts_hook( void );
void    stm32_led_sts_flash( const size_t );


/*******************************************************************************
* MPU
*******************************************************************************/
void    stm32_mpu_disable( void );
void    stm32_mpu_enable( void );
void    stm32_mpu_init( void );

/*******************************************************************************
* NVM (NON-VOLATILE MEMORY)
*******************************************************************************/
uint16_t stm32_nvm_init( void );
uint16_t stm32_nvm_read16( uint16_t VirtAddress, uint16_t * Data );
uint16_t stm32_nvm_write16( uint16_t VirtAddress, uint16_t Data );

/*******************************************************************************
* RESET
*******************************************************************************/
void    stm32_reset( void );


/*******************************************************************************
* TIM5
*******************************************************************************/
int     stm32_tim5_init( void );
int     stm32_tim5_pwm_start( void );
int     stm32_tim5_pwm_stop( void );


/*******************************************************************************
* TIM16
*******************************************************************************/
void    stm32_tim16_init( void );
int     stm32_tim16_run( void );
void    stm32_tim16_set( const uint16_t );
uint16_t stm32_tim16_get( void );


/*******************************************************************************
* USART1
*******************************************************************************/
int     stm32_usart1_deinit( void );
int     stm32_usart1_init( void );
void    stm32_usart1_config_baudrate( const size_t );
int     stm32_usart1_recv_dma( uint8_t *, size_t );
int     stm32_usart1_xmit_dma( uint8_t *, size_t );

void    stm32_usart1_isr( void );
int     stm32_usart1_isr_is_idle( void );

uint32_t stm32_usart1_dma_recv_get_ndtr( void );
uint32_t stm32_usart1_dma_xmit_get_ndtr( void );
void    stm32_usart1_dma_isr( void );


/*******************************************************************************
* USART2
*******************************************************************************/
/*
void    stm32_usart2_deinit( void );
int     stm32_usart2_init( void );
int     stm32_usart2_config_baudrate( const size_t );
int     stm32_usart2_recv_dma( uint8_t *, size_t );
int     stm32_usart2_xmit_dma( const uint8_t *, size_t );

int     stm32_usart2_isr( void );

uint32_t stm32_usart2_dma_rx_get_ndtr( void );
void    stm32_usart2_dma_rx_isr( void );

uint32_t stm32_usart2_dma_tx_get_ndtr( void );
void    stm32_usart2_dma_tx_isr( void );
*/

/*******************************************************************************
* UART4
*******************************************************************************/
int     stm32_uart4_deinit( void );
int     stm32_uart4_init( void );
void    stm32_uart4_config_baudrate( const size_t );
int     stm32_uart4_recv( uint8_t *, size_t );
int     stm32_uart4_xmit( uint8_t *, size_t );
void    stm32_uart4_idle_clear( void );
void    stm32_uart4_isr( void );
int     stm32_uart4_isr_is_idle( void );

uint32_t stm32_uart4_dma_rx_get_ndtr( void );
void    stm32_uart4_dma_rx_isr( void );

uint32_t stm32_uart4_dma_tx_get_ndtr( void );
void    stm32_uart4_dma_tx_isr( void );



/*******************************************************************************
* UART7
*******************************************************************************/
void    stm32_uart7_deinit( void );
int     stm32_uart7_init( void );
void    stm32_uart7_config_baudrate( const size_t );
int     stm32_uart7_recv_dma( uint8_t *, size_t );
int     stm32_uart7_xmit_dma( const uint8_t *, size_t );

int     stm32_uart7_isr( void );

uint32_t stm32_uart7_dma_rx_get_ndtr( void );
void    stm32_uart7_dma_rx_isr( void );

uint32_t stm32_uart7_dma_tx_get_ndtr( void );
void    stm32_uart7_dma_tx_isr( void );


#endif	//STM32_H
