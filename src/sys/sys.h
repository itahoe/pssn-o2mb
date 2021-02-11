#ifndef SYS_H
#define SYS_H


#include <stdint.h>
#include <string.h>
#include "app.h"


typedef enum    sys_nvm_addr_e
{
        SYS_NVM_ADDR_STARTS_COUNT       = 0,
        SYS_NVM_ADDR_SENS_OFST_RAW,
        SYS_NVM_ADDR_02,
        SYS_NVM_ADDR_03,
        SYS_NVM_ADDR_04,
        SYS_NVM_ADDR_05,
        SYS_NVM_ADDR_06,
        SYS_NVM_ADDR_07,

        SYS_NVM_ADDR_TRIM_P0_TIMESTMP_HI,
        SYS_NVM_ADDR_TRIM_P0_TIMESTMP_LO,
        SYS_NVM_ADDR_TRIM_P0_OXGN_PPM_HI,
        SYS_NVM_ADDR_TRIM_P0_OXGN_PPM_LO,
        SYS_NVM_ADDR_TRIM_P0_OXGN_RAW_HI,
        SYS_NVM_ADDR_TRIM_P0_OXGN_RAW_LO,
        //SYS_NVM_ADDR_TRIM_P0_TEMP_RAW_HI,
        //SYS_NVM_ADDR_TRIM_P0_TEMP_RAW_LO,
        SYS_NVM_ADDR_TRIM_P0_TEMP_DIGC_HI,
        SYS_NVM_ADDR_TRIM_P0_TEMP_DIGC_LO,
        SYS_NVM_ADDR_TRIM_P0_PRES_RAW_HI,
        SYS_NVM_ADDR_TRIM_P0_PRES_RAW_LO,
        SYS_NVM_ADDR_18,
        SYS_NVM_ADDR_19,

        SYS_NVM_ADDR_TRIM_P1_TIMESTMP_HI,
        SYS_NVM_ADDR_TRIM_P1_TIMESTMP_LO,
        SYS_NVM_ADDR_TRIM_P1_OXGN_PPM_HI,
        SYS_NVM_ADDR_TRIM_P1_OXGN_PPM_LO,
        SYS_NVM_ADDR_TRIM_P1_OXGN_RAW_HI,
        SYS_NVM_ADDR_TRIM_P1_OXGN_RAW_LO,
        //SYS_NVM_ADDR_TRIM_P1_TEMP_RAW_HI,
        //SYS_NVM_ADDR_TRIM_P1_TEMP_RAW_LO,
        SYS_NVM_ADDR_TRIM_P1_TEMP_DIGC_HI,
        SYS_NVM_ADDR_TRIM_P1_TEMP_DIGC_LO,
        SYS_NVM_ADDR_TRIM_P1_PRES_RAW_HI,
        SYS_NVM_ADDR_TRIM_P1_PRES_RAW_LO,
        SYS_NVM_ADDR_30,
        SYS_NVM_ADDR_31,

        SYS_NVM_ADDR_MAX,
} sys_nvm_addr_t;


/*******************************************************************************
* SYS
*******************************************************************************/
void
sys_reset( void );

uint16_t
sys_device_id(                                  const   size_t          idx );

uint16_t
sys_hardware_id(                                const   size_t          idx );

uint16_t
sys_firmware_id(                                const   size_t          idx );

uint16_t
sys_serial_num(                                 const   size_t          idx );


/*******************************************************************************
* SYS CLOCK
*******************************************************************************/
void
sys_clock_config( void );


/*******************************************************************************
* SYS NVM
*******************************************************************************/
void
sys_nvm_init( void );


void
sys_nvm_read16(                                 sys_nvm_addr_t  addr,
                                                uint16_t *      data,
                                        const   size_t          len );


void
sys_nvm_write16(                                sys_nvm_addr_t  addr,
                                                uint16_t *      data,
                                        const   size_t          len );


/*******************************************************************************
* SYS SERIAL PORT
*******************************************************************************/
/*
void
sys_ser1_init(                          const   size_t          baudrate );


void
sys_ser1_recv(                                  uint8_t *       data,
                                        const   size_t          len );


void
sys_ser1_xmit(                                  uint8_t *       data,
                                        const   size_t          len );
*/

#endif //SYS_H
