#ifndef NVM_H
#define NVM_H


#include <stdint.h>
#include <string.h>
#include "app.h"


typedef enum    nvm_addr_e
{
        NVM_ADDR_STARTS_COUNT           = 0,
        NVM_ADDR_AFE_BIAS,
        NVM_ADDR_AFE_ADC_REG_MODE,
        NVM_ADDR_AFE_ADC_REG_CONF,
        NVM_ADDR_AFE_K_TEMP_DRIFT_HI,
        NVM_ADDR_AFE_K_TEMP_DRIFT_LO,
        NVM_ADDR_AFE_K_PRES_DRIFT_HI,
        NVM_ADDR_AFE_K_PRES_DRIFT_LO,

        NVM_ADDR_TRIM_P0_TIMESTMP_HI,
        NVM_ADDR_TRIM_P0_TIMESTMP_LO,
        NVM_ADDR_TRIM_P0_OXGN_PPM_HI,
        NVM_ADDR_TRIM_P0_OXGN_PPM_LO,
        NVM_ADDR_TRIM_P0_OXGN_RAW_HI,
        NVM_ADDR_TRIM_P0_OXGN_RAW_LO,
        //NVM_ADDR_TRIM_P0_TEMP_RAW_HI,
        //NVM_ADDR_TRIM_P0_TEMP_RAW_LO,
        NVM_ADDR_TRIM_P0_TEMP_DIGC_HI,
        NVM_ADDR_TRIM_P0_TEMP_DIGC_LO,
        NVM_ADDR_TRIM_P0_PRES_RAW_HI,
        NVM_ADDR_TRIM_P0_PRES_RAW_LO,
        NVM_ADDR_18,
        NVM_ADDR_19,

        NVM_ADDR_TRIM_P1_TIMESTMP_HI,
        NVM_ADDR_TRIM_P1_TIMESTMP_LO,
        NVM_ADDR_TRIM_P1_OXGN_PPM_HI,
        NVM_ADDR_TRIM_P1_OXGN_PPM_LO,
        NVM_ADDR_TRIM_P1_OXGN_RAW_HI,
        NVM_ADDR_TRIM_P1_OXGN_RAW_LO,
        //NVM_ADDR_TRIM_P1_TEMP_RAW_HI,
        //NVM_ADDR_TRIM_P1_TEMP_RAW_LO,
        NVM_ADDR_TRIM_P1_TEMP_DIGC_HI,
        NVM_ADDR_TRIM_P1_TEMP_DIGC_LO,
        NVM_ADDR_TRIM_P1_PRES_RAW_HI,
        NVM_ADDR_TRIM_P1_PRES_RAW_LO,
        NVM_ADDR_30,
        NVM_ADDR_31,

        NVM_ADDR_MAX,
} nvm_addr_t;


/*******************************************************************************
* FUNCTION PROTOTYPES
*******************************************************************************/
void
nvm_init( void );


void
nvm_read16(                                     nvm_addr_t  addr,
                                                uint16_t *      data,
                                        const   size_t          len );

/*
void
nvm_write16(                                    nvm_addr_t  addr,
                                                uint16_t *      data,
                                        const   size_t          len );
*/

void
nvm_write16(                                    nvm_addr_t              addr,
                                                uint16_t *      const   data,
                                        const   size_t                  len );


#endif //NVM_H
