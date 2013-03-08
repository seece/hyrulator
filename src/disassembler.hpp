//disassembler

#include <vector>
#include <string>
#include "NES/memory.hpp"
#include "NES/instruction_decoder.hpp"

namespace dasm {
    using std::vector;
    using std::string;

    struct SymbolicInstruction {
        string name;
        uint8_t length;
    };
    const string INSTRUCTIONS[58] = {"ADC", "AND", "ASL", "BCC", "BCS", "BPL", "BMI", "BVC", "BVS", "BNE", "BEQ", "BIT", "DEX", "JMP", "JMP_INDIRECT", "PLA", "PLP", "ROL", "ROR", "CPX", "CPY", "ORA", "EOR", "RTI", "RTS", "SBC", "SEC", "SED", "SEI", "STA", "STX", "STY", "TAX", "LDA", "CMP", "LSR", "LDX", "LDY", "DEC", "INC", "TAY", "TSX", "TXA", "TXS", "TYA", "NOP", "UNDEFINED_OPCODE", "PHP", "CLC", "PHA", "CLI", "DEY", "CLV", "INY", "CLD", "INX", "BRK", "JSR"};
    //Returns false if the code isn't valid 
     bool disassemble(vector<SymbolicInstruction>& assembly, Memory& mem, uint16_t startPos, uint16_t endPos);
}
