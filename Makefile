SRCDIR := src
BUILDDIR := build
BINDIR := bin
TARGET := superquad

CXX := g++
SRCEXT := cpp
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o)) $(BUILDDIR)/PhysicsModel/extApi.o  $(BUILDDIR)/PhysicsModel/extApiPlatform.o $(BUILDDIR)/PhysicsModel/extApiCustom.o
CC := gcc

#-std=c++11

CFLAGS := -fPIC -g -Wall -DNON_MATLAB_PARSING -DMAX_EXT_API_CONNECTIONS=255 -D__linux `pkg-config --cflags opencv` `sdl2-config --cflags`
LIB := `pkg-config --libs opencv` -lboost_system  -lncurses -lfreenect `sdl2-config --libs` -lpthread -lX11
INC := -I include -I /usr/include/boost -I /usr/local/include/libfreenect

$(BINDIR)/$(TARGET): $(OBJECTS)
	@mkdir -p $(BINDIR)
	@echo " Linking..."
	@echo " $(CXX) $^ -o $(TARGET) $(LIB)"; $(CXX) $^ -o $(BINDIR)/$(TARGET) $(LIB)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	@mkdir -p $(BUILDDIR)/PhysicsModel
	@mkdir -p $(BUILDDIR)/Tx
	@mkdir -p $(BUILDDIR)/Kinect
	@mkdir -p $(BUILDDIR)/Controller
	@echo " $(CXX) $(CFLAGS) $(INC) -c -o $@ $<"; $(CXX) $(CFLAGS) $(INC) -c -o $@ $<

$(BUILDDIR)/PhysicsModel/extApi.o: $(SRCDIR)/PhysicsModel/extApi.c
	@echo "Compiling $< to $@" 
	@echo " $(CC) $(CFLAGS) $(INC) -c -o $@ $<"; $(CC) $(CFLAGS) $(INC) -c -o $@ $<

$(BUILDDIR)/PhysicsModel/extApiPlatform.o: $(SRCDIR)/PhysicsModel/extApiPlatform.c 
	@echo "Compiling $< to $@" 
	@echo " $(CC) $(CFLAGS) $(INC) -c -o $@ $<"; $(CC) $(CFLAGS) $(INC) -c -o $@ $<

$(BUILDDIR)/PhysicsModel/extApiCustom.o: $(SRCDIR)/PhysicsModel/extApiCustom.c
	@echo "Compiling $< to $@" 
	@echo " $(CC) $(CFLAGS) $(INC) -c -o $@ $<"; $(CC) $(CFLAGS) $(INC) -c -o $@ $<

clean:
	@echo " Cleaning..."; 
	@echo " $(RM) -r $(BUILDDIR) $(TARGET)"; $(RM) -r $(BUILDDIR) $(TARGET)

.PHONY: clean