//decoding 6592 instructions  decoding
#include "instruction_decoder.hpp"

namespace codec {
    Instruction decode(uint8_t instruction) {
        uint8_t op1 = (instruction & 0b11100000) >> 5;
        uint8_t op2 = instruction & 0b00000011;
        uint8_t add = (instruction & 0b00011100) >> 2;
        Opcode opcode = OPCODE[op2][op1];
        AddressingMode amode = ADDRESSING_MODE[op2][add];
        uint8_t length = LENGTH[op2][add];
        if((instruction << 3) == 0b10000000) {
            opcode = JUMPS[instruction << 4];
            amode = RELATIVE;
            length = 2; 
        }
        if((instruction & 0x0f) == 8) {
            opcode = SINGLE_BYTES[instruction << 4];
            amode = SINGLE_BYTE;
            length = 1;
        }
        if((instruction & 0x0f) == 0xa && (instruction << 4) > 0x7 && (instruction << 4) < 0xf && (instruction << 4) != 0xd) {
            opcode = SINGLE_BYTES2[(instruction << 4) - 8];
            amode = SINGLE_BYTE;
            length = 1;
        }
        if(!(instruction & 0x0f) && !((instruction << 4) % 2) && (instruction << 4) <= 6) {
            opcode = LEFTOVER[instruction << 4];
            amode = (opcode == JSR) ? ABSOLUTE : SINGLE_BYTE; 
            length = (opcode == JSR) ? 3 : 1; 
        }
        return Instruction(opcode, amode, length);
    } 
}
