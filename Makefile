SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:.cpp=.o)
BINFILE = ./noname
CC = g++

EDITOR = vim

PROFILER = gprof
PFLAGS = -Q

DEBUGGER = insight
DFLAGS =

#COMPILER_FLAGS = -I/home/voider/mydata/current/toolbox -I/usr/X11R6/include -I/usr/local/include -I./external/noise/include -finline-functions -DFREEGLUT_STATIC
COMPILER_FLAGS = -I/usr/X11R6/include -I/usr/local/include -I./external/noise/include -I./external/enet-1.3.1/include -finline-functions
#LINKER_FLAGS = -lm -L/usr/X11R6/lib -L/usr/local/lib -Lexternal/sfml16_linux64/lib -lGL -lGLU -lpthread -lsfml-system-s -lsfml-window-s -lsfml-graphics-s -lsfml-system-s -lXrandr -lIL -lILU -lX11
LINKER_FLAGS = -lm -L/usr/X11R6/lib -L/usr/local/lib -Lexternal/sfml16_linux64/lib -lGL -lGLU -lpthread -lsndfile -lopenal -lGLEW -lsfml-audio -lsfml-graphics -lsfml-window -lsfml-system -lXrandr -lIL -lILU -lX11 -ldl
#LINKER_FLAGS = -L/home/voider/mydata/current/toolbox -L/usr/X11R6/lib -L/usr/local/lib -L./external/noise/lib -lnoise -ltoolbox

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

windows:
	@echo Cross-compiling for Windows
	@make clean
	#@make CC='i686-pc-mingw32-g++' BINFILE='$(BINFILE).exe' COMPILER_FLAGS='-DWINDOWS -I./external/freeglut_mingw/include $(COMPILER_FLAGS) -w' LINKER_FLAGS=' -Wl,--subsystem,windows -L./external/freeglut_mingw/lib -lglu32 -lopengl32 -lfreeglut' all
	#@make CC='i686-pc-mingw32-g++' BINFILE='$(BINFILE).exe' COMPILER_FLAGS='-DFREEGLUT_STATIC -I./external/freeglut_mingw/include $(COMPILER_FLAGS) -w' LINKER_FLAGS='-static-libgcc -static-libstdc++ -Wl,--subsystem,windows -L./external/freeglut_mingw/lib -lglu32 -lopengl32 -lfreeglut_static' all
	@make CC='i486-mingw32-g++' BINFILE='$(BINFILE).exe' COMPILER_FLAGS='-DOS_WINDOWS -DFREEGLUT_STATIC -I./external/freeglut_mingw/include $(COMPILER_FLAGS) -w' LINKER_FLAGS='-static-libgcc -static-libstdc++  -L./external/freeglut_mingw/lib -lfreeglut_static -lopengl32 -lwinmm -lgdi32 -lglu32 -lws2_32' all
	@make clean

mingw:
	@echo Compiling on Windows using mingw
	@make CC='g++' BINFILE='$(BINFILE).exe' COMPILER_FLAGS='-DOS_WINDOWS -DFREEGLUT_STATIC -I./external/freeglut_mingw/include $(COMPILER_FLAGS) -w' LINKER_FLAGS='-static-libgcc -static-libstdc++  -L./external/freeglut_mingw/lib -lfreeglut_static -lopengl32 -lwinmm -lgdi32 -lglu32' all

$(BINFILE) : $(OBJS)
	@echo Linking to $(BINFILE)...
	@$(CC) $(OBJS) -o $(BINFILE) $(LFLAGS)

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

profiler:
	@echo Running program to garther profile informations...
	@./$(BINFILE)
	@echo Running profiler...
	@$(PROFILER) $(PFLAGS) $(BINFILE) > profiler_output.txt
	@echo Opening profiler output...
	@$(EDITOR) profiler_output.txt

cppcheck:
	@cppcheck --enable=all --quiet --template=gcc .

debug:
	@echo Opening Debugger...
	@$(DEBUGGER) $(DFLAGS) $(BINFILE)

valgrind: all
	@valgrind -v --tool=memcheck --leak-check=yes --num-callers=10 $(BINFILE)

valgrindgdb: all
	@valgrind -v --tool=memcheck --leak-check=yes --num-callers=10 --db-attach=yes $(BINFILE)

alleyoop: all
	@alleyoop $(BINFILE)

strip:
	@strip $(BINFILE)

# Regenerate this make file with the sames arguments you used to create it:
regen:
	@genmake

clean:
	@rm -f Makefile.bak
	@rm -f $(OBJS)
	@rm -f gmon.out
	@rm -f profiler_output.txt
	@rm -f $(BINFILE)

depend:
	@$(CC) -MM $(CFLAGS) $(SRCS) > Makefile.dep
	@make -s clean

info:
	@echo SRCS: $(SRCS)
	@echo OBJS: $(OBJS)
	@echo $(BINFILE)


include Makefile.dep

