/**
  * @file    bsp_eeprom.h
  * @brief   Board Support Package - EEPROM Emulation
  * @author  Igor T. <research.tahoe@gmail.com>
  */


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef BSP_EEPROM_H
#define BSP_EEPROM_H


/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx.h"


/*******************************************************************************
* SPL
*******************************************************************************/
#define FLASH_ER_PRG_TIMEOUT         ((uint32_t)0x000B0000)


/*******************************************************************************
*
*******************************************************************************/
/* Define the size of the sectors to be used */
#define PAGE_SIZE             ((uint32_t)0x0400)  /* Page size = 1KByte */

/* EEPROM start address in Flash */
//#define EEPROM_START_ADDRESS  ((uint32_t)0x08002000)
#define EEPROM_START_ADDRESS  ((uint32_t)0x0800F800)
                                                        /* EEPROM emulation start address:
                                                        from sector2, after 8KByte of used 
                                                        Flash memory */

/* Pages 0 and 1 base and end addresses */
#define PAGE0_BASE_ADDRESS    ((uint32_t)(EEPROM_START_ADDRESS + 0x0000))
#define PAGE0_END_ADDRESS     ((uint32_t)(EEPROM_START_ADDRESS + (PAGE_SIZE - 1)))

#define PAGE1_BASE_ADDRESS    ((uint32_t)(EEPROM_START_ADDRESS + 0x0400))
#define PAGE1_END_ADDRESS     ((uint32_t)(EEPROM_START_ADDRESS + (2 * PAGE_SIZE - 1)))

/* Used Flash pages for EEPROM emulation */
#define PAGE0                 ((uint16_t)0x0000)
#define PAGE1                 ((uint16_t)0x0001)

#define NO_VALID_PAGE         ((uint16_t)0x00AB)        //No valid page define

/* Page status definitions */
#define ERASED                ((uint16_t)0xFFFF)     /* Page is empty */
#define RECEIVE_DATA          ((uint16_t)0xEEEE)     /* Page is marked to receive data */
#define VALID_PAGE            ((uint16_t)0x0000)     /* Page containing valid data */

/* Valid pages in read and write defines */
#define READ_FROM_VALID_PAGE  ((uint8_t)0x00)
#define WRITE_IN_VALID_PAGE   ((uint8_t)0x01)

#define PAGE_FULL             ((uint8_t)0x80)   //Page full define

#define NB_OF_VAR             ((uint8_t)0x10)   //Variables' number




#endif // BSP_EEPROM_H
