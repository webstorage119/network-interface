CFLAGS = -g -Wall -I../

all: basic.exe monitor.exe

basic.exe: basic.o ../network_interface.o
	$(CC) $(CFLAGS) -o $@ basic.o ../network_interface.o

monitor.exe: monitor.o ../network_interface.o
	$(CC) $(CFLAGS) -o $@ monitor.o ../network_interface.o

clean:
	rm -rf *.o *.exe
