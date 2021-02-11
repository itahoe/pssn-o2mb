/**
  * @file    bsp.h
  * @brief   Board Support Package
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#ifndef  BSP_H
#define  BSP_H


#include <stdbool.h>
//#include <string.h>
#include <stdint.h>
#include "stm32f0xx_hal.h"


#define BSP_SYSTICK_HZ                  1000ul

#define BSP_NVIC_PRIO_SER1_DMA_RX       9
#define BSP_NVIC_PRIO_SER1_DMA_TX       9
#define BSP_NVIC_PRIO_SER1_UART         11
#define BSP_NVIC_PRIO_SENS              7


/*******************************************************************************
* MCU related
*******************************************************************************/
void
bsp_clock_config( void );


void
bsp_system_reset( void );


/*******************************************************************************
* OXGN
*******************************************************************************/
void
bsp_oxgn_init(                          const   size_t          samplerate_sps );


void
bsp_oxgn_run(                           const   uint16_t *      data,
                                        const   size_t          len );

void
bsp_oxgn_adc_isr( void );

void
bsp_oxgn_dma_isr( void );


/*******************************************************************************
* OXGN OFST
*******************************************************************************/
void
bsp_oxgn_ofst_init(                             const   size_t          samplerate_sps );

void
bsp_oxgn_ofst_run( void );

void
bsp_oxgn_ofst_set(                              const   uint16_t        cmp );

uint16_t
bsp_oxgn_ofst_get( void );


/*******************************************************************************
* LED
*******************************************************************************/
void
bsp_led_sts_init( void );

bool
bsp_led_sts_get( void );

void
bsp_led_sts_set(                        const   bool            color );


/*******************************************************************************
* LM75 SENS TEMPERATURE
*******************************************************************************/
void
bsp_lm75_init( void );


void
bsp_lm75_write(                         const   uint8_t         addr,
                                        const   uint8_t         reg,
                                                uint8_t         value );


uint8_t
bsp_lm75_read(                          const   uint8_t         addr,
                                        const   uint8_t         reg );


void
bsp_lm75_isr( void );


uint8_t
bsp_lm75_read_u08(                      const   uint8_t         addr,
                                        const   uint8_t         reg );


uint16_t
bsp_lm75_read_u16(                      const   uint8_t         addr,
                                        const   uint8_t         reg );


/*******************************************************************************
* NVM (NON-VOLATILE MEMORY)
*******************************************************************************/
/*
uint16_t
bsp_nvm_init( void );


uint16_t
bsp_nvm_read16(                                 uint16_t        VirtAddress,
                                                uint16_t *      Data );


uint16_t
bsp_nvm_write16(                                uint16_t        VirtAddress,
                                                uint16_t        Data );

*/
/*******************************************************************************
* SER1
*******************************************************************************/
void
bsp_ser1_init(                          const   size_t          baud );


void
bsp_ser1_recv_start(                            uint8_t *       data,
                                        const   size_t          size );


uint8_t
bsp_ser1_getc( void );


uint32_t
bsp_ser1_dma_recv_get_ndtr( void );


void
bsp_ser1_xmit(                                  uint8_t *               data,
                                        const   size_t                  size );


bool
bsp_ser1_uart_isr( void );


void
bsp_ser1_dma_isr( void );


/*******************************************************************************
* SYS
*******************************************************************************/
void
bsp_sys_reset( void );

uint16_t
bsp_sys_device_id(                              const   size_t          idx  );

uint16_t
bsp_sys_hardware_id(                            const   size_t          idx  );

uint16_t
bsp_sys_firmware_id(                            const   size_t          idx  );

uint16_t
bsp_sys_serial_num(                             const   size_t          idx );


#endif	//BSP_H
