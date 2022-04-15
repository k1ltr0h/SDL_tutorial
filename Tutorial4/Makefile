CPP=g++
CFLAGS= -Wall -o
LIBS= -lmingw32 -lSDL2main -lSDL2
CPP_FILES= *.cpp
OUT=tutorial

all:
	$(CPP) $(CPP_FILES) $(CFLAGS) $(OUT) $(LIBS)

.PHONY: clean
clean:
	del *.exe