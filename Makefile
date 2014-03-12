SRCDIR := src
BUILDDIR := build
BINDIR := bin
TESTDIR := test
MAINTARGET := superquad
MODELTESTTARGET := modeltest
KINECTTESTTARGET := kinecttest

CXX := g++
CC := gcc

SRCEXT := cpp
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
MAINSOURCES := $(filter-out src/Test/ModelTest.cpp src/Test/KinectTest.cpp, $(SOURCES))
MODELTESTSOURCES := $(filter-out src/SuperQuad.cpp src/Test/KinectTest.cpp, $(SOURCES))
KINECTTESTSOURCES := $(filter-out src/SuperQuad.cpp src/Test/ModelTest.cpp, $(SOURCES))

MAINOBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(MAINSOURCES:.$(SRCEXT)=.o)) $(BUILDDIR)/PhysicsModel/extApi.o  \
	$(BUILDDIR)/PhysicsModel/extApiPlatform.o $(BUILDDIR)/PhysicsModel/extApiCustom.o
MODELTESTOBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(MODELTESTSOURCES:.$(SRCEXT)=.o)) $(BUILDDIR)/PhysicsModel/extApi.o \
	$(BUILDDIR)/PhysicsModel/extApiPlatform.o $(BUILDDIR)/PhysicsModel/extApiCustom.o
KINECTTESTOBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(KINECTTESTSOURCES:.$(SRCEXT)=.o)) $(BUILDDIR)/PhysicsModel/extApi.o \
	$(BUILDDIR)/PhysicsModel/extApiPlatform.o $(BUILDDIR)/PhysicsModel/extApiCustom.o

CFLAGS := -fPIC -g -Wall -DNON_MATLAB_PARSING -DMAX_EXT_API_CONNECTIONS=255 -D__linux `pkg-config --cflags opencv` `sdl2-config --cflags` -O3
LIB := `pkg-config --libs opencv` -lboost_system  -lncurses -lfreenect `sdl2-config --libs` -lpthread -lX11
INC := -I include -I /usr/include/boost -I /usr/local/include/libfreenect

$(MAINTARGET): $(MAINOBJECTS)
	@mkdir -p $(BINDIR)
	@echo " Linking..."
	@echo " $(CXX) $^ -o $(MAINTARGET) $(LIB)"; $(CXX) $^ -o $(BINDIR)/$(MAINTARGET) $(LIB)

$(MODELTESTTARGET): $(MODELTESTOBJECTS)
	@mkdir -p $(TESTDIR)
	@echo " Linking..."
	@echo " $(CXX) $^ -o $(MODELTESTTARGET) $(LIB)"; $(CXX) $^ -o $(TESTDIR)/$(MODELTESTTARGET) $(LIB)

$(KINECTTESTTARGET): $(KINECTTESTOBJECTS)
	@mkdir -p $(TESTDIR)
	@echo " Linking..."
	@echo " $(CXX) $^ -o $(KINECTTESTTARGET) $(LIB)"; $(CXX) $^ -o $(TESTDIR)/$(KINECTTESTTARGET) $(LIB)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	@mkdir -p $(BUILDDIR)/PhysicsModel
	@mkdir -p $(BUILDDIR)/Tx
	@mkdir -p $(BUILDDIR)/Kinect
	@mkdir -p $(BUILDDIR)/Controller
	@mkdir -p $(BUILDDIR)/Test
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
