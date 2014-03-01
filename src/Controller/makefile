############
MACHINE= $(shell uname -s)

ifeq ($(MACHINE),Darwin)
    SDL_INC= `sdl-config --cflags`
    SDL_LIB= `sdl-config --libs`
else
    SDL_INC= `sdl-config --cflags`
    SDL_LIB= `sdl-config --libs`
endif

# object files have corresponding source files
OBJS= boost_xbox_controller.o
CXX=g++
COMPILER_FLAGS= -g
INCLUDE= $(SDL_INC)
LIBS= $(SDL_LIB)

EXEC= cont

$(EXEC): $(OBJS)
	$(CXX) $(COMPILER_FLAGS) -o $(EXEC) $(OBJS) $(LIBS)

boost_xbox_controller.o: boost_xbox_controller.cpp
	$(CXX) -c $(COMPILER_FLAGS) -o $@ $< $(INCLUDE)

%.o: %.cpp
	$(CXX) -c $(COMPILER_FLAGS) -o $@ $< $(INCLUDE)

clean:
	rm -f $(EXEC) $(OBJS)