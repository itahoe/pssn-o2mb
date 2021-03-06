/**
  * @file    app_isr_hw2026.c
  * @brief   Interrupt Service Routines
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include "app.h"
#include "ui_led.h"


void    ADC1_IRQHandler( void );

/******************************************************************************/
/*           Cortex-M0 Processor Interruption and Exception Handlers          */ 
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void
NMI_Handler( void )
{
}


/**
  * @brief This function handles Hard fault interrupt.
  */
/*
void
hard_fault_handler (                            uint32_t *      arg )
{
        volatile    uint32_t    stacked_r0      = (unsigned long) arg[0];
        volatile    uint32_t    stacked_r1      = (unsigned long) arg[1];
        volatile    uint32_t    stacked_r2      = (unsigned long) arg[2];
        volatile    uint32_t    stacked_r3      = (unsigned long) arg[3];
        volatile    uint32_t    stacked_r12     = (unsigned long) arg[4];
        volatile    uint32_t    stacked_lr      = (unsigned long) arg[5];
        volatile    uint32_t    stacked_pc      = (unsigned long) arg[6];
        volatile    uint32_t    stacked_psr     = (unsigned long) arg[7];


        printf( "[Hard fault handler]\n" );
        printf( "R0 = 0x%x\n", stacked_r0 );
        printf( "R1 = 0x%x\n", stacked_r1 );
        printf( "R2 = 0x%x\n", stacked_r2 );
        printf( "R3 = 0x%x\n", stacked_r3 );
        printf( "R12 = 0x%x\n", stacked_r12 );
        printf( "LR = 0x%x\n", stacked_lr );
        printf( "PC = 0x%x\n", stacked_pc );
        printf( "PSR = 0x%x\n\n", stacked_psr );
        printf( "Execute code from 0x%x\n\n", stacked_pc );
        printf( "See command before 0x%x\n\n", stacked_lr );

	__asm volatile ("bkpt #0\n");         // Break into the debugger
}
*/

/**
  * @brief This function handles Hard fault interrupt.
  */
void
HardFault_Handler(void)
{
/*
        asm volatile
        (
                "    MOV R0, LR                 \n"
                "    CMP R0, #4                 \n"
                "    BNE hf_psp                 \n"

                "hf_msp:                        \n"
                "    MRS R0, MSP                \n"
                "    B hard_fault_handler       \n"

                "hf_psp:                        \n"
                "    MRS R0, PSP                \n"
                "    B hard_fault_handler       \n"
                :                    // no output
                : [param]"r" (0)     // dummy param
                :                    // no clobbers
        );
*/
        #ifdef  NDEBUG
        NVIC_SystemReset();
        #else
        __asm volatile ("bkpt #0\n");   // Break into the debugger
        #endif //NDEBUG

}


/**
  * @brief This function handles System service call via SWI instruction.
  */
void
SVC_Handler(void)
{
}


/**
  * @brief This function handles Pendable request for system service.
  */
void
PendSV_Handler(void)
{
}


/**
  * @brief This function handles System tick timer.
  */
void
SysTick_Handler( void )
{
        HAL_IncTick();
        ui_led_sts_hook();
        app_1msec_tick_hook();
}


/*******************************************************************************
* SER1
*******************************************************************************/
/**
  * @brief  USART1 interrupt request handling
  * @param  None
  * @retval None
  */
void
USART1_IRQHandler( void )
{
        bool    idle    = bsp_ser1_uart_isr();


        if( idle )
        {
                app_ser1_recv_hook();
        }
}


/**
  * @brief  DMA1 channel 2 and 3 interrupts handling
  * @param  None
  * @retval None
  */
void
DMA1_Channel2_3_IRQHandler(void)
{
        bsp_ser1_dma_isr();
}


/*******************************************************************************
* TEMP
*******************************************************************************/
/**
  * @brief  DMA1 channel 4 and 5 interrupts handling
  * @param  None
  * @retval None
  */
void
DMA1_Channel4_5_IRQHandler(void)
{
        //HAL_DMA_IRQHandler(&hdma_i2c2_tx);
        //HAL_DMA_IRQHandler(&hdma_i2c2_rx);
}


/*******************************************************************************
* OXGN
*******************************************************************************/
void
TIM3_IRQHandler( void )
{
        bsp_oxgn_timebase_isr();
        //app_sens_hook();
}


/**
  * @brief  DMA1 channel 4 and 5 interrupts handling
  * @param  None
  * @retval None
  */
void
ADC1_IRQHandler( void )
{
        bool    end_of_seq      = bsp_oxgn_adc_isr();

        if( end_of_seq )
        {
                app_adc_eos_hook();
        }

}


/**
  * @brief  DMA1 channel 1 interrupts handling
  * @param  None
  * @retval None
  */
void DMA1_Channel1_IRQHandler(void)
{
        bsp_oxgn_adc_dma_isr();
}
