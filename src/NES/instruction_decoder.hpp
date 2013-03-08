//decoding 6592 instructions  decoding
#pragma once
#include <stdint.h>

namespace codec {
    typedef int32_t Cycle;
    
    enum Opcode{ADC, AND, ASL, BCC, BCS, BPL, BMI, BVC, BVS, BNE, BEQ, BIT, DEX, JMP, JMP_INDIRECT, PLA, PLP, ROL, ROR,
                    CPX, CPY, ORA, EOR, RTI, RTS, SBC, SEC, SED, SEI, STA, STX, STY, TAX,
                    LDA, CMP, LSR, LDX, LDY, DEC, INC, TAY, TSX, TXA, TXS, TYA, NOP, UNDEFINED_OPCODE,
                    PHP, CLC, PHA, CLI, DEY, CLV, INY, CLD, INX, BRK, JSR};

    enum AddressingMode {A, IMMEDIATE, ZERO_PAGE, ZERO_PAGE_X, ZERO_PAGE_Y, ABSOLUTE, ABSOLUTE_X, ABSOLUTE_Y, INDEXED_INDIRECT,
                    PREINDEXED_INDIRECT, ACCUMULATOR, RELATIVE, SINGLE_BYTE, UNDEFINED_MODE};
    
    struct Instruction {
        Instruction(Opcode a, AddressingMode b, uint8_t c) : opcode(a), addressingMode(b), length(c) {};
        Opcode opcode;
        AddressingMode addressingMode;
        uint8_t length;
    };

    //tables for determining what to do
    const Opcode OPCODE[3][8] = {{UNDEFINED_OPCODE, BIT, JMP, JMP_INDIRECT, STY, LDY, CPY, CPX},
                                {ORA, AND, EOR, ADC, STA, LDA, CMP, SBC},
                                {ASL, ROL, LSR, ROR, STX, LDX, DEC, INC}};
    const AddressingMode ADDRESSING_MODE[3][8] = {{IMMEDIATE, ZERO_PAGE, UNDEFINED_MODE, ABSOLUTE, UNDEFINED_MODE, ZERO_PAGE_X, UNDEFINED_MODE, ABSOLUTE_X},
                                                    {PREINDEXED_INDIRECT, ZERO_PAGE, IMMEDIATE, ABSOLUTE, INDEXED_INDIRECT, ZERO_PAGE_X, ABSOLUTE_Y, ABSOLUTE_X},
                                                    {IMMEDIATE, ZERO_PAGE, UNDEFINED_MODE, ACCUMULATOR, ABSOLUTE, UNDEFINED_MODE, ZERO_PAGE_X, ABSOLUTE_X}};
    const uint8_t LENGTH[3][8] = {{2, 2, 0, 3, 0, 2, 0, 3},
                                    {3, 2, 2, 3, 2, 2, 3, 3},
                                    {2, 2, 0, 1, 3, 0, 2, 3}};
    const Opcode JUMPS[16] = {UNDEFINED_OPCODE, BPL, UNDEFINED_OPCODE, BMI, UNDEFINED_OPCODE, BVC, UNDEFINED_OPCODE, BCS, UNDEFINED_OPCODE, BCC, UNDEFINED_OPCODE, BCS, UNDEFINED_OPCODE, BNE, UNDEFINED_OPCODE, BEQ};

    const Opcode SINGLE_BYTES[16] = {PHP, CLC, PLP, SEC, PHA, CLI, PLA, SEI, DEY, TYA, TAY, CLV, INY, CLD, INX, SED};

    const Opcode SINGLE_BYTES2[7] = {TXA, TXS, TAX, TSX,  DEX, NOP};

    const Opcode LEFTOVER[7] = {BRK, UNDEFINED_OPCODE, JSR, UNDEFINED_OPCODE, RTI, UNDEFINED_OPCODE, RTS};

    Instruction decode(uint8_t instruction);
}
