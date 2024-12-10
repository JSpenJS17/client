GXX = g++
CFILES = src/client.cpp src/engine.cpp src/client_socket.cpp
CARGS = -Wall -D LINUX
OUTNAME = client


all: build

debug:
	$(GXX) -g $(CFILES) -o $(OUTNAME) $(CARGS)

build:
	$(GXX) $(CFILES) -o $(OUTNAME) $(CARGS)

clean:
	rm -f $(OUTNAME)
