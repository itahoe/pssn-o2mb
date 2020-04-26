/**
  * @file    sys_ser.c
  * @brief   System Serial Port related
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include "sys.h"
#include "bsp.h"


/*******************************************************************************
* SERIAL
*******************************************************************************/
/**
  * @brief  
  * @param  None
  * @retval None
  */
void
sys_ser1_init(                          const   size_t          baudrate )
{
        bsp_ser1_init( baudrate );
}


/**
  * @brief  
  * @param  None
  * @retval None
  */
void
sys_ser1_recv(                                  uint8_t *       data,
                                        const   size_t          len )
{
        bsp_ser1_recv_start( data, len );
}


/**
  * @brief  
  * @param  None
  * @retval None
  */
void
sys_ser1_xmit(                                  uint8_t *       data,
                                        const   size_t          len )
{
        bsp_ser1_xmit( data, len );
}
