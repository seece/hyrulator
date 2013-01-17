all:
	make -C ./src
	make -C ./src/test

emulator:
	make -C ./src

clean:
	make -C ./src clean
	make -C ./src/test clean

test:
	./tests
