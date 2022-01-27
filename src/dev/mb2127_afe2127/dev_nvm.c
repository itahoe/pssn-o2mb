/**
  * @file    dev_nvm.c
  * @brief   Device Non-Volatile Memory
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include <string.h>
#include "dev.h"
#include "nvm.h"
//#include "crc.h"
#include "mdbs.h"


/**
  * @brief  
  * @param  None
  * @retval None
  */
/*******************************************************************************
* GLOBAL VARIABLES
*******************************************************************************/
        uint16_t                VirtAddVarTab[ NVM_ADDR_MAX ]  =
{
        NVM_ADDR_STARTS_COUNT,
        NVM_ADDR_AFE_BIAS,
        NVM_ADDR_AFE_ADC_REG_MODE,
        NVM_ADDR_AFE_ADC_REG_CONF,
        NVM_ADDR_AFE_K_TEMP_DRIFT_HI,
        NVM_ADDR_AFE_K_TEMP_DRIFT_LO,
        NVM_ADDR_AFE_K_PRES_DRIFT_HI,
        NVM_ADDR_AFE_K_PRES_DRIFT_LO,

        NVM_ADDR_SENS_ZERO_TIMESTMP_LO,
        NVM_ADDR_SENS_ZERO_TIMESTMP_HI,
        NVM_ADDR_SENS_ZERO_OXGN_PPM_LO,
        NVM_ADDR_SENS_ZERO_OXGN_PPM_HI,
        NVM_ADDR_SENS_ZERO_OXGN_RAW_LO,
        NVM_ADDR_SENS_ZERO_OXGN_RAW_HI,
        NVM_ADDR_SENS_ZERO_TEMP_DIGC_LO,
        NVM_ADDR_SENS_ZERO_TEMP_DIGC_HI,
        NVM_ADDR_SENS_ZERO_PRES_RAW_LO,
        NVM_ADDR_SENS_ZERO_PRES_RAW_HI,
        NVM_ADDR_SENS_SPAN_TIMESTMP_LO,
        NVM_ADDR_SENS_SPAN_TIMESTMP_HI,
        NVM_ADDR_SENS_SPAN_OXGN_PPM_LO,
        NVM_ADDR_SENS_SPAN_OXGN_PPM_HI,
        NVM_ADDR_SENS_SPAN_OXGN_RAW_LO,
        NVM_ADDR_SENS_SPAN_OXGN_RAW_HI,
        NVM_ADDR_SENS_SPAN_TEMP_DIGC_LO,
        NVM_ADDR_SENS_SPAN_TEMP_DIGC_HI,
        NVM_ADDR_SENS_SPAN_PRES_RAW_LO,
        NVM_ADDR_SENS_SPAN_PRES_RAW_HI,
        NVM_ADDR_SENS_LPF_FCUT,
        NVM_ADDR_SENS_LPF_ORDER,
        NVM_ADDR_30,
        NVM_ADDR_CHKSUM,
};



/*******************************************************************************
* PUBLIC FUNCTIONS
*******************************************************************************/
/**
  * @brief  
  * @param  None
  * @retval None
  */
void
dev_nvm_write(                                  nvm_addr_t      addr,
                                                uint16_t        data )
{
    //uint16_t        crc16;
    //uint16_t        buf[NVM_ADDR_MAX];

    //printf( "addr: %d, data:%d\n", addr, data );

    //nvm_read16_( NVM_ADDR_START, buf, NVM_ADDR_MAX );
    //buf[addr]           = data;
    //crc16               = mdbs_crc( (uint8_t *) buf, (NVM_ADDR_MAX-1)*2 );
    //buf[NVM_ADDR_CHKSUM] = crc16;

    nvm_write16_( addr, &data, 1 );
}


/**
  * @brief  
  * @param  None
  * @retval None
  */
uint16_t
dev_nvm_read(                       const       nvm_addr_t      addr )
{
    uint16_t    data;


    nvm_read16_( addr, &data, 1 );

    return( data );
}

/**
  * @brief  
  * @param  None
  * @retval None
  */
void
dev_nvm_reset(                                  dev_t *         p )
{
    uint16_t        buf[NVM_ADDR_MAX];


    buf[NVM_ADDR_STARTS_COUNT           ]   = 0;
    buf[NVM_ADDR_AFE_BIAS               ]   = 0;
    buf[NVM_ADDR_AFE_ADC_REG_MODE       ]   = 0x000A;
    buf[NVM_ADDR_AFE_ADC_REG_CONF       ]   = 0x0710 | 0x0002 | 0x1000;
    buf[NVM_ADDR_AFE_K_TEMP_DRIFT_HI    ]   = 0xFFFF;
    buf[NVM_ADDR_AFE_K_TEMP_DRIFT_LO    ]   = 0xFFFF;
    buf[NVM_ADDR_AFE_K_PRES_DRIFT_HI    ]   = 0xFFFF;
    buf[NVM_ADDR_AFE_K_PRES_DRIFT_LO    ]   = 0xFFFF;
    buf[NVM_ADDR_SENS_ZERO_TIMESTMP_HI  ]   = 0;
    buf[NVM_ADDR_SENS_ZERO_TIMESTMP_LO  ]   = 0;
    buf[NVM_ADDR_SENS_ZERO_OXGN_PPM_HI  ]   = 0;
    buf[NVM_ADDR_SENS_ZERO_OXGN_PPM_LO  ]   = 0;
    buf[NVM_ADDR_SENS_ZERO_OXGN_RAW_HI  ]   = 0;
    buf[NVM_ADDR_SENS_ZERO_OXGN_RAW_LO  ]   = 0;
    buf[NVM_ADDR_SENS_ZERO_TEMP_DIGC_HI ]   = 0;
    buf[NVM_ADDR_SENS_ZERO_TEMP_DIGC_LO ]   = 0;
    buf[NVM_ADDR_SENS_ZERO_PRES_RAW_HI  ]   = 0;
    buf[NVM_ADDR_SENS_ZERO_PRES_RAW_LO  ]   = 0;
    buf[NVM_ADDR_SENS_SPAN_TIMESTMP_HI  ]   = 0;
    buf[NVM_ADDR_SENS_SPAN_TIMESTMP_LO  ]   = 0;
    buf[NVM_ADDR_SENS_SPAN_OXGN_PPM_HI  ]   = 0;
    buf[NVM_ADDR_SENS_SPAN_OXGN_PPM_LO  ]   = 0;
    buf[NVM_ADDR_SENS_SPAN_OXGN_RAW_HI  ]   = 0;
    buf[NVM_ADDR_SENS_SPAN_OXGN_RAW_LO  ]   = 0;
    buf[NVM_ADDR_SENS_SPAN_TEMP_DIGC_HI ]   = 0;
    buf[NVM_ADDR_SENS_SPAN_TEMP_DIGC_LO ]   = 0;
    buf[NVM_ADDR_SENS_SPAN_PRES_RAW_HI  ]   = 0;
    buf[NVM_ADDR_SENS_SPAN_PRES_RAW_LO  ]   = 0;
    buf[NVM_ADDR_SENS_LPF_FCUT          ]   = 0;
    buf[NVM_ADDR_SENS_LPF_ORDER         ]   = 0;
    buf[NVM_ADDR_30                     ]   = 0xFFFF;
    buf[NVM_ADDR_CHKSUM                 ]   = 0x55AA;

    nvm_write16_( NVM_ADDR_START, buf, NVM_ADDR_MAX );
}


/**
  * @brief  
  * @param  None
  * @retval None
  */
void
dev_nvm_restore(                            dev_t *         p )
{
    uint16_t        chksum;


    //chksum  = mdbs_crc( (uint8_t *) buf, (NVM_ADDR_MAX-1)*2 );
    chksum  = dev_nvm_read( NVM_ADDR_CHKSUM );


    if( chksum != 0x55AA )
    {
        dev_nvm_reset( p );
    }

    //p->starts_cnt   = dev_nvm_read( NVM_ADDR_STARTS_COUNT );
    //p->starts_cnt++;
    //dev_nvm_write( NVM_ADDR_STARTS_COUNT,       p->starts_cnt       );

    p->afe.bias                     = dev_nvm_read( NVM_ADDR_AFE_BIAS               );
    p->ad7799.reg.mode.u16          = dev_nvm_read( NVM_ADDR_AFE_ADC_REG_MODE       );
    p->ad7799.reg.conf.u16          = dev_nvm_read( NVM_ADDR_AFE_ADC_REG_CONF       );
    p->sens.drift_k_temp.u16[ 1]    = dev_nvm_read( NVM_ADDR_AFE_K_TEMP_DRIFT_HI    );
    p->sens.drift_k_temp.u16[ 0]    = dev_nvm_read( NVM_ADDR_AFE_K_TEMP_DRIFT_LO    );
    p->sens.drift_k_pres.u16[ 1]    = dev_nvm_read( NVM_ADDR_AFE_K_PRES_DRIFT_HI    );
    p->sens.drift_k_pres.u16[ 0]    = dev_nvm_read( NVM_ADDR_AFE_K_PRES_DRIFT_LO    );
    p->sens.zero.timestmp.u16[ 1]   = dev_nvm_read( NVM_ADDR_SENS_ZERO_TIMESTMP_HI  );
    p->sens.zero.timestmp.u16[ 0]   = dev_nvm_read( NVM_ADDR_SENS_ZERO_TIMESTMP_LO  );
    p->sens.zero.ppm.u16[ 1]        = dev_nvm_read( NVM_ADDR_SENS_ZERO_OXGN_PPM_HI  );
    p->sens.zero.ppm.u16[ 0]        = dev_nvm_read( NVM_ADDR_SENS_ZERO_OXGN_PPM_LO  );
    p->sens.zero.raw.u16[ 1]        = dev_nvm_read( NVM_ADDR_SENS_ZERO_OXGN_RAW_HI  );
    p->sens.zero.raw.u16[ 0]        = dev_nvm_read( NVM_ADDR_SENS_ZERO_OXGN_RAW_LO  );
    p->sens.zero.temp_digc.u16[ 1]  = dev_nvm_read( NVM_ADDR_SENS_ZERO_TEMP_DIGC_HI );
    p->sens.zero.temp_digc.u16[ 0]  = dev_nvm_read( NVM_ADDR_SENS_ZERO_TEMP_DIGC_LO );
    p->sens.zero.pres_raw.u16[ 1]   = dev_nvm_read( NVM_ADDR_SENS_ZERO_PRES_RAW_HI  );
    p->sens.zero.pres_raw.u16[ 0]   = dev_nvm_read( NVM_ADDR_SENS_ZERO_PRES_RAW_LO  );
    p->sens.span.timestmp.u16[ 1]   = dev_nvm_read( NVM_ADDR_SENS_SPAN_TIMESTMP_HI  );
    p->sens.span.timestmp.u16[ 0]   = dev_nvm_read( NVM_ADDR_SENS_SPAN_TIMESTMP_LO  );
    p->sens.span.ppm.u16[ 1]        = dev_nvm_read( NVM_ADDR_SENS_SPAN_OXGN_PPM_HI  );
    p->sens.span.ppm.u16[ 0]        = dev_nvm_read( NVM_ADDR_SENS_SPAN_OXGN_PPM_LO  );
    p->sens.span.raw.u16[ 1]        = dev_nvm_read( NVM_ADDR_SENS_SPAN_OXGN_RAW_HI  );
    p->sens.span.raw.u16[ 0]        = dev_nvm_read( NVM_ADDR_SENS_SPAN_OXGN_RAW_LO  );
    p->sens.span.temp_digc.u16[ 1]  = dev_nvm_read( NVM_ADDR_SENS_SPAN_TEMP_DIGC_HI );
    p->sens.span.temp_digc.u16[ 0]  = dev_nvm_read( NVM_ADDR_SENS_SPAN_TEMP_DIGC_LO );
    p->sens.span.pres_raw.u16[ 1]   = dev_nvm_read( NVM_ADDR_SENS_SPAN_PRES_RAW_HI  );
    p->sens.span.pres_raw.u16[ 0]   = dev_nvm_read( NVM_ADDR_SENS_SPAN_PRES_RAW_LO  );
    p->sens.lpf.fcut                = dev_nvm_read( NVM_ADDR_SENS_LPF_FCUT          );
    p->sens.lpf.order               = dev_nvm_read( NVM_ADDR_SENS_LPF_ORDER         );
}
