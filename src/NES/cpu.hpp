//implementation of 6502
#pragma once
#include <stdint.h>
#include <functional>
#include "memory.hpp"

enum StatusFlag { 	CARRY = 0x01,
			ZERO = 0x02,
			INTERRUPT = 0x04,
			DECIMAL_MODE = 0x08,
			BRK = 0x10, 
			//ALWAYS_ONE = 0x20,
			OVERFLOW = 0x40,
			SIGN= 0x80 };

struct Registers {
	Registers() : A(0), X(0), Y(0), S(0), PC(0), SP(0) { }
	Registers(uint8_t a, uint8_t x,   uint8_t y, 
		  uint8_t s, uint16_t pc, uint8_t sp)
		: A(a), X(x), Y(y), S(s), PC(pc), SP(sp) { }
	uint8_t A;
	uint8_t X;
	uint8_t Y;
	uint8_t S;
	uint16_t PC;
	uint8_t SP;
};
	
typedef int32_t Cycle;

class CPU {
	public:
		CPU() {};
		CPU(const RomFile* rom);
		CPU(Registers reg, Cycle clock);
		bool getFlag(StatusFlag sflag);
		void setFlag(StatusFlag sflag, bool value = true);
		void clearFlag(StatusFlag sflag);
        void setOverflow(uint8_t a, uint8_t b, uint8_t sum);
		void resetInterrupt(void);
		//do we even need these
		Cycle getClock();
		Cycle changeState();
        //Processor commands
        void ADC(std::function<uint8_t()> read, std::function<uint8_t()> cycles, uint8_t increment);
        void AND(std::function<uint8_t()> read, std::function<uint8_t()> cycles, uint8_t increment);
        void ASL(std::function<uint8_t()> read, std::function<void(uint8_t value)> store, std::function<uint8_t()> cycles, uint8_t increment);
        //branch: bcs
        void branch(uint8_t operand1, bool flag);
        //bit
		friend class Debugger;
	private:
		Registers mReg;
		Cycle mClock;
		Memory mMem;
        Cycle mLastInstructionCycles;

};
