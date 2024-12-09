GXX = g++
CFILES = funny_game_online.cpp
OUTNAME = online

all: build

debug:
	$(GXX) -g $(CFILES) -o $(OUTNAME)

build:
	$(GXX) $(CFILES) -o $(OUTNAME)

clean:
	rm -f $(OUTNAME)
