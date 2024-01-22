CPP=g++
CFLAGS= -Wall -o
LIBS= -lmingw32 -lSDL2main -lSDL2
CPP_FILES= *.cpp
OUT=tutorial
ICON=icon.res
FREE=-mwindows

all:
	$(CPP) $(CPP_FILES) $(CFLAGS) $(OUT) $(LIBS) $(ICON) $(FREE)

test: 
	$(CPP) $(CPP_FILES) $(CFLAGS) $(OUT) $(LIBS) $(ICON)

.PHONY: clean
clean:
	rm *.exe