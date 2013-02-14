//implementation of 6502
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

Cycle CPU::getClock() {
	return mClock;
}

Cycle CPU::changeState() {
	std::cout << "0x" << std::hex << mReg.PC << ":\t";

	Cycle cycles = 0;
	int8_t opcode = mMem.readb(mReg.PC);
	int8_t operand1 = mMem.readb(mReg.PC + 1);
	int8_t operand2 = mMem.readb(mReg.PC + 2);
	Registers old_reg = mReg;
	//logic
    #define STATUS(X) static_cast<StatusFlag>(X)
	switch(opcode) {
		//ADC - Add With Carry
		//immediate: adc #oper
		//2 bytes & 2 cycles
		case 0x69:
			mReg.A += operand1 + getFlag(CARRY);
			setFlag(STATUS(CARRY|OVERFLOW), mReg.A < old_reg.A);
			setFlag(STATUS(ZERO), !mReg.A);
            setFlag(STATUS(SIGN), mReg.A & 0x80); 
			cycles = 2;
			mReg.PC += 2;
		break;
		//zero page: adc oper
		//2 bytes & 3 cycles
		case 0x65:
			mReg.A += mMem.readb(operand1) + getFlag(CARRY);
			setFlag(STATUS(CARRY|OVERFLOW), mReg.A < old_reg.A);
			setFlag(STATUS(ZERO), !mReg.A);
            setFlag(STATUS(SIGN), mReg.A & 0x80); 
			cycles = 3;
			mReg.PC += 2;
		break;
		//zero page,X: adc oper,X
		//2 bytes & 4 cycles
		case 0x75:
			mReg.A += mMem.readb(operand1 + mReg.X) +getFlag(CARRY);
			setFlag(STATUS(CARRY|OVERFLOW), mReg.A < old_reg.A);
			setFlag(STATUS(ZERO), !mReg.A);
            setFlag(STATUS(SIGN), mReg.A & 0x80); 
			cycles = 4;
			mReg.PC += 2;
		break;
		//Absolute: adc oper
		//3 bytes & 4 cycles
		case 0x60:
			mReg.A += mMem.readb(operand1 + operand2*0x100) +getFlag(CARRY);
			setFlag(STATUS(CARRY|OVERFLOW), mReg.A < old_reg.A);
			setFlag(STATUS(ZERO), !mReg.A);
            setFlag(STATUS(SIGN), mReg.A & 0x80); 
			cycles = 4;
			mReg.PC += 3;
		break;
		//Absolute,X: adc oper,X
		//3 bytes & 4(+1 if page boundary exceeded) cycles
		case 0x70:
			mReg.A += mMem.readb(operand1 + operand2*0x100 + mReg.X) + getFlag(CARRY);
			setFlag(STATUS(CARRY|OVERFLOW), mReg.A < old_reg.A);
			setFlag(STATUS(ZERO), !mReg.A);
            setFlag(STATUS(SIGN), mReg.A & 0x80); 
			cycles = 4 + (operand1 + mReg.X) > 0xff;
			mReg.PC += 3;
		break;
		//Absolute,Y: adc oper,Y
		//3 bytes & 4(+1 if page boundary exceeded) cycles
		case 0x79:
			mReg.A += mMem.readb(operand1 + operand2*0x100 + mReg.Y) + getFlag(CARRY);
			setFlag(STATUS(CARRY|OVERFLOW), mReg.A < old_reg.A);
			setFlag(STATUS(ZERO), !mReg.A);
            setFlag(STATUS(SIGN), mReg.A & 0x80); 
			cycles = 4 + (operand1 + mReg.Y) > 0xff;
			mReg.PC += 3;
		break;
		//(Indirect,X): adc (oper,X)
		//2 bytes & 6 cycles 
		case 0x61:
			mReg.A += mMem.readb(mMem.readw(operand1 + mReg.X)) + getFlag(CARRY);
			setFlag(STATUS(CARRY|OVERFLOW), mReg.A < old_reg.A);
			setFlag(STATUS(ZERO), !mReg.A);
            setFlag(STATUS(SIGN), mReg.A & 0x80); 
			cycles = 6;
			mReg.PC += 2;
		break;
		//(Indirect),X: adc (oper),X
		//2 bytes & 5 cycles
		case 0x71:
			mReg.A += mMem.readb(mMem.readw(operand1) + mReg.X) + getFlag(CARRY);
			setFlag(STATUS(CARRY|OVERFLOW), mReg.A < old_reg.A);
			setFlag(STATUS(ZERO), !mReg.A);
            setFlag(STATUS(SIGN), mReg.A & 0x80); 
			cycles = 5 + (mMem.readb(operand1) + mReg.X) > 0xff;
			mReg.PC += 2;
		break;
        //AND -- memory with accumalator
        //immediate: and #oper
        //2 bytes, 2 cycles
        case 0x29:
            mReg.A &= operand1;
            setFlag(STATUS(ZERO), mReg.A);
            setFlag(STATUS(SIGN), mReg.A & 0x80); 
            cycles = 2;
            mReg.PC += 2;
        //zeropage: and oper
        //2 bytes, 3 cycles
        case 0x35:
            mReg.A &= mMem.readb(operand1);
            setFlag(STATUS(ZERO), mReg.A);
            setFlag(STATUS(SIGN), mReg.A & 0x80); 
            cycles = 3;
            mReg.PC += 2;
        //zeropage,X: and oper,X
        //2 bytes, 4 cycles
        case 0x2d:
            mReg.A &= mMem.readb(operand1 + mReg.X);
            setFlag(STATUS(ZERO), mReg.A);
            setFlag(STATUS(SIGN), mReg.A & 0x80); 
            cycles = 4;
            mReg.PC += 2;
        case 0x3d:
        //absolute: and oper
        //4 bytes, 4 cycles
            mReg.A &= mMem.readb(operand1 + operand2*0x100);
            setFlag(STATUS(ZERO), mReg.A);
            setFlag(STATUS(SIGN), mReg.A & 0x80); 
            cycles = 4;
            mReg.PC += 3;
        case 0x39:
        //absolute,X: and oper,X
        //4 bytes, 4 + (1 if boundary crossed)
            mReg.A &= mMem.readb(operand1 + operand2*0x100 + mReg.X);
            setFlag(STATUS(ZERO), mReg.A);
            setFlag(STATUS(SIGN), mReg.A & 0x80); 
            cycles = 4 + (operand1 + mReg.X) > 0xFF;
            mReg.PC += 3;
        case 0x21:
        //indirect absolute,x: and (oper,X)
        //4 bytes, 4 + (1 if boundary crossed)
            mReg.A &= mMem.readb(mMem.readw(operand1 + mReg.X));
            setFlag(STATUS(ZERO), mReg.A);
            setFlag(STATUS(SIGN), mReg.A & 0x80); 
            cycles = 6;
            mReg.PC += 2;
        case 0x31:
        //absolute,Y: and (oper),Y
        //4 bytes, 4 + (1 if boundary crossed)
            mReg.A &= mMem.readb(mMem.readw(operand1) + mReg.Y);
            setFlag(STATUS(ZERO), mReg.A);
            setFlag(STATUS(SIGN), mReg.A & 0x80); 
            cycles = 5 + (operand1 + mReg.Y) > 0xFF;
            mReg.PC += 2;
		default:
			std::cout << static_cast<int32_t>(opcode)
			 	<< " is not a legal opcode."
				<< " PC will be incremented by one byte"
				<< std::endl;
			mReg.PC += 1;
	}
	
	return cycles;
}

void CPU::resetInterrupt(void) {
	// read the reset interrupt vector
	mReg.PC = mMem.readw(0xFFFC); 
	std::cout << "Reset interrupt vector: " << std::hex << mReg.PC << std::endl;
}
