@=============================================================================
@ Modbus CRC-16 Function Version
@ Minimal ARM Assembly Implementation
@ Author: 臧德运 (Zang Deyun)
@ License: MIT
@=============================================================================

    .syntax unified
    .arch armv7-m
    .thumb

    .global ModbusCrc
    .type ModbusCrc, %function

@-----------------------------------------------------------------------------
@ uint16_t ModbusCrc(const uint8_t *data, uint16_t len);
@ Input:  R0 = data pointer
@         R1 = data length
@ Output: R0 = CRC-16 result (REV16 swapped)
@ Clobbers: R2, R3, R12, LR
@-----------------------------------------------------------------------------
    ModbusCrc:
        PUSH    {LR}                    @ Save return address
        MOV     LR, #0x1C0              @ Preload mask constant
        MOV     R12, R0                 @ R12 = data pointer
        MOVW    R0, #0xFFFF             @ Initialize CRC = 0xFFFF

    ModbusCrcByteLoop:
        LDRB    R2, [R12], #1           @ Load byte, post-increment
        EOR     R2, R2, R0, LSR #8      @ R2 = byte ^ (CRC >> 8)

        @ Bit-cross XOR reduction: 8 -> 4 -> 2 -> 1
        EOR     R3, R2, R2, LSR #4      @ Cross bits 0-3 with 4-7
        EOR     R3, R3, R3, LSR #2      @ Cross bits 0-1 with 2-3
        EOR     R3, R3, R3, LSR #1      @ Cross bit 0 with 1

        ANDS    R3, R3, #1              @ Extract parity bit, set Z flag
        MUL     R3, R3, LR              @ Conditional: 0 or 0x1C0

        EOR     R0, R3, R0, LSL #8      @ CRC = (CRC << 8) ^ mask
        EOR     R0, R0, R2, LSL #15     @ Bit 0 -> CRC bit 15
        EOR     R0, R0, R2, LSR #1      @ Bits 1-7 -> CRC high byte (1)
        EOR     R0, R0, R2, LSR #2      @ Bits 2-7 -> CRC high byte (2)
        EOR     R0, R0, R2, ROR #18     @ Bits 0-1 -> CRC low byte 14-15

        UXTH    R0, R0                  @ Mask to 16 bits
        SUBS    R1, R1, #1              @ Decrement length
        BNE     ModbusCrcByteLoop       @ Loop if not done

        REV16   R0, R0                  @ Byte order swap
        POP     {PC}                    @ Return
