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
    auto readA = [&] () { return mReg.A; };
    auto readImmediate = [&] () { return operand1; };
    auto readZeroPage = [&] () { return mMem.readb(operand1); };
    auto readZeroPageX = [&] () { return mMem.readb(operand1 + mReg.X); };
    auto readAbsolute = [&] () { return mMem.readb(operand1 + operand2*0x100); };
    auto readAbsoluteX = [&] () { return mMem.readb(operand1 + operand2*0x100 + mReg.X); };
    auto readAbsoluteY =  [&] () { return mMem.readb(operand1 + operand2*0x100 + mReg.Y); };
    auto readIndexedIndirect =  [&] () { return mMem.readb(mMem.readw(operand1 + mReg.X)); };
    auto readPreIndexedIndirect = [&] () { return mMem.readb(mMem.readw(operand1) + mReg.X); };

    //auto storeImmediate = [&] () { operand1; };
    auto storeA = [&] (const uint8_t& val) { mReg.A = val; };
    auto storeZeroPage = [&] (const uint8_t& val) { mMem.write(operand1, val); };
    auto storeZeroPageX = [&] (const uint8_t& val) { mMem.write(operand1 + mReg.X, val); };
    auto storeAbsolute = [&] (const uint8_t& val) { mMem.write(operand1 + operand2*0x100, val); };
    auto storeAbsoluteX = [&] (const uint8_t& val) { mMem.write(operand1 + operand2*0x100 + mReg.X, val); };
    auto storeAbsoluteY =  [&] (const uint8_t& val) { mMem.write(operand1 + operand2*0x100 + mReg.Y, val); };
    auto storeIndexedIndirect =  [&] (const uint8_t& val) { mMem.write(mMem.readw(operand1 + mReg.X), val); };
    auto storePreIndexedIndirect = [&] (const uint8_t& val) { mMem.write(mMem.readw(operand1) + mReg.X, val); };
  
    //TODO overload instructions so that this unelegancy is reduced 
    auto cycles1 = [] () { return 1; };
    auto cycles2 = [] () { return 2; };
    auto cycles3 = [] () { return 3; };
    auto cycles4 = [] () { return 4; };
    auto cycles5 = [] () { return 5; };
    auto cycles6 = [] () { return 6; };
    auto cycles7 = [] () { return 7; };
    auto cyclesAbsoluteX = [&] () { return 4 + ((operand1 + mReg.Y) > 0xff); };
    auto cyclesAbsoluteY = [&] () { return 4 + ((operand1 + mReg.Y) > 0xff); };
    auto cyclesPreIndexedIndirect = [&] () { return 5 + ((mMem.readb(operand1) + mReg.Y) > 0xff); };
    #define STATUS(X) static_cast<StatusFlag>(X)

	switch(opcode) {
        case 0x69: ADC(readImmediate, cycles2, 2); break;
        case 0x65: ADC(readZeroPage, cycles3, 2); break;
        case 0x75: ADC(readZeroPageX, cycles4, 2); break;
        case 0x60: ADC(readAbsolute, cycles4, 3); break;
        case 0x70: ADC(readAbsoluteX, cyclesAbsoluteX, 3); break;
        case 0x79: ADC(readAbsoluteY, cyclesAbsoluteY, 3); break;
        case 0x61: ADC(readIndexedIndirect, cycles6, 2); break;
        case 0x71: ADC(readPreIndexedIndirect, cyclesPreIndexedIndirect, 2); break;

        case 0x29: AND(readImmediate, cycles2, 2); break;
        case 0x25: AND(readZeroPage, cycles3, 2); break;
        case 0x35: AND(readZeroPageX, cycles4, 2); break;
        case 0x2d: AND(readAbsolute, cycles4, 3); break;
        case 0x3d: AND(readAbsoluteX, cyclesAbsoluteX, 3); break;
        case 0x39: AND(readAbsoluteY, cyclesAbsoluteY, 3); break;
        case 0x21: AND(readIndexedIndirect, cycles6, 2); break;
        case 0x31: AND(readPreIndexedIndirect, cyclesPreIndexedIndirect, 2); break;

        //lambda 
        case 0x0A: ASL(readA, storeA, cycles2, 1); break;
        case 0x06: ASL(readZeroPage, storeZeroPage, cycles5, 2); break;
        case 0x16: ASL(readZeroPageX, storeZeroPageX, cycles6, 2); break;
        case 0x0E: ASL(readAbsolute, storeAbsolute, cycles6, 3); break;
        case 0x1E: ASL(readAbsoluteX, storeAbsoluteX, cycles7, 3); break; 

        case 0x90: branch(operand1, !getFlag(CARRY)); break;
        case 0xb0: branch(operand1, getFlag(CARRY)); break;
        case 0xf0: branch(operand1, getFlag(ZERO)); break;
        case 0x30: branch(operand1, getFlag(SIGN)); break;
        case 0xd0: branch(operand1, !getFlag(ZERO)); break;
        case 0x10: branch(operand1, !getFlag(SIGN)); break;

		default:
			std::cout << static_cast<int32_t>(opcode)
			 	<< " is not a legal opcode."
				<< " PC will be incremented by one byte"
				<< std::endl;
			mReg.PC += 1;
	}
	
	return mLastInstructionCycles;
}

void CPU::ADC(std::function<uint8_t()> read, std::function<uint8_t()> cycles, uint8_t increment) {
    uint8_t mem_value = read();
    uint16_t sum = mReg.A + mem_value + getFlag(CARRY);
    setFlag(STATUS(CARRY), sum > 0xff);
    setOverflow(mReg.A, mem_value, sum);
    setFlag(STATUS(ZERO), !sum);
    setFlag(STATUS(SIGN), sum & 0x80); 
    mLastInstructionCycles = cycles();
    mReg.A = sum;
    mReg.PC += increment;
}

void CPU::AND(std::function<uint8_t()> read, std::function<uint8_t()> cycles, uint8_t increment) {
    mReg.A &= read();
    setFlag(STATUS(ZERO), mReg.A);
    setFlag(STATUS(SIGN), mReg.A & 0x80); 
    mLastInstructionCycles = cycles();
    mReg.PC += increment;
}

void CPU::ASL(std::function<uint8_t()> read, std::function<void(uint8_t value)> store, std::function<uint8_t()> cycles, uint8_t increment) {
    uint8_t mem_value = read();
    setFlag(CARRY, mem_value & 0x80);
    setFlag(SIGN, mem_value & 0x70);
    mem_value <<= 1;
    setFlag(ZERO, !mem_value);
    store(mem_value); 
    mLastInstructionCycles = cycles();
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

void CPU::resetInterrupt(void) {
	// read the reset interrupt vector
	mReg.PC = mMem.readw(0xFFFC); 
	std::cout << "Reset interrupt vector: " << std::hex << mReg.PC << std::endl;
}
