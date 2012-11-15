//implementation of 6502
#include <stdin.h>
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

uint8_t CPU::getA() {
 	return mA;
}

uint8_t CPU::getX() {
	return mX;
}

uint8_t CPU::getY() {
	return mY;
}

uint8_t CPU::getPC() {
	return mPC;
}

uint8_t CPU::getSP() {
	return mSP;
}

void CPU::setA(uin8_t value) {
	mA = value;
}

void CPU::setX(uin8_t value) {
	mX = value;
}
	
void CPU::setY(uin8_t value) {
	mY = value;
}

void CPU::setPC(uin8_t value) {
	mPC = value;
}
		
void CPU::setSP(uin8_t value){
	mSP = value;
}
Cycle CPU:: getClock() {
	return mClock;

}

Cycle CPU::changeState() {
	Cycle cycles = 0;
	uint8_t opcode = mMem.get(mPC);

	//logic

	mPC++;
	return cycles;
}

