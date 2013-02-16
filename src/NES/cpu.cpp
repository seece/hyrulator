//implementation of 6503
#include <stdint.h>
#include <iostream>
#include "cpu.hpp"
			
CPU::CPU(const RomFile* rom) {
	mClock = 0;
	mReg = Registers(); // equivalent to Registers(0, 0, 0, 0, 0, 0);
	mMem = Memory(rom);
}

CPU::CPU(Registers reg, int32_t clock) :
	mReg(reg), mClock(clock) { }


bool CPU::getFlag(StatusFlag sflag) {
	return mReg.S & sflag;
}
	
void CPU::setFlag(StatusFlag sflag, bool value) {
	if(value) {
		mReg.S = mReg.S | sflag ;
	} else {
		clearFlag(sflag);
	}
}

void CPU::clearFlag(StatusFlag sflag) {
	mReg.S = mReg.S & (0xff ^ sflag);
}

void CPU::setOverflow(uint8_t a, uint8_t b, uint8_t sum) {
    //If a and b don't differ in sign but a and result do.
    setFlag(OVERFLOW, !((a ^ b) & 0x80) && ((a ^ sum) & 0x80));
}

Cycle CPU::getClock() {
	return mClock;
}

Cycle CPU::changeState() {
	std::cout << "0x" << std::hex << mReg.PC << ":\t";

	Cycle mLastInstructionCycles = 0;
    //TODO Exception handling
	uint8_t opcode = mMem.readb(mReg.PC);
	uint8_t operand1 = mMem.readb(mReg.PC + 1);
	uint8_t operand2 = mMem.readb(mReg.PC + 2);
	//logic
    #define STATUS(X) static_cast<StatusFlag>(X)
	switch(opcode) {
		case 0x69: adcImmediate(operand1); break;
		case 0x65: adcZeroPage(operand1); break;
		case 0x75: adcZeroPageX(operand1); break;
        case 0x60: adcAbsolute(operand1, operand2); break;
		case 0x70: adcAbsoluteX(operand1, operand2); break;
		case 0x79: adcAbsoluteY(operand1, operand2); break;
		case 0x61: adcIndexedIndirect(operand1); break;
		case 0x71: adcPreIndexedIndirect(operand1); break;

        case 0x29: andImmediate(operand1); break;
        case 0x35: andZeroPage(operand1); break;
        case 0x2d: andZeroPageX(operand1); break;
        case 0x3d: andAbsolute(operand1, operand2); break;
        case 0x39: andAbsoluteX(operand1, operand2); break;
        case 0x21: andAbsoluteY(operand1, operand2); break;
        case 0x31: andIndexedIndirect(operand1); break;
		default:
			std::cout << static_cast<int32_t>(opcode)
			 	<< " is not a legal opcode."
				<< " PC will be incremented by one byte"
				<< std::endl;
			mReg.PC += 1;
	}
	
	return mLastInstructionCycles;
}
//ADC - Add With Carry
//immidiate: adc #oper
//2 bytes & 2 cycles

//a+b=c
//(!((a & 80) ^ (b & 80)) ^ (c & 80))
void CPU::adcImmediate(uint8_t operand1) {
    uint16_t sum = mReg.A + operand1 + getFlag(CARRY);
    setFlag(STATUS(CARRY), sum > 0xff);
    setOverflow(mReg.A, operand1, sum);
    setFlag(STATUS(ZERO), !sum);
    setFlag(STATUS(SIGN), sum & 0x80); 
    mLastInstructionCycles = 2;
    mReg.A = sum;
    mReg.PC += 2;
}

//zero page: adc oper
//2 bytes & 3 cycles
void CPU::adcZeroPage(uint8_t operand1) {
    uint8_t mem_value = mMem.readb(operand1); 
    uint16_t sum = mReg.A + mem_value + getFlag(CARRY);
    setFlag(STATUS(CARRY), sum > 0xff);
    setOverflow(mReg.A, mem_value, sum);
    setFlag(STATUS(ZERO), !sum);
    setFlag(STATUS(SIGN), sum & 0x80); 
    mLastInstructionCycles = 3;
    mReg.A = sum;
    mReg.PC += 2;
}

//zero page,X: adc oper,X
//2 bytes & 4 cycles
void CPU::adcZeroPageX(uint8_t operand1){
    uint8_t mem_value = mMem.readb(operand1 + mReg.X); 
    uint16_t sum = mReg.A + mem_value + getFlag(CARRY);
    setFlag(STATUS(CARRY), sum > 0xff);
    setOverflow(mReg.A, mem_value, sum);
    setFlag(STATUS(ZERO), !sum);
    setFlag(STATUS(SIGN), sum & 0x80); 
    mLastInstructionCycles = 4;
    mReg.A = sum;
    mReg.PC += 2;
}

//Absolute: adc oper
//3 bytes & 4 cycles
void CPU::adcAbsolute(uint8_t operand1, uint8_t operand2) {
    uint8_t mem_value = mMem.readb(operand1 + operand2*0x100); 
    uint16_t sum = mReg.A + mem_value + getFlag(CARRY);
    setFlag(STATUS(CARRY|OVERFLOW), sum > 0xff);
    setOverflow(mReg.A, mem_value, sum);
    setFlag(STATUS(ZERO), !sum);
    setFlag(STATUS(SIGN), sum & 0x80); 
    mLastInstructionCycles = 4;
    mReg.A = sum;
    mReg.PC += 3;
}

//Absolute,X: adc oper,X
//3 bytes & 4(+1 if page boundary exceeded) cycles
void CPU::adcAbsoluteX(uint8_t operand1, uint8_t operand2) {
    uint8_t mem_value = mMem.readb(operand1 + operand2*0x100 + mReg.X); 
    uint16_t sum = mReg.A + mem_value  + getFlag(CARRY);
    setFlag(STATUS(CARRY|OVERFLOW), sum > 0xff);
    setOverflow(mReg.A, mem_value, sum);
    setFlag(STATUS(ZERO), !sum);
    setFlag(STATUS(SIGN), sum & 0x80); 
    mLastInstructionCycles = 4 + (operand1 + mReg.X) > 0xff;
    mReg.A = sum;
    mReg.PC += 3;
}

//Absolute,Y: adc oper,Y
//3 bytes & 4(+1 if page boundary exceeded) cycles
void CPU::adcAbsoluteY(uint8_t operand1, uint8_t operand2) {
    uint8_t mem_value =  mMem.readb(operand1 + operand2*0x100 + mReg.Y);
    uint16_t sum = mReg.A + mem_value + getFlag(CARRY);
    setFlag(STATUS(CARRY), sum > 0xff);
    setOverflow(mReg.A, mem_value, sum);
    setFlag(STATUS(ZERO), !sum);
    setFlag(STATUS(SIGN), sum & 0x80); 
    mLastInstructionCycles = 4 + (operand1 + mReg.Y) > 0xff;
    mReg.A = sum;
    mReg.PC += 3;
}

//(Indirect,X): adc (oper,X)
//2 bytes & 6 cycles 
void CPU::adcIndexedIndirect(uint8_t operand1) {
    uint8_t mem_value =  mMem.readb(mMem.readw(operand1 + mReg.X));
    uint16_t sum = mReg.A + mem_value + getFlag(CARRY);
    setFlag(STATUS(CARRY), sum > 0xff);
    setOverflow(mReg.A, mem_value, sum);
    setFlag(STATUS(ZERO), !sum);
    setFlag(STATUS(SIGN), sum & 0x80); 
    mLastInstructionCycles = 6;
    mReg.A = sum;
    mReg.PC += 2;
}

//(Indirect),X: adc (oper),X
//2 bytes & 5 cycles
void CPU::adcPreIndexedIndirect(uint8_t operand1) {
    uint8_t mem_value = mMem.readb(mMem.readw(operand1) + mReg.X);
    uint16_t sum = mReg.A + mem_value + getFlag(CARRY);
    setFlag(STATUS(CARRY), sum > 0xff);
    setOverflow(mReg.A, mem_value, sum);
    setFlag(STATUS(ZERO), !sum);
    setFlag(STATUS(SIGN), sum & 0x80); 
    mLastInstructionCycles = 5 + (mMem.readb(operand1) + mReg.X) > 0xff;
    mReg.A = sum;
    mReg.PC += 2;
}

//AND -- memory with accumalator
//immediate: and #oper
//2 bytes, 2 cycles
void CPU::andImmediate(uint8_t operand1) {
    mReg.A &= operand1;
    setFlag(STATUS(ZERO), mReg.A);
    setFlag(STATUS(SIGN), mReg.A & 0x80); 
    mLastInstructionCycles = 2;
    mReg.PC += 2;
}

//zeropage: and oper
//2 bytes, 3 cycles
void CPU::andZeroPage(uint8_t operand1) {
    mReg.A &= mMem.readb(operand1);
    setFlag(STATUS(ZERO), mReg.A);
    setFlag(STATUS(SIGN), mReg.A & 0x80); 
    mLastInstructionCycles = 3;
    mReg.PC += 2;
}

//zeropage,X: and oper,X
//2 bytes, 4 cycles
void CPU::andZeroPageX(uint8_t operand1) {
    mReg.A &= mMem.readb(operand1 + mReg.X);
    setFlag(STATUS(ZERO), mReg.A);
    setFlag(STATUS(SIGN), mReg.A & 0x80); 
    mLastInstructionCycles = 4;
    mReg.PC += 2;
}

//absolute: and oper
//3 bytes, 4 cycles
void CPU::andAbsolute(uint8_t operand1, uint8_t operand2) {
    mReg.A &= mMem.readb(operand1 + operand2*0x100);
    setFlag(STATUS(ZERO), mReg.A);
    setFlag(STATUS(SIGN), mReg.A & 0x80); 
    mLastInstructionCycles = 4;
    mReg.PC += 3;
}

//absolute,X: and oper,X
//3 bytes, 4 + (1 if boundary crossed)
void CPU::andAbsoluteX(uint8_t operand1, uint8_t operand2) {
    mReg.A &= mMem.readb(operand1 + operand2*0x100 + mReg.X);
    setFlag(STATUS(ZERO), mReg.A);
    setFlag(STATUS(SIGN), mReg.A & 0x80); 
    mLastInstructionCycles = 4 + (operand1 + mReg.X) > 0xFF;
    mReg.PC += 3;
}

//indirect absolute,x: and (oper,X)
//4 bytes, 4 + (1 if boundary crossed)
void CPU::andAbsoluteY(uint8_t operand1, uint8_t operand2) {
    mReg.A &= mMem.readb(mMem.readw(operand1 + mReg.X));
    setFlag(STATUS(ZERO), mReg.A);
    setFlag(STATUS(SIGN), mReg.A & 0x80); 
    mLastInstructionCycles = 6;
    mReg.PC += 2;
}

//absolute,Y: and (oper),Y
//4 bytes, 4 + (1 if boundary crossed)
void CPU::andIndexedIndirect(uint8_t operand1) {
    mReg.A &= mMem.readb(mMem.readw(operand1) + mReg.Y);
    setFlag(STATUS(ZERO), mReg.A);
    setFlag(STATUS(SIGN), mReg.A & 0x80); 
    mLastInstructionCycles = 5 + (operand1 + mReg.Y) > 0xFF;
    mReg.PC += 2;
}

void CPU::andPreIndexedIndirect(uint8_t operand1) {
    mReg.A &= mMem.readb(mMem.readw(operand1) + mReg.X);
    setFlag(STATUS(ZERO), !mReg.A);
    setFlag(STATUS(SIGN), mReg.A & 0x80); 
    mLastInstructionCycles = 5 + (mMem.readb(operand1) + mReg.X) > 0xff;
    mReg.PC += 2;
}

void CPU::resetInterrupt(void) {
	// read the reset interrupt vector
	mReg.PC = mMem.readw(0xFFFC); 
	std::cout << "Reset interrupt vector: " << std::hex << mReg.PC << std::endl;
}
