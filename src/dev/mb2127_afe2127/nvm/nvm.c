/**
  * @file    nvm.c
  * @brief   Non-Volatile Memory
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include "stm32.h"
#include "nvm.h"


/*******************************************************************************
*
*******************************************************************************/
/**
  * @brief  
  * @param  None
  * @retval None
  */
void
nvm_init( void )
{
        HAL_FLASH_Unlock();

        stm32_nvm_init();

        HAL_FLASH_Lock();
}


/**
  * @brief  
  * @param  None
  * @retval None
  */
void
nvm_read16_(                                     nvm_addr_t  addr,
                                                uint16_t *      data,
                                        const   size_t          len )
{
        for( size_t i = 0; i < len; i++ )
        {
                stm32_nvm_read16( addr + i, data + i );
        }
}


/**
  * @brief  
  * @param  None
  * @retval None
  */
void
nvm_write16_(                            nvm_addr_t      addr,
                                        uint16_t *      const   data,
                                const   size_t          len )
{
    //printf( "addr: %d, len: %d\n", addr, len );

        HAL_FLASH_Unlock();

        for( size_t i = 0; i < len; i++ )
        {
                stm32_nvm_write16( addr + i, *(data + i) );

                //printf( "%04X ", *(data + i) );
        }
        //printf( "\n" );

        HAL_FLASH_Lock();
}
