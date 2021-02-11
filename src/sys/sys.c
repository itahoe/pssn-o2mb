/**
  * @file    sys.c
  * @brief   System  related
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include "sys.h"
#include "bsp.h"


/**
  * @brief  
  * @param  None
  * @retval None
  */
void
sys_reset( void )
{
        bsp_sys_reset();
}


/**
  * @brief  
  * @param  None
  * @retval None
  */
uint16_t
sys_device_id(                                  const   size_t          idx )
{
        return( bsp_sys_device_id( idx ) );
}


/**
  * @brief  
  * @param  None
  * @retval None
  */
/*
uint16_t
sys_hardware_id(                                const   size_t          idx )
{
        return( bsp_sys_hardware_id( idx ) );
}
*/

/**
  * @brief  
  * @param  None
  * @retval None
  */
uint16_t
sys_firmware_id(                                const   size_t          idx )
{
        return( bsp_sys_firmware_id( idx ) );
}


/**
  * @brief  
  * @param  None
  * @retval None
  */
uint16_t
sys_serial_num(                                 const   size_t          idx )
{
        return( bsp_sys_serial_num( idx ) );
}
