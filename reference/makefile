# -j4

#LDFLAGS = -fsanitize=address -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lm
LDFLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lm
INCLUDES = -Iinc/ -Idanklib/ -I.
CFLAGS = -Wall -Werror -Wfatal-errors -g -O3 -MMD -pipe -D_REENTRANT

# Warning exceptions
CFLAGS += -Wno-unused-variable
CFLAGS += -Wno-unused-const-variable
CFLAGS += -Wno-unused-but-set-variable # might be a useful actually
CFLAGS += -Wno-narrowing # might be useful too, we will c
CFLAGS += -Wno-sign-compare

CFLAGS += -std=c++20

LINUX_CFLAGS = -I/usr/include/SDL2

MINGW_DIR = /usr/x86_64-w64-mingw32/bin
SRCS += $(wildcard *.cpp)
SRCS += $(wildcard danklib/*.cpp)

ODIR := crap

OBJ := $(patsubst %.cpp, $(ODIR)/%.o, ${SRCS})
#OBJ = $(SRCS:.cpp=.o)

CC = g++

XCC = /usr/bin/x86_64-w64-mingw32-g++
XOBJ := $(OBJ:.o=.owin)
XCFLAGS += -I/usr/x86_64-w64-mingw32/include/SDL2 -Dmain=SDL_main
XLDFLAGS = -L/usr/x86_64-w64-mingw32/lib -lmingw32 -lSDL2main -lSDL2 -mwindows -Wl,--no-undefined -Wl,--dynamicbase -Wl,--nxcompat -Wl,--high-entropy-va -lm -ldinput8 -ldxguid -ldxerr8 -luser32 -lgdi32 -lwinmm -limm32 -lole32 -loleaut32 -lshell32 -lsetupapi -lversion -luuid -static-libgcc
XLDFLAGS += -lSDL2_image.dll -lSDL2_ttf.dll -lSDL2_mixer.dll

.PHONY: all clean

all: dirs snowkoban

$(ODIR)/%.owin: %.cpp
	$(XCC) -o $@ -c $< $(CFLAGS) $(XCFLAGS) $(INCLUDES)

windist: win
	zip windist.zip *.dll assets/* snowkoban.exe

lindist: snowkoban
	zip lindist.zip assets/* snowkoban

win: $(XOBJ)
	$(XCC) -o snowkoban.exe $^ $(XLDFLAGS)

dirs:
	mkdir -p $(ODIR)/danklib

run: all
	./snowkoban

snowkoban: $(OBJ)
	$(CC) -o  snowkoban $^ $(LDFLAGS)

$(ODIR)/%.o: %.cpp
	$(CC) -o $@ -c $< $(CFLAGS) $(LINUX_CFLAGS) $(INCLUDES)

-include $(OBJ:.o=.d)

clean:
	rm -f snowkoban $(ODIR)/*.o $(ODIR)/*.d $(ODIR)/*.owin
