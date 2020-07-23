/**
  * @file    app_error.h
  * @brief   App - Error
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#ifndef APP_ERROR_H
#define APP_ERROR_H


#include <stdio.h>
#include <stdint.h>


static
void
app_error_handler( void )
{
        #ifdef  NDEBUG
        NVIC_SystemReset();
        #else
        __asm volatile ("bkpt #0\n");   // Break into the debugger
        #endif //NDEBUG
}


#endif	//APP_ERROR_H
