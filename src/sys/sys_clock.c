/**
  * @file    sys_clock.c
  * @brief   System Clock related
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include "sys.h"
#include "bsp.h"


/*******************************************************************************
* CLOCK
*******************************************************************************/
/**
  * @brief  
  * @param  None
  * @retval None
  */
void
sys_clock_config( void )
{
        bsp_clock_config();
}


