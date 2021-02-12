#ifndef SYS_H
#define SYS_H


#include <stdint.h>
#include <string.h>
#include "app.h"


/*******************************************************************************
* SYS
*******************************************************************************/
/*
void
sys_reset( void );
*/

uint16_t
sys_device_id(                                  const   size_t          idx );

uint16_t
sys_hardware_id(                                const   size_t          idx );

uint16_t
sys_firmware_id(                                const   size_t          idx );

uint16_t
sys_serial_num(                                 const   size_t          idx );


#endif //SYS_H
