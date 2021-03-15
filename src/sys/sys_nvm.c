/**
  * @file    sys_nvm.c
  * @brief   Non-Volatile Memory
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include "sys.h"
#include "bsp.h"


/*******************************************************************************
*
*******************************************************************************/
/**
  * @brief  
  * @param  None
  * @retval None
  */
void
sys_nvm_init( void )
{
        HAL_FLASH_Unlock();

        bsp_nvm_init();

        HAL_FLASH_Lock();
}


/**
  * @brief  
  * @param  None
  * @retval None
  */
void
sys_nvm_read16(                                 sys_nvm_addr_t  addr,
                                                uint16_t *      data,
                                        const   size_t          len )
{
        for( size_t i = 0; i < len; i++ )
        {
                bsp_nvm_read16( addr + i, data + i );
        }
}


/**
  * @brief  
  * @param  None
  * @retval None
  */
void
sys_nvm_write16(                                sys_nvm_addr_t  addr,
                                                uint16_t *      data,
                                        const   size_t          len )
{
        HAL_FLASH_Unlock();

        for( size_t i = 0; i < len; i++ )
        {
                bsp_nvm_write16( addr + i, *(data + i) );
        }

        HAL_FLASH_Lock();
}
