//implementation of 6502
#include <stdint.h>
#include "cpu.hpp"
			
CPU::CPU(uint8_t A,
	 uint8_t X,
	 uint8_t Y,
	 uint8_t status,
	 uint16_t PC,
	 uint8_t SP,
         int32_t clock) :
	mA(A), mX(X), mY(Y), mStatus(status), mPC(PC), mSP(SP), mClock(clock) { }


bool CPU::getFlag(StatusFlah sflag) {
	return mStatus & sflag;
}
	
void CPU::setFlag(StatusFlag sflag) {
	mStatus = mStatus | sflag ;
}

void CPU::clearFlag(StatusFlag sflag) {
	mStatus = mStatus & ~sflag;

};

Cycle CPU:: getClock() {
	return mClock;

}

Cycle CPU::changeState() {
	Cycle cycles = 0;
	uint8_t opcode = mMem.get(mPC);

	//logic

	//mPC++;
	return cycles;
}

