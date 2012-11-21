//NES


class NES {
	public:
		NES();
		void loadROM(ROM rom);
		void restart();
		

	private:
		CPU cpu;
		PPU ppu;
		APU apu;
};
