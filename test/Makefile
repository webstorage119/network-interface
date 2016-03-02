CFLAGS = -g -Wall -I../ -I./mocha/

all: test.exe
	@./test.exe

test.exe: test.o ../network_interface.o ./mocha/mocha.o
	$(CC) $(CFLAGS) -o $@ test.o ../network_interface.o ./mocha/mocha.o

clean:
	rm -rf *.o *.exe
