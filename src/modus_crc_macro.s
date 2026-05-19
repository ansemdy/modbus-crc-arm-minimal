@=============================================================================
@ Modbus CRC-16 Macro Version
@ Zero-Overhead ISR-Optimized Implementation
@ Author: 臧德运 (Zang Deyun)
@ License: MIT
@=============================================================================

@-----------------------------------------------------------------------------
@ MCRC16 Macro
@ Rcrc  = CRC register (input/output, low 16 bits valid)
@ Rxor  = New data byte (input, clobbered)
@ Rswap = Scratch register (input/output, returns 0 or 0x1C0)
@ Cycles: ~12 on Cortex-M3/M4
@-----------------------------------------------------------------------------

    .macro MCRC16 Rcrc, Rxor, Rswap
        EOR     \Rswap, \Rxor, \Rxor, LSR #4
        EOR     \Rswap, \Rswap, \Rswap, LSR #2
        EOR     \Rswap, \Rswap, \Rswap, LSR #1
        ANDS    \Rswap, \Rswap, #1
        IT      NE
        MOVNE   \Rswap, #0x1C0
        EOR     \Rcrc, \Rswap, \Rcrc, LSL #8
        EOR     \Rcrc, \Rcrc, \Rxor, LSL #15
        EOR     \Rcrc, \Rcrc, \Rxor, LSR #1
        EOR     \Rcrc, \Rcrc, \Rxor, LSR #2
        EOR     \Rcrc, \Rcrc, \Rxor, ROR #18
    .endm

@-----------------------------------------------------------------------------
@ Example: USART RX Interrupt Service Routine
@-----------------------------------------------------------------------------
    .global USART2_RX_ISR
    .type USART2_RX_ISR, %function

    USART2_RX_ISR:
        LDR     R1, =USART2_BASE
        LDRB    R2, [R1, #USART_DR]     @ Read received byte

        LDR     R12, =RxBuffer
        LDR     R1, =RxPointer
        LDRB    R3, [R1]                @ Load current pointer

        LDRH    R0, [R12, R3]           @ Load current CRC
        STRB    R2, [R12, R3]           @ Store received byte

        ADD     R3, R3, #1              @ Increment pointer
        STRB    R3, [R1]                @ Store pointer back

        MCRC16  R0, R2, R1              @ 12-cycle CRC update
        STRH    R0, [R12, R3]           @ Store updated CRC

        BX      LR                      @ Return from interrupt
