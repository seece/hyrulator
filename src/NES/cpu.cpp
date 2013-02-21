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

    uint8_t addressImmediate = operand1;
    uint8_t addressZeroPage = mMem.readb(operand1);
    uint8_t addressZeroPageX = mMem.readb(operand1 + mReg.X);
    uint8_t addressAbsolute = mMem.readb(operand1 + operand2*0x100);
    uint8_t addressAbsoluteX = mMem.readb(operand1 + operand2*0x100 + mReg.X);
    uint8_t addressAbsoluteY =  mMem.readb(operand1 + operand2*0x100 + mReg.Y);
    uint8_t addressIndexedIndirect =  mMem.readb(mMem.readw(operand1 + mReg.X));
    uint8_t addressPreIndexedIndirect = mMem.readb(mMem.readw(operand1) + mReg.X);

    uint8_t cyclesAbsoluteX = 4 + ((operand1 + mReg.Y) > 0xff);
    uint8_t cyclesAbsoluteY = 4 + ((operand1 + mReg.Y) > 0xff);
    uint8_t cyclesPreIndexedIndirect = 5 + ((mMem.readb(operand1) + mReg.Y) > 0xff);
    #define STATUS(X) static_cast<StatusFlag>(X)

	switch(opcode) {
        case 0x69: ADC(addressImmediate, 2, 2); break;
        case 0x65: ADC(addressZeroPage, 3, 2); break;
        case 0x75: ADC(addressZeroPageX, 4, 2); break;
        case 0x60: ADC(addressAbsolute, 4, 3); break;
        case 0x7d: ADC(addressAbsoluteX, cyclesAbsoluteX, 3); break;
        case 0x79: ADC(addressAbsoluteY, cyclesAbsoluteY, 3); break;
        case 0x61: ADC(addressIndexedIndirect, 6, 2); break;
        case 0x71: ADC(addressPreIndexedIndirect, cyclesPreIndexedIndirect, 2); break;

        case 0x29: AND(addressImmediate, 2, 2); break;
        case 0x25: AND(addressZeroPage, 3, 2); break;
        case 0x35: AND(addressZeroPageX, 4, 2); break;
        case 0x2d: AND(addressAbsolute, 4, 3); break;
        case 0x3d: AND(addressAbsoluteX, cyclesAbsoluteX, 3); break;
        case 0x39: AND(addressAbsoluteY, cyclesAbsoluteY, 3); break;
        case 0x21: AND(addressIndexedIndirect, 6, 2); break;
        case 0x31: AND(addressPreIndexedIndirect, cyclesPreIndexedIndirect, 2); break;

        case 0x0A: ASLA(2, 1); break;
        case 0x06: ASL(addressZeroPage, 5, 2); break;
        case 0x16: ASL(addressZeroPageX, 6, 2); break;
        case 0x0E: ASL(addressAbsolute, 6, 3); break;
        case 0x1E: ASL(addressAbsoluteX, 7, 3); break; 

        case 0x90: branch(operand1, !getFlag(CARRY)); break;
        case 0xb0: branch(operand1, getFlag(CARRY)); break;
        case 0xf0: branch(operand1, getFlag(ZERO)); break;
        case 0x30: branch(operand1, getFlag(SIGN)); break;
        case 0xd0: branch(operand1, !getFlag(ZERO)); break;
        case 0x10: branch(operand1, !getFlag(SIGN)); break;
        case 0x50: branch(operand1, !getFlag(OVERFLOW)); break;
        case 0x70: branch(operand1, getFlag(OVERFLOW)); break;

        case 0x24: BIT(addressZeroPage, 3, 2); break;
        case 0x2C: BIT(addressAbsolute, 4, 2); break;

        case 0x00: BRK(); break;

        case 0x18: setFlag(CARRY, 0); mReg.PC++; mLastInstructionCycles = 2; break;
        case 0xd8: setFlag(DECIMAL_MODE, 0); mReg.PC++; mLastInstructionCycles = 2; break;
        case 0x58: setFlag(INTERRUPT, 0); mReg.PC++; mLastInstructionCycles = 2; break;
        case 0xb8: setFlag(OVERFLOW, 0); mReg.PC++; mLastInstructionCycles = 2; break;

		default:
			std::cout << static_cast<int32_t>(opcode)
			 	<< " is not a legal opcode."
				<< " PC will be incremented by one byte"
				<< std::endl;
			mReg.PC += 1;
	}
	
	return mLastInstructionCycles;
}

void CPU::ADC(uint8_t address, uint8_t  cycles, uint8_t increment) {
    uint8_t mem_value = mMem.readb(address);
    uint16_t sum = mReg.A + mem_value + getFlag(CARRY);
    setFlag(CARRY, sum > 0xff);
    setOverflow(mReg.A, mem_value, sum);
    setFlag(ZERO, !sum);
    setFlag(SIGN, sum & 0x80); 
    mLastInstructionCycles = cycles;
    mReg.A = sum;
    mReg.PC += increment;
}

void CPU::AND(uint8_t address, uint8_t cycles, uint8_t increment) {
    mReg.A &= mMem.readb(address);
    setFlag(ZERO, mReg.A);
    setFlag(SIGN, mReg.A & 0x80); 
    mLastInstructionCycles = cycles; 
    mReg.PC += increment;
}

void CPU::ASL(uint8_t address, uint8_t cycles, uint8_t increment) {
    uint8_t mem_value = mMem.readb(address);
    setFlag(CARRY, mem_value & 0x80);
    setFlag(SIGN, mem_value & 0x70);
    mem_value <<= 1;
    setFlag(ZERO, !mem_value);
    mMem.write(address, mem_value); 
    mLastInstructionCycles = cycles;
    mReg.PC += increment;
}

void CPU::ASLA(uint8_t cycles, uint8_t increment) {
    setFlag(CARRY, mReg.A & 0x80);
    setFlag(SIGN, mReg.A & 0x70);
    mReg.A <<= 1;
    setFlag(ZERO, !mReg.A);
    mLastInstructionCycles = cycles;
    mReg.PC += increment;
}

void CPU::branch(uint8_t operand1, bool flag) {
    if(flag) {
        mReg.PC += 2;
        mLastInstructionCycles = 2;
    } else {
        uint8_t address = mReg.PC + (int8_t)operand1;
        mLastInstructionCycles = 3 + ((address & 0xff00) != (mReg.PC & 0xff00));
        mReg.PC = address;
    }
}

void CPU::BIT(uint8_t address, uint8_t cycles, uint8_t increment) {
    uint8_t mem_value = mMem.readb(address);
    setFlag(SIGN, mem_value & 0x80);
    setFlag(OVERFLOW, mem_value & 0x70);
    setFlag(ZERO, !(mem_value & mReg.A));
    mLastInstructionCycles = cycles;
    mReg.PC += increment;
}


void CPU::BRK() {
    mReg.PC++;
    setFlag(INTERRUPT);
    //TODO
}


void CPU::resetInterrupt(void) {
	// read the reset interrupt vector
	mReg.PC = mMem.readw(0xFFFC); 
	std::cout << "Reset interrupt vector: " << std::hex << mReg.PC << std::endl;
}
