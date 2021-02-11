/**
  * @file       stm32_reset.h
  * @brief      Global MCU Reset
  * @author     Igor T. <research.tahoe@gmail.com>
  */


#include "stm32.h"


/*******************************************************************************
* PUBLIC FUNCTIONS
*******************************************************************************/
/**
 * @brief Restart MCU
 */
void
stm32_reset( void )
{
        #if defined( NDEBUG )
                NVIC_SystemReset();
        #else
                __asm volatile ("bkpt #0\n");   // Break into the debugger
        #endif //NDEBUG
}
