/**
  * @file    bsp_nvm.c
  * @brief   Board Support Package - EEPROM Emulation
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include "stm32_nvm.h"
#include "stm32.h"


extern  uint16_t        VirtAddVarTab[ NB_OF_VAR ];     //Virtual address defined by the user: 0xFFFF value is prohibited
        uint16_t        DataVar         = 0;            //Global variable used to store variable value in read sequence


/*******************************************************************************
* SPL
*******************************************************************************/
/**
  * @brief  Programs a half word at a specified address.
  * @note   To correctly run this function, the FLASH_Unlock() function must be called before.
  * @note   Call the FLASH_Lock() to disable the flash memory access (recommended
  *         to protect the FLASH memory against possible unwanted operation)
  * @param  Address: specifies the address to be programmed.
  * @param  Data: specifies the data to be programmed.
  * @retval FLASH Status: The returned value can be: FLASH_ERROR_PG,
  *         FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT. 
  */
static
HAL_StatusTypeDef
FLASH_ProgramHalfWord(                          uint32_t        Address,
                                                uint16_t        Data )
{
        HAL_StatusTypeDef       status  = HAL_OK;


        assert_param( IS_FLASH_PROGRAM_ADDRESS(Address) );

        status  = FLASH_WaitForLastOperation( FLASH_ER_PRG_TIMEOUT );   //Wait for last operation to be completed

        if( status == HAL_OK )  //if( status == FLASH_COMPLETE )
        {
                FLASH->CR       |= FLASH_CR_PG; //If the previous operation is completed, proceed to program the new data
  
                *(__IO uint16_t *) Address = Data;

                status          = FLASH_WaitForLastOperation( FLASH_ER_PRG_TIMEOUT );   //Wait for last operation to be completed
                FLASH->CR       &= ~FLASH_CR_PG;                                //Disable the PG Bit
        }

        return( status );
}


/**
  * @brief  Erases a specified page in program memory.
  * @note   To correctly run this function, the FLASH_Unlock() function must be called before.
  * @note   Call the FLASH_Lock() to disable the flash memory access (recommended
  *         to protect the FLASH memory against possible unwanted operation)
  * @param  Page_Address: The page address in program memory to be erased.
  * @note   A Page is erased in the Program memory only if the address to load 
  *         is the start address of a page (multiple of 1024 bytes).
  * @retval FLASH Status: The returned value can be: 
  *         FLASH_ERROR_PROGRAM, FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT.
  */
static
HAL_StatusTypeDef
FLASH_ErasePage(                                uint32_t        Page_Address )
{
        HAL_StatusTypeDef       status  = HAL_OK;


        assert_param(IS_FLASH_PROGRAM_ADDRESS(Page_Address));   //Check the parameters
        status  = FLASH_WaitForLastOperation( FLASH_ER_PRG_TIMEOUT );           //Wait for last operation to be completed

        if( status == HAL_OK )
        {
                //If the previous operation is completed, proceed to erase the page
                FLASH->CR       |= FLASH_CR_PER;
                FLASH->AR       = Page_Address;
                FLASH->CR       |= FLASH_CR_STRT;
    
                status          = FLASH_WaitForLastOperation( FLASH_ER_PRG_TIMEOUT );
    
                FLASH->CR       &= ~FLASH_CR_PER;     //Disable the PER Bit
        }
    
        return( status );
}


/*******************************************************************************
*
*******************************************************************************/
/**
  * @brief  Erases PAGE0 and PAGE1 and writes VALID_PAGE header to PAGE0
  * @param  None
  * @retval Status of the last operation (Flash write or erase) done during
  *         EEPROM formating
  */
static
HAL_StatusTypeDef
EE_Format( void )
{
        HAL_StatusTypeDef       FlashStatus     = HAL_OK;

        FlashStatus = FLASH_ErasePage(PAGE0_BASE_ADDRESS);                      //Erase Page0

        if( FlashStatus != HAL_OK )
        {
                return FlashStatus;
        }

        FlashStatus     = FLASH_ProgramHalfWord( PAGE0_BASE_ADDRESS, VALID_PAGE );      //Set Page0 as valid page: Write VALID_PAGE at Page0 base address

        if( FlashStatus != HAL_OK )
        {
                return FlashStatus;
        }

        FlashStatus     = FLASH_ErasePage( PAGE1_BASE_ADDRESS );                //Erase Page1

        return( FlashStatus );
}


/**
  * @brief  Find valid Page for write or read operation
  * @param  Operation: operation to achieve on the valid page.
  *   This parameter can be one of the following values:
  *     @arg READ_FROM_VALID_PAGE: read operation from valid page
  *     @arg WRITE_IN_VALID_PAGE: write operation from valid page
  * @retval Valid page number (PAGE0 or PAGE1) or NO_VALID_PAGE in case
  *   of no valid page was found
  */
static
uint16_t
EE_FindValidPage(                               uint8_t         Operation )
{
        uint16_t        PageStatus0     = 6,
                        PageStatus1     = 6;


        PageStatus0     = (*(__IO uint16_t*)PAGE0_BASE_ADDRESS);
        PageStatus1     = (*(__IO uint16_t*)PAGE1_BASE_ADDRESS);

        switch( Operation )     //Write or read operation
        {
                case WRITE_IN_VALID_PAGE:
                        if( PageStatus1 == VALID_PAGE )
                        {
                                if (PageStatus0 == RECEIVE_DATA)
                                {
                                        return PAGE0;         /* Page0 valid */
                                }
                                else
                                {
                                        return PAGE1;         /* Page1 valid */
                                }
                        }
                        else if( PageStatus0 == VALID_PAGE )
                        {
                                if (PageStatus1 == RECEIVE_DATA)
                                {
                                        return PAGE1;         /* Page1 valid */
                                }
                                else
                                {
                                        return PAGE0;         /* Page0 valid */
                                }
                        }
                        else
                        {
                                return NO_VALID_PAGE;   /* No valid Page */
                        }

                case READ_FROM_VALID_PAGE:  /* ---- Read operation ---- */
                        if( PageStatus0 == VALID_PAGE )
                        {
                                return PAGE0;           /* Page0 valid */
                        }
                        else if (PageStatus1 == VALID_PAGE)
                        {
                                return PAGE1;           /* Page1 valid */
                        }
                        else
                        {
                                return NO_VALID_PAGE ;  /* No valid Page */
                        }

                default:
                        return PAGE0;             /* Page0 valid */
        }
}


/**
  * @brief  Verify if active page is full and Writes variable in EEPROM.
  * @param  VirtAddress: 16 bit virtual address of the variable
  * @param  Data: 16 bit data to be written as variable value
  * @retval Success or error status:
  *           - FLASH_COMPLETE: on success
  *           - PAGE_FULL: if valid page is full
  *           - NO_VALID_PAGE: if no valid page was found
  *           - Flash error code: on write Flash error
  */
static
uint16_t
EE_VerifyPageFullWriteVariable(         uint16_t        VirtAddress,
                                        uint16_t        Data )
{
        HAL_StatusTypeDef       FlashStatus     = HAL_OK;
        //FLASH_Status            FlashStatus     = FLASH_COMPLETE;
        uint16_t                ValidPage       = PAGE0;
        uint32_t                Address         = 0x08010000,
                                PageEndAddress  = 0x080107FF;


        ValidPage       = EE_FindValidPage( WRITE_IN_VALID_PAGE );              //Get valid Page for write operation

        if( ValidPage == NO_VALID_PAGE )                                        //Check if there is no valid page
        {
                return  NO_VALID_PAGE;
        }

        Address         = (uint32_t) ( EEPROM_START_ADDRESS + (uint32_t) (ValidPage * PAGE_SIZE) );     //Get the valid Page start Address
        PageEndAddress  = (uint32_t) ( (EEPROM_START_ADDRESS - 2) + (uint32_t) ( (1 + ValidPage) * PAGE_SIZE) );        //Get the valid Page end Address

        while( Address < PageEndAddress )                                       //Check each active page address starting from begining
        {
                if( (*(__IO uint32_t*) Address) == 0xFFFFFFFF )                 //Verify if Address and Address+2 contents are 0xFFFFFFFF
                {
                        FlashStatus     = FLASH_ProgramHalfWord( Address, Data );       //Set variable data


                        if( FlashStatus != HAL_OK )
                        //if( FlashStatus != FLASH_COMPLETE )                     //If program operation was failed, a Flash error code is returned
                        {
                                return( FlashStatus );
                        }

                        FlashStatus     = FLASH_ProgramHalfWord( Address + 2, VirtAddress );    //Set variable virtual address

                        return( FlashStatus );
                }
                else
                {
                        Address         = Address + 4;                          //Next address location
                }
        }

        return( PAGE_FULL );
}


/**
  * @brief  Transfers last updated variables data from the full Page to
  *   an empty one.
  * @param  VirtAddress: 16 bit virtual address of the variable
  * @param  Data: 16 bit data to be written as variable value
  * @retval Success or error status:
  *           - FLASH_COMPLETE: on success
  *           - PAGE_FULL: if valid page is full
  *           - NO_VALID_PAGE: if no valid page was found
  *           - Flash error code: on write Flash error
  */
static
uint16_t
EE_PageTransfer(                        uint16_t        VirtAddress,
                                        uint16_t        Data )
{
        HAL_StatusTypeDef       FlashStatus     = HAL_OK;
        //FLASH_Status            FlashStatus     = FLASH_COMPLETE;

        //uint32_t                NewPageAddress  = 0x080103FF;
        //uint32_t                OldPageAddress  = 0x08010000;

        uint32_t                NewPageAddress  = 0x0800F800;
        uint32_t                OldPageAddress  = 0x0800FC00;

        uint16_t                ValidPage       = PAGE0,
                                VarIdx          = 0;
        uint16_t                EepromStatus    = 0,
                                ReadStatus      = 0;


        /* Get active Page for read operation */
        ValidPage       = EE_FindValidPage( READ_FROM_VALID_PAGE );

        if (ValidPage == PAGE1)       /* Page1 valid */
        {
                /* New page address where variable will be moved to */
                NewPageAddress = PAGE0_BASE_ADDRESS;

                /* Old page address where variable will be taken from */
                OldPageAddress = PAGE1_BASE_ADDRESS;
        }
        else if( ValidPage == PAGE0 )  /* Page0 valid */
        {
                /* New page address where variable will be moved to */
                NewPageAddress = PAGE1_BASE_ADDRESS;

                /* Old page address where variable will be taken from */
                OldPageAddress = PAGE0_BASE_ADDRESS;
        }
        else
        {
                return NO_VALID_PAGE;       /* No valid Page */
        }

        /* Set the new Page status to RECEIVE_DATA status */
        FlashStatus     = FLASH_ProgramHalfWord( NewPageAddress, RECEIVE_DATA );

        if( FlashStatus != HAL_OK )
        //if( FlashStatus != FLASH_COMPLETE )                                     //If program operation was failed, a Flash error code is returned
        {
                return( FlashStatus );
        }

        EepromStatus    = EE_VerifyPageFullWriteVariable( VirtAddress, Data );  //Write the variable passed as parameter in the new active page

        if( EepromStatus != HAL_OK )
        //if( EepromStatus != FLASH_COMPLETE )                                    //If program operation was failed, a Flash error code is returned
        {
                return EepromStatus;
        }

        /* Transfer process: transfer variables from old to the new active page */
        for( VarIdx = 0; VarIdx < NB_OF_VAR; VarIdx++ )
        {
                if( VirtAddVarTab[VarIdx] != VirtAddress )  /* Check each variable except the one passed as parameter */
                {
                        /* Read the other last variable updates */
                        ReadStatus = stm32_nvm_read16(VirtAddVarTab[VarIdx], &DataVar);
                        /* In case variable corresponding to the virtual address was found */
                        if (ReadStatus != 0x1)
                        {
                                /* Transfer the variable to the new active page */
                                EepromStatus = EE_VerifyPageFullWriteVariable(VirtAddVarTab[VarIdx], DataVar);

                                if( EepromStatus != HAL_OK )
                                //if( EepromStatus != FLASH_COMPLETE )            //If program operation was failed, a Flash error code is returned
                                {
                                        return EepromStatus;
                                }
                        }
                }
        }

        FlashStatus     = FLASH_ErasePage( OldPageAddress );                    //Erase the old Page: Set old Page status to ERASED status
 
        if( FlashStatus != HAL_OK )
        //if( FlashStatus != FLASH_COMPLETE )                                     //If erase operation was failed, a Flash error code is returned
        {
                return( FlashStatus );
        }

        FlashStatus     = FLASH_ProgramHalfWord( NewPageAddress, VALID_PAGE );  //Set new Page status to VALID_PAGE status

        if( FlashStatus != HAL_OK )
        //if( FlashStatus != FLASH_COMPLETE )                                     //If program operation was failed, a Flash error code is returned
        {
                return FlashStatus;
        }

        /* Return last operation flash status */
        return FlashStatus;
}


/**
  * @brief  Restore the pages to a known good state in case of page's status
  *   corruption after a power loss.
  * @param  None.
  * @retval - Flash error code: on write Flash error
  *         - FLASH_COMPLETE: on success
  */
uint16_t
stm32_nvm_init( void )
{
        uint16_t        PageStatus0     = (*(__IO uint16_t*)PAGE0_BASE_ADDRESS);
        uint16_t        PageStatus1     = (*(__IO uint16_t*)PAGE1_BASE_ADDRESS);
        uint16_t        VarIdx          = 0;
        uint16_t        EepromStatus    = 0,
                        ReadStatus      = 0;
        int16_t         x               = -1;
        uint16_t        sts;


        /* Check for invalid header states and repair if necessary */
        switch( PageStatus0 )
        {
                case ERASED:
                        if (PageStatus1 == VALID_PAGE) /* Page0 erased, Page1 valid */
                        {
                                /* Erase Page0 */
                                sts     = FLASH_ErasePage( PAGE0_BASE_ADDRESS );

                                if( sts != HAL_OK )
                                //if (FlashStatus != FLASH_COMPLETE)              //If erase operation was failed, a Flash error code is returned
                                {
                                        return( sts );
                                }
                        }
                        else if (PageStatus1 == RECEIVE_DATA) /* Page0 erased, Page1 receive */
                        {
                                /* Erase Page0 */
                                sts     = FLASH_ErasePage( PAGE0_BASE_ADDRESS );
 
                                if( sts != HAL_OK )
                                //if( FlashStatus != FLASH_COMPLETE )             //If erase operation was failed, a Flash error code is returned
                                {
                                        return( sts );
                                }
                                /* Mark Page1 as valid */
                                sts     = FLASH_ProgramHalfWord(PAGE1_BASE_ADDRESS, VALID_PAGE);

                                if( sts != HAL_OK )
                                //if( FlashStatus != FLASH_COMPLETE )             //If program operation was failed, a Flash error code is returned
                                {
                                        return( sts );
                                }
                        }
                        else /* First EEPROM access (Page0&1 are erased) or invalid state -> format EEPROM */
                        {
                                /* Erase both Page0 and Page1 and set Page0 as valid page */
                                sts     = EE_Format();

                                if( sts != HAL_OK )
                                //if( FlashStatus != FLASH_COMPLETE )             //If erase/program operation was failed, a Flash error code is returned
                                {
                                        return( sts );
                                }
                        }
                        break;

                case RECEIVE_DATA:
                        if (PageStatus1 == VALID_PAGE) /* Page0 receive, Page1 valid */
                        {
                                /* Transfer data from Page1 to Page0 */
                                for (VarIdx = 0; VarIdx < NB_OF_VAR; VarIdx++)
                                {
                                        if (( *(__IO uint16_t*)(PAGE0_BASE_ADDRESS + 6)) == VirtAddVarTab[VarIdx])
                                        {
                                                x = VarIdx;
                                        }

                                        if (VarIdx != x)
                                        {
                                                /* Read the last variables' updates */
                                                ReadStatus = stm32_nvm_read16(VirtAddVarTab[VarIdx], &DataVar);

                                                /* In case variable corresponding to the virtual address was found */
                                                if (ReadStatus != 0x1)
                                                {
                                                        /* Transfer the variable to the Page0 */
                                                        EepromStatus = EE_VerifyPageFullWriteVariable(VirtAddVarTab[VarIdx], DataVar);

                                                        if( EepromStatus != HAL_OK )
                                                        //if( EepromStatus != FLASH_COMPLETE )    //If program operation was failed, a Flash error code is returned
                                                        {
                                                                return EepromStatus;
                                                        }
                                                }
                                        }
                                }

                                /* Mark Page0 as valid */
                                sts     = FLASH_ProgramHalfWord(PAGE0_BASE_ADDRESS, VALID_PAGE);

                                if( sts != HAL_OK )
                                //if( FlashStatus != FLASH_COMPLETE )             //If program operation was failed, a Flash error code is returned
                                {
                                        return( sts );
                                }

                                /* Erase Page1 */
                                sts     = FLASH_ErasePage(PAGE1_BASE_ADDRESS);

                                if( sts != HAL_OK )
                                //if( FlashStatus != FLASH_COMPLETE )             //If erase operation was failed, a Flash error code is returned
                                {
                                        return( sts );
                                }
                        }
                        else if (PageStatus1 == ERASED) /* Page0 receive, Page1 erased */
                        {
                                /* Erase Page1 */
                                sts     = FLASH_ErasePage(PAGE1_BASE_ADDRESS);

                                if( sts != HAL_OK )
                                //if( FlashStatus != FLASH_COMPLETE )             //If erase operation was failed, a Flash error code is returned
                                {
                                        return( sts );
                                }

                                /* Mark Page0 as valid */
                                sts     = FLASH_ProgramHalfWord(PAGE0_BASE_ADDRESS, VALID_PAGE);

                                if( sts != HAL_OK )
                                //if( FlashStatus != FLASH_COMPLETE )             //If program operation was failed, a Flash error code is returned
                                {
                                        return( sts );
                                }
                        }
                        else /* Invalid state -> format eeprom */
                        {
                                /* Erase both Page0 and Page1 and set Page0 as valid page */
                                sts     = EE_Format();

                                if( sts != HAL_OK )
                                //if( FlashStatus != FLASH_COMPLETE )             //If erase/program operation was failed, a Flash error code is returned
                                {
                                        return( sts );
                                }
                        }
                        break;

                case VALID_PAGE:
                        if (PageStatus1 == VALID_PAGE) /* Invalid state -> format eeprom */
                        {
                                /* Erase both Page0 and Page1 and set Page0 as valid page */
                                sts     = EE_Format();

                                if( sts != HAL_OK )
                                //if( FlashStatus != FLASH_COMPLETE )             //If erase/program operation was failed, a Flash error code is returned
                                {
                                        return( sts );
                                }
                        }
                        else if (PageStatus1 == ERASED) /* Page0 valid, Page1 erased */
                        {
                                /* Erase Page1 */
                                sts     = FLASH_ErasePage(PAGE1_BASE_ADDRESS);

                                if( sts != HAL_OK )
                                //if( FlashStatus != FLASH_COMPLETE )             //If erase operation was failed, a Flash error code is returned
                                {
                                        return( sts );
                                }
                        }
                        else /* Page0 valid, Page1 receive */
                        {
                                /* Transfer data from Page0 to Page1 */
                                for (VarIdx = 0; VarIdx < NB_OF_VAR; VarIdx++)
                                {
                                        if ((*(__IO uint16_t*)(PAGE1_BASE_ADDRESS + 6)) == VirtAddVarTab[VarIdx])
                                        {
                                                x = VarIdx;
                                        }

                                        if (VarIdx != x)
                                        {
                                                /* Read the last variables' updates */
                                                ReadStatus = stm32_nvm_read16(VirtAddVarTab[VarIdx], &DataVar);

                                                /* In case variable corresponding to the virtual address was found */
                                                if (ReadStatus != 0x1)
                                                {
                                                        /* Transfer the variable to the Page1 */
                                                        EepromStatus = EE_VerifyPageFullWriteVariable(VirtAddVarTab[VarIdx], DataVar);

                                                        if( EepromStatus != HAL_OK )
                                                        //if( EepromStatus != FLASH_COMPLETE )    //If program operation was failed, a Flash error code is returned
                                                        {
                                                                return EepromStatus;
                                                        }
                                                }
                                        }
                                }

                                /* Mark Page1 as valid */
                                sts     = FLASH_ProgramHalfWord(PAGE1_BASE_ADDRESS, VALID_PAGE);

                                if( sts != HAL_OK )
                                //if( FlashStatus != FLASH_COMPLETE )             //If program operation was failed, a Flash error code is returned
                                {
                                        return( sts );
                                }

                                /* Erase Page0 */
                                sts     = FLASH_ErasePage(PAGE0_BASE_ADDRESS);

                                if( sts != HAL_OK )
                                //if( FlashStatus != FLASH_COMPLETE )             //If erase operation was failed, a Flash error code is returned
                                {
                                        return( sts );
                                }
                        }
                        break;

                default:  /* Any other state -> format eeprom */
                        /* Erase both Page0 and Page1 and set Page0 as valid page */
                        sts     = EE_Format();

                        if( sts != HAL_OK )
                        //if( FlashStatus != FLASH_COMPLETE )                     //If erase/program operation was failed, a Flash error code is returned
                        {
                                return( sts );
                        }
                        break;
        }

        //return( FLASH_COMPLETE );
        return( HAL_OK );
}


/**
  * @brief  Returns the last stored variable data, if found, which correspond to
  *   the passed virtual address
  * @param  VirtAddress: Variable virtual address
  * @param  Data: Global variable contains the read variable value
  * @retval Success or error status:
  *           - 0: if variable was found
  *           - 1: if the variable was not found
  *           - NO_VALID_PAGE: if no valid page was found.
  */
uint16_t
stm32_nvm_read16(                               uint16_t        VirtAddress,
                                                uint16_t *      Data )
{
        uint16_t        ValidPage               = PAGE0;
        uint16_t        AddressValue            = 0x5555,
                        ReadStatus              = 1;
        uint32_t        Address                 = 0x08010000,
                        PageStartAddress        = 0x08010000;


        /* Get active Page for read operation */
        ValidPage = EE_FindValidPage(READ_FROM_VALID_PAGE);

        /* Check if there is no valid page */
        if( ValidPage == NO_VALID_PAGE )
        {
                return  NO_VALID_PAGE;
        }

        /* Get the valid Page start Address */
        PageStartAddress = (uint32_t)(EEPROM_START_ADDRESS + (uint32_t)(ValidPage * PAGE_SIZE));

        /* Get the valid Page end Address */
        Address = (uint32_t)((EEPROM_START_ADDRESS - 2) + (uint32_t)((1 + ValidPage) * PAGE_SIZE));

        /* Check each active page address starting from end */
        while( Address > (PageStartAddress + 2) )
        {
                /* Get the current location content to be compared with virtual address */
                AddressValue = (*(__IO uint16_t*)Address);

                /* Compare the read address with the virtual address */
                if (AddressValue == VirtAddress)
                {
                        /* Get content of Address-2 which is variable value */
                        *Data = (*(__IO uint16_t*)(Address - 2));

                        /* In case variable value is read, reset ReadStatus flag */
                        ReadStatus = 0;

                        break;
                }
                else
                {
                        /* Next address location */
                        Address = Address - 4;
                }
        }

        /* Return ReadStatus value: (0: variable exist, 1: variable doesn't exist) */
        return ReadStatus;
}


/**
  * @brief  Writes/upadtes variable data in EEPROM.
  * @param  VirtAddress: Variable virtual address
  * @param  Data: 16 bit data to be written
  * @retval Success or error status:
  *           - FLASH_COMPLETE: on success
  *           - PAGE_FULL: if valid page is full
  *           - NO_VALID_PAGE: if no valid page was found
  *           - Flash error code: on write Flash error
  */
uint16_t
stm32_nvm_write16(                              uint16_t        VirtAddress,
                                                uint16_t        Data )
{
        uint16_t        ee_sts  = 0;

        /* Write the variable virtual address and value in the EEPROM */
        ee_sts  = EE_VerifyPageFullWriteVariable( VirtAddress, Data );

        /* In case the EEPROM active page is full */
        if( ee_sts == PAGE_FULL )
        {
                ee_sts  = EE_PageTransfer( VirtAddress, Data );
        }

        return( ee_sts );
}

