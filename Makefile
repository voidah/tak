SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:.cpp=.o)
BINFILE = ./libtak.so
CC = g++

EDITOR = vim

#COMPILER_FLAGS = -I/usr/X11R6/include -I/usr/local/include -I./external/noise/include -I./external/enet-1.3.1/include -finline-functions -fPIC
COMPILER_FLAGS = -I/usr/X11R6/include -I/usr/local/include -I./external/noise/include -I./external/enet-1.3.1/include -finline-functions -fPIC
LINKER_FLAGS = -lm -L/usr/X11R6/lib -L/usr/local/lib -lGL -lGLU -lpthread -lsndfile -lopenal -lGLEW -lsfml-audio -lsfml-graphics -lsfml-window -lsfml-system -lXrandr -lIL -lILU -lX11 -ldl

# Release:
#CFLAGS = -O3 -fomit-frame-pointer -ffast-math -w $(COMPILER_FLAGS)
#LFLAGS = $(LINKER_FLAGS)

# Debug:
CFLAGS = -g -W -Wall $(COMPILER_FLAGS) -Wno-write-strings -Wno-unused-parameter -Wno-switch -Wno-reorder -DDEBUGMODE -DDEBUG
LFLAGS = $(LINKER_FLAGS)

# Profiling:
#CFLAGS = -pg -W -Wall $(COMPILER_FLAGS) -Wno-write-strings
#LFLAGS = -pg $(LINKER_FLAGS)

all : $(BINFILE)

$(BINFILE) : $(OBJS)
	@echo Creating $(BINFILE)...
	@$(CC) $(LFLAGS) -shared -o $(BINFILE) *.o

%.o : %.cpp
	@echo CC $@
	@$(CC) $(CFLAGS) -c $< -o $@

%.o : %.cc
	@echo CC $@
	@$(CC) $(CFLAGS) -c $< -o $@

%.o : %.C
	@echo CC $@
	@$(CC) $(CFLAGS) -c $< -o $@

%.o : %.cxx
	@echo CC $@
	@$(CC) $(CFLAGS) -c $< -o $@

strip:
	@strip $(BINFILE)

clean:
	@rm -f Makefile.bak
	@rm -f $(OBJS)
	@rm -f gmon.out
	@rm -f profiler_output.txt

depend:
	@$(CC) -MM $(CFLAGS) $(SRCS) > Makefile.dep
	@make -s clean

info:
	@echo SRCS: $(SRCS)
	@echo OBJS: $(OBJS)
	@echo $(BINFILE)


include Makefile.dep

