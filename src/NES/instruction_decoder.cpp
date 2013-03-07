//decoding 6592 instructions  decoding

#include <stdint.h>

namespace codec {
    typedef int32_t Cycle;
    enum Opcode{ADC, AND, ASL, BCC, BCS, BIT, DEX, JMP, JMP_INDIRECT, PLA, PLP, ROL, ROR,
                    CPX, CPY, ORA, EOR, RTI, RTS, SBC, SEC, SED, SEI, STA, STX, STY, TAX,
                    LDA, CMP, LSR, LDX, LDY, DEC, INC, TAY, TSX, TXA, TXS, TYA, NOP, UNDEFINED_OPCODE};

    enum AddressingMode {A, IMMEDIATE, ZERO_PAGE, ZERO_PAGE_X, ZERO_PAGE_Y,
                    ABSOLUTE, ABSOLUTE_X, ABSOLUTE_Y, INDEXED_INDIRECT,
                    PREINDEXED_INDIRECT, ACCUMULATOR, EMPTY, UNDEFINED_MODE};
    
    struct Instruction {
        Instruction(Opcode a, AddressingMode b, uint8_t c) : opcode(a), addressingMode(b), length(c) {};
        Opcode opcode;
        AddressingMode addressingMode;
        uint8_t length;
    };

    const Opcode OPCODE[3][8] = {{UNDEFINED_OPCODE, BIT, JMP, JMP_INDIRECT, STY, LDY, CPY, CPX},
                                {ORA, AND, EOR, ADC, STA, LDA, CMP, SBC},
                                {ASL, ROL, LSR, ROR, STX, LDX, DEC, INC}};
    const AddressingMode ADDRESSING_MODE[3][8] = {{IMMEDIATE, ZERO_PAGE, UNDEFINED_MODE, ABSOLUTE, UNDEFINED_MODE, ZERO_PAGE_X, UNDEFINED_MODE, ABSOLUTE_X},
                                                    {PREINDEXED_INDIRECT, ZERO_PAGE, IMMEDIATE, ABSOLUTE, INDEXED_INDIRECT, ZERO_PAGE_X, ABSOLUTE_Y, ABSOLUTE_X},
                                                    {IMMEDIATE, ZERO_PAGE, UNDEFINED_MODE, ACCUMULATOR, ABSOLUTE, UNDEFINED_MODE, ZERO_PAGE_X, ABSOLUTE_X}};
    const uint8_t LENGTH[3][8] = {{2, 2, 0, 3, 0, 2, 0, 3},
                                    {3, 2, 2, 3, 2, 2, 3, 3},
                                    {2, 2, 0, 1, 3, 0, 2, 3}};
    Instruction decode(uint8_t instruction) {
        uint8_t op1 = (instruction & 0b11100000) >> 5;
        uint8_t op2 = instruction & 0b00000011;
        uint8_t add = (instruction & 0b00011100) >> 2;
        Opcode opcode = OPCODE[op2][op1];
        AddressingMode amode = ADDRESSING_MODE[op2][add];
        uint8_t length = LENGTH[op2][add];
        return Instruction(opcode, amode, length);
    } 
}
