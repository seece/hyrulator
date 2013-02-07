//implementation of 6502
#include <stdint.h>
#include <iostream>
#include "cpu.hpp"
			
CPU::CPU() {
	mClock = 0;
	mReg = Registers(); // equivalent to Registers(0, 0, 0, 0, 0, 0);
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
	Cycle cycles = 0;
	uint8_t opcode = mMem.read(mReg.PC);
	uint8_t operand1 = mMem.read(mReg.PC + 1);
	uint8_t operand2 = mMem.read(mReg.PC + 2);
	Registers old_reg = mReg;
	//logic
	switch(opcode) {
		//ADC - Add With Carry
		//immidiate: adc #oper
		//2 bytes & 2 cycles
		case 0x69:
			mReg.A += operand1 + getFlag(CARRY);
			setFlag(static_cast<StatusFlag>(CARRY|OVERFLOW), mReg.A < old_reg.A);
			setFlag(static_cast<StatusFlag>(ZERO), !mReg.A);
			cycles = 2;
			mReg.PC += 2;
		break;
		//zero page: adc oper
		//2 bytes & 3 cycles
		case 0x65:
			mReg.A += mMem.read(operand1) + getFlag(CARRY);
			setFlag(static_cast<StatusFlag>(CARRY|OVERFLOW), mReg.A < old_reg.A);
			setFlag(static_cast<StatusFlag>(ZERO), !mReg.A);
			cycles = 3;
			mReg.PC += 2;
		break;
		//zero page,X: adc oper,X
		//2 bytes & 4 cycles
		case 0x75:
			mReg.A += mMem.read(operand1 + mReg.X) +getFlag(CARRY);
			setFlag(static_cast<StatusFlag>(CARRY|OVERFLOW), mReg.A < old_reg.A);
			setFlag(static_cast<StatusFlag>(ZERO), !mReg.A);
			cycles = 4;
			mReg.PC += 2;
		break;
		//Absolute: adc oper
		//3 bytes & 4 cycles
		case 0x60:
			mReg.A += mMem.read(operand1 + operand2*0x100) +getFlag(CARRY);
			setFlag(static_cast<StatusFlag>(CARRY|OVERFLOW), mReg.A < old_reg.A);
			setFlag(static_cast<StatusFlag>(ZERO), !mReg.A);
			cycles = 4;
			mReg.PC += 3;
		break;
		//Absolute,X: adc oper,X
		//3 bytes & 4(+1 if page boundary exceeded) cycles
		case 0x70:
			mReg.A += mMem.read(operand1 + operand2*0x100 + mReg.X) + getFlag(CARRY);
			setFlag(static_cast<StatusFlag>(CARRY|OVERFLOW), mReg.A < old_reg.A);
			setFlag(static_cast<StatusFlag>(ZERO), !mReg.A);
			cycles = 4 + (operand1 + mReg.X) > 0xff;
			mReg.PC += 3;
		break;
		//Absolute,Y: adc oper,Y
		//3 bytes & 4(+1 if page boundary exceeded) cycles
		case 0x79:
			mReg.A += mMem.read(operand1 + operand2*0x100 + mReg.Y) + getFlag(CARRY);
			setFlag(static_cast<StatusFlag>(CARRY|OVERFLOW), mReg.A < old_reg.A);
			setFlag(static_cast<StatusFlag>(ZERO), !mReg.A);
			cycles = 4 + (operand1 + mReg.Y) > 0xff;
			mReg.PC += 3;
		break;
		//(Indirect,X): adc (oper,X)
		//2 bytes & 6 cycles 
		case 0x61:
			mReg.A += mMem.read(mMem.read(operand1 + mReg.X)) + getFlag(CARRY);
			setFlag(static_cast<StatusFlag>(CARRY|OVERFLOW), mReg.A < old_reg.A);
			setFlag(static_cast<StatusFlag>(ZERO), !mReg.A);
			cycles = 6;
			mReg.PC += 2;
		break;
		//(Indirect),X: adc (oper),X
		//2 bytes & 5 cycles
		case 0x71:
			mReg.A += mMem.read(mMem.read(operand1) + mReg.X) + getFlag(CARRY);
			setFlag(static_cast<StatusFlag>(CARRY|OVERFLOW), mReg.A < old_reg.A);
			setFlag(static_cast<StatusFlag>(ZERO), !mReg.A);
			cycles = 5 + (mMem.read(operand1) + mReg.X) > 0xff;
			mReg.PC += 2;
		break;
		default:
			std::cout << static_cast<int32_t>(opcode)
			 	<< " is not a legal opcode."
				<< " PC will be incremented by one byte"
				<< std::endl;
			mReg.PC += 1;
	}
	
	return cycles;
}
