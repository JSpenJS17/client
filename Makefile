GXX = g++
CFILES = main.cpp socket.cpp
OUTNAME = main

all: build

build:
	$(GXX) $(CFILES) -o main
	$(GXX) main2.cpp socket.cpp -o main2

clean:
	rm -f main server
