//disassembler

#include <vector>
#include <string>
#include "disassembler.hpp"

namespace dasm {
    using std::vector;
    using std::string;

     bool disassemble(vector<SymbolicInstruction>& assembly, Memory& mem, uint16_t startPos, uint16_t endPos) {
        uint16_t address = startPos;
        uint8_t opcode = 0;
        while(address < endPos) {
            opcode = mem.readb(address);
            codec::Instruction ins = codec::decode(opcode);
            //ofcourse this isn't final
            if(ins.opcode == codec::UNDEFINED_OPCODE || ins.addressingMode == codec::UNDEFINED_MODE) {
                return false;   //so it wasn't program all the way
            }
            string s = INSTRUCTIONS[opcode] + " ";
            switch (ins.addressingMode) {
                using namespace codec;
                //make to_hex somewhere
                case ACCUMULATOR: s += "A"; break;
                case IMMEDIATE: s += "#"+mem.readb(address+1); break;
                case ZERO_PAGE: s += "$"+mem.readb(address+1); break;
                case ZERO_PAGE_X: s += "$"+mem.readb(address+1);s += ",X"; break;
                case ZERO_PAGE_Y: s += "$"+mem.readb(address+1);s += ",Y"; break;
                case ABSOLUTE: s += "$"+mem.readw(address+1); break;
                case ABSOLUTE_X: s += "$"+mem.readw(address+1); s += ",X"; break;
                case ABSOLUTE_Y: s += "$"+mem.readw(address+1); s += ",Y"; break;
                case INDEXED_INDIRECT: s += "($"+mem.readb(address+1); s += ",X)"; break;
                case PREINDEXED_INDIRECT: s += "($"+mem.readb(address+1); s += "),Y"; break;
                case RELATIVE: s += "$"+(address+(int32_t)mem.readb(address+1)); break;
                case SINGLE_BYTE: break;
                case UNDEFINED_MODE: s += "UNDEFINED_MODE"; break; 

            }
            assembly.push_back({s, ins.length}); 
        }
        return true;
    }
}
