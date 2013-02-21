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

    uint8_t addressImmediate = mReg.PC + 1;
    uint8_t addressZeroPage = operand1;
    uint8_t addressZeroPageX = operand1 + mReg.X;
    uint8_t addressZeroPageY = operand1 + mReg.Y;
    uint8_t addressAbsolute = operand1 + operand2*0x100;
    uint8_t addressAbsoluteX = operand1 + operand2*0x100 + mReg.X;
    uint8_t addressAbsoluteY =  operand1 + operand2*0x100 + mReg.Y;
    uint8_t addressIndexedIndirect =  mMem.readw(operand1 + mReg.X);
    uint8_t addressPreIndexedIndirect = mMem.readw(operand1) + mReg.Y;

    uint8_t cyclesAbsoluteX = 4 + ((operand1 + mReg.X) > 0xff);
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

        case 0x0A: ASLA(); break;
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

        case 0xc9: compare(mReg.A, addressImmediate, 2, 2); break;
        case 0xc5: compare(mReg.A, addressZeroPage, 3, 2); break;
        case 0xd5: compare(mReg.A, addressZeroPageX, 4, 2); break;
        case 0xcd: compare(mReg.A, addressAbsolute, 4, 3); break;
        case 0xdd: compare(mReg.A, addressAbsoluteX, cyclesAbsoluteX, 3); break;
        case 0xd9: compare(mReg.A, addressAbsoluteY, cyclesAbsoluteY, 3); break;
        case 0xc1: compare(mReg.A, addressIndexedIndirect, 6, 2); break;
        case 0xd1: compare(mReg.A, addressPreIndexedIndirect, cyclesPreIndexedIndirect, 2); break;

        case 0xe0: compare(mReg.X, addressImmediate, 2, 2); break;
        case 0xe4: compare(mReg.X, addressZeroPage, 3, 2); break;
        case 0xec: compare(mReg.X, addressAbsolute, 4, 3); break;

        case 0xc0: compare(mReg.X, addressImmediate, 2, 2); break;
        case 0xc4: compare(mReg.X, addressZeroPage, 3, 2); break;
        case 0xcc: compare(mReg.X, addressAbsolute, 4, 3); break;

        case 0xc6: addToMemory(addressZeroPage, 5, 2, -1); break;
        case 0xd6: addToMemory(addressZeroPageX, 6, 2, -1); break;
        case 0xce: addToMemory(addressAbsolute, 6, 3, -1); break;
        case 0xde: addToMemory(addressAbsoluteX, 7, 3, -1); break;

        case 0xca: addToRegister(mReg.X, -1); break;
        case 0x88: addToRegister(mReg.Y, -1); break;

        case 0x49: EOR(addressImmediate, 2, 2); break;
        case 0x45: EOR(addressZeroPage, 3, 2); break;
        case 0x55: EOR(addressZeroPageX, 4, 2); break;
        case 0x4d: EOR(addressAbsolute, 4, 3); break;
        case 0x5d: EOR(addressAbsoluteX, cyclesAbsoluteX, 3); break;
        case 0x59: EOR(addressAbsoluteY, cyclesAbsoluteY, 3); break;
        case 0x41: EOR(addressIndexedIndirect, 6, 2); break;
        case 0x51: EOR(addressPreIndexedIndirect, cyclesPreIndexedIndirect, 2); break;

        case 0xe6: addToMemory(addressZeroPage, 5, 2, 1); break;
        case 0xf6: addToMemory(addressZeroPageX, 6, 2, 1); break;
        case 0xee: addToMemory(addressAbsolute, 6, 3, 1); break;
        case 0xfe: addToMemory(addressAbsoluteX, 7, 3, 1); break;

        case 0xe8: addToRegister(mReg.X, 1); break;
        case 0xc8: addToRegister(mReg.Y, 1); break;

        case 0x4c: JMP(operand1 + operand2*0x100, 3); break;
        case 0x6c: JMP(mMem.readw(operand1 + operand2*0x100), 5); break;

        case 0x20: JSR(operand1, operand2); break; 

        case 0xa9: loadToRegister(mReg.A, addressImmediate, 2, 2); break;
        case 0xa5: loadToRegister(mReg.A, addressZeroPage, 3, 2); break;
        case 0xb5: loadToRegister(mReg.A, addressZeroPageX, 4, 2); break;
        case 0xad: loadToRegister(mReg.A, addressAbsolute, 4, 3); break;
        case 0xbd: loadToRegister(mReg.A, addressAbsoluteX, cyclesAbsoluteX, 3); break;
        case 0xb9: loadToRegister(mReg.A, addressAbsoluteY, cyclesAbsoluteY, 3); break;
        case 0xa1: loadToRegister(mReg.A, addressIndexedIndirect, 6, 2); break;
        case 0xb1: loadToRegister(mReg.A, addressPreIndexedIndirect, cyclesPreIndexedIndirect, 2); break;

        case 0xa2: loadToRegister(mReg.X, addressImmediate, 2, 2); break;
        case 0xa6: loadToRegister(mReg.X, addressZeroPage, 3, 2); break;
        case 0xb6: loadToRegister(mReg.X, addressZeroPageY, 4, 2); break;
        case 0xae: loadToRegister(mReg.X, addressAbsolute, 4, 3); break;
        case 0xbe: loadToRegister(mReg.X, addressAbsoluteY, cyclesAbsoluteY, 3); break;

        case 0xa0: loadToRegister(mReg.Y, addressImmediate, 2, 2); break;
        case 0xa4: loadToRegister(mReg.Y, addressZeroPage, 3, 2); break;
        case 0xb4: loadToRegister(mReg.Y, addressZeroPageX, 4, 2); break;
        case 0xac: loadToRegister(mReg.Y, addressAbsolute, 4, 3); break;
        case 0xbc: loadToRegister(mReg.Y, addressAbsoluteX, cyclesAbsoluteX, 3); break;

        case 0x4A: ASRA(); break;
        case 0x46: ASR(addressZeroPage, 5, 2); break;
        case 0x56: ASR(addressZeroPageX, 6, 2); break;
        case 0x4E: ASR(addressAbsolute, 6, 3); break;
        case 0x5E: ASR(addressAbsoluteX, 7, 3); break; 

        case 0xea: mReg.PC++; mLastInstructionCycles = 2; break;

        case 0x09: ORA(addressImmediate, 2, 2); break;
        case 0x05: ORA(addressZeroPage, 3, 2); break;
        case 0x15: ORA(addressZeroPageX, 4, 2); break;
        case 0x0d: ORA(addressAbsolute, 4, 3); break;
        case 0x1d: ORA(addressAbsoluteX, cyclesAbsoluteX, 3); break;
        case 0x19: ORA(addressAbsoluteY, cyclesAbsoluteY, 3); break;
        case 0x01: ORA(addressIndexedIndirect, 6, 2); break;
        case 0x11: ORA(addressPreIndexedIndirect, cyclesPreIndexedIndirect, 2); break;

        case 0x48: push(mReg.A); break;
        case 0x08: push(mReg.SP); break;
        case 0x68: pull(mReg.A); break;
        case 0x28: pull(mReg.A); break;
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

void CPU::ASLA() {
    setFlag(CARRY, mReg.A & 0x80);
    setFlag(SIGN, mReg.A & 0x70);
    mReg.A <<= 1;
    setFlag(ZERO, !mReg.A);
    mLastInstructionCycles = 2;
    mReg.PC += 1;
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
    mReg.SP -= 3;
    mMem.write(0x0100 + mReg. PC + 3, mReg.PC & 0xff00);
    mMem.write(0x0100 + mReg. PC + 2, mReg.PC & 0x00ff);
    mMem.write(0x0100 + mReg. PC + 1, mReg.S & STATUS(BREAK));
    mLastInstructionCycles = 7;
}

void CPU::compare(uint8_t reg, uint8_t address, uint8_t cycles, uint8_t increment) {
    int32_t sum = reg - mMem.readb(address);
    setFlag(ZERO, !sum);
    setFlag(CARRY, sum > 0xff);
    setFlag(SIGN, sum < 0);
    mLastInstructionCycles = cycles;
    mReg.PC += increment;
}

void CPU::addToMemory(uint8_t address, uint8_t cycles, uint8_t increment, int8_t addition) {
    uint8_t sum = mMem.readb(address) + addition;
    mMem.write(address, sum);
    setFlag(ZERO, !sum);    
    setFlag(SIGN, sum & 0x80);
    mLastInstructionCycles = cycles;
    mReg.PC += increment;
}

void CPU::addToRegister(uint8_t& reg, int8_t addition) {
    reg += addition;
    setFlag(ZERO, !reg);
    setFlag(SIGN, reg & 0x80);
    mLastInstructionCycles = 2;
    mReg.PC++;
}

void CPU::EOR(uint8_t address, uint8_t cycles, uint8_t increment) {
    mReg.A ^= mMem.readb(address);
    setFlag(ZERO, mReg.A);
    setFlag(SIGN, mReg.A & 0x80); 
    mLastInstructionCycles = cycles; 
    mReg.PC += increment;
}

void CPU::JMP(uint8_t address, uint8_t cycles) {
    mReg.PC = address;
    mLastInstructionCycles = cycles;   
}

void CPU::JSR(uint8_t operand1, uint8_t operand2) {
    mReg.SP -= 2;
    mMem.write(0x0100+mReg.SP + 1, operand1);
    mMem.write(0x0100+mReg.SP + 2, operand2);
    mReg.PC = operand1 + operand2*0x100;
    mLastInstructionCycles = 3;   
}

void CPU::loadToRegister(uint8_t& reg, uint8_t address, uint8_t cycles, uint8_t increment) {
    reg = mMem.readb(address);
    setFlag(SIGN, 0x80 & reg);
    setFlag(ZERO, !reg);
    mReg.PC += increment;
    mLastInstructionCycles = cycles;
}

void CPU::ASR(uint8_t address, uint8_t cycles, uint8_t increment) {
    uint8_t mem_value = mMem.readb(address);
    setFlag(CARRY, mem_value & 0x01);
    clearFlag(SIGN);
    mem_value >>= 1;
    setFlag(ZERO, !mem_value);
    mMem.write(address, mem_value); 
    mLastInstructionCycles = cycles;
    mReg.PC += increment;
}

void CPU::ASRA() {
    setFlag(CARRY, mReg.A & 0x01);
    clearFlag(SIGN);
    mReg.A >>= 1;
    setFlag(ZERO, !mReg.A);
    mLastInstructionCycles = 2;
    mReg.PC += 1;
}

void CPU::ORA(uint8_t address, uint8_t cycles, uint8_t increment) {
    mReg.A |= mMem.readb(address);
    setFlag(ZERO, mReg.A);
    setFlag(SIGN, mReg.A & 0x80); 
    mLastInstructionCycles = cycles; 
    mReg.PC += increment;
}

void CPU::push(uint8_t reg) {
    mMem.write(0x0100 + mReg.SP, reg);
    mReg.SP--;
    mLastInstructionCycles = 3;
    mReg.PC += 1;
}

void CPU::pull(uint8_t& reg) {
    mReg.SP++;
    reg = mMem.readb(0x0100 + mReg.SP);
    mLastInstructionCycles = 4;
    mReg.PC += 1;
}

void CPU::resetInterrupt(void) {
	// read the reset interrupt vector
	mReg.PC = mMem.readw(0xFFFC); 
	std::cout << "Reset interrupt vector: " << std::hex << mReg.PC << std::endl;
}
