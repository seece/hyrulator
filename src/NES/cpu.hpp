//implementation of 6502
#include <stdint.h>

enum StatusFlag= { 	CARRY = 0x01,
			ZERO = 0x02,
			INTERRUPT = 0x04,
			DECIMAL_MODE = 0x08,
			BRK = 0x10, 
			//ALWAYS_ONE = 0x20,
			OVERFLOW = 0x40,
			SIGN= 0x80 };
			
typedef int32_t Cycle;

class CPU {
	public:
		CPU(	uint8_t A,
			uint8_t X,
			uint8_t Y,
			uint8_t status,
			uint16_t PC,
			uint8_t SP,
			int32_t clock);
		bool getFlag(StatusFlag sflag);
		void setFlag(StatusFlag sflag, bool value);
		//do we even need these
		int32_t getClock();
		Cycle changeState();

	private:
		uint8_t mA;
		uint8_t mX;
		uint8_t mY;
		uint8_t mStatus;
		uint16_t mPC;
		uint8_t mSP;
		Cycle mClock;
		Memory mMem;
};
