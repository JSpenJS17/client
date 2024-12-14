GXX = g++
CFILES = src/client.cpp src/engine.cpp src/client_socket.cpp
OUTNAME = client

# only need socket libraries on windows
ifeq ($(OS), Windows_NT)
	CARGS = -Wall -lws2_32
else
	CARGS = -Wall
endif

all: build

debug:
	$(GXX) -g $(CFILES) -o $(OUTNAME) $(CARGS)

build:
	$(GXX) $(CFILES) -o $(OUTNAME) $(CARGS)

clean:
	rm -f $(OUTNAME)
