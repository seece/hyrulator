//implementation of 6502
#include <stdint.h>
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
		CPU(Registers reg, Cycle clock);
		bool getFlag(StatusFlag sflag);
		void setFlag(StatusFlag sflag, bool value = 1);
		void clearFlag(StatusFlag sflag);
		//do we even need these
		Cycle getClock();
		Cycle changeState();

	private:
		Registers mReg;
		Cycle mClock;
		Memory mMem;

};
