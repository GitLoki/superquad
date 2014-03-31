# Compiler options:
CXX := g++
CC := gcc
CFLAGS := -fPIC -g -Wall -DNON_MATLAB_PARSING -DMAX_EXT_API_CONNECTIONS=255 \
-D__linux `pkg-config --cflags opencv` `sdl2-config --cflags` -O3
LIB := `pkg-config --libs opencv` -lboost_system  -lncurses -lfreenect \
`sdl2-config --libs` -lpthread -lX11
TESTFLAGS := -fPIC -g -pg -fprofile-arcs -ftest-coverage -Wall \
-DNON_MATLAB_PARSING -DMAX_EXT_API_CONNECTIONS=255 -D__linux \
`pkg-config --cflags opencv` `sdl2-config --cflags`
TESTLIB := `pkg-config --libs opencv` -lboost_system  -lncurses -lfreenect \
`sdl2-config --libs` -lpthread -lX11 -lgcov -fprofile-arcs
INC := -I include -I /usr/include/boost -I /usr/local/include/libfreenect

# Directories:
SRCDIR := src
BUILDDIR := build
BINDIR := bin
TESTDIR := test

# Targets:
MAINTARGET := superquad
MODELTESTTARGET := modeltest
KINECTTESTTARGET := kinecttest
KINECTIMAGECAPTURETARGET := kinectimagecapture
TRACKINGTESTTARGET := trackingtest
PIDTESTTARGET := pidtest
TESTTARGETS = $(MODELTESTTARGET) $(KINECTTESTTARGET) \
$(KINECTIMAGECAPTURETARGET) $(TRACKINGTESTTARGET) $(PIDTEST)

ifneq (,$(findstring $(MAKECMDGOALS),$(TESTTARGETS)))
	CFLAGS = $(TESTFLAGS) 
	LIB = $(TESTLIB)
	BUILDDIR := testbuild
endif

# Source files:
SRCEXT := cpp
# SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
# MAINSOURCES := $(filter-out src/Test/ModelTest.cpp src/Test/KinectTest.cpp \
# src/Test/KinectImageCapture.cpp src/Test/TrackingTest.cpp, $(SOURCES))
# MODELTESTSOURCES := $(filter-out src/SuperQuad.cpp src/Test/KinectTest.cpp \
# src/Test/KinectImageCapture.cpp src/Test/TrackingTest.cpp, $(SOURCES))
# KINECTTESTSOURCES := $(filter-out src/SuperQuad.cpp src/Test/ModelTest.cpp \
# src/Test/KinectImageCapture.cpp src/Test/TrackingTest.cpp, $(SOURCES))
# KINECTIMAGECAPTURESOURCES := $(filter-out src/SuperQuad.cpp \
# src/Test/ModelTest.cpp src/Test/KinectTest.cpp src/Test/TrackingTest.cpp, \
# $(SOURCES))
# TRACKINGTESTSOURCES := $(filter-out src/SuperQuad.cpp src/Test/ModelTest.cpp \
# src/Test/KinectTest.cpp src/Test/KinectImageCapture.cpp, $(SOURCES))

#$(SRCDIR)/PhysicsModel/extApi.c $(SRCDIR)/PhysicsModel/extApiCustom.c $(SRCDIR)/PhysicsModel/extApiPlatform.c

AUXSOURCES := $(SRCDIR)/Controller/boost_xbox_controller.cpp $(SRCDIR)/Kinect/Kinect.cpp $(SRCDIR)/Kinect/myfreenectdevice.cpp $(SRCDIR)/PhysicsModel/PhysicsModel.cpp  $(SRCDIR)/Tx/Tx.cpp
MAINSOURCES := $(SRCDIR)/SuperQuad.cpp $(AUXSOURCES)
MODELTESTSOURCES :=  $(SRCDIR)/Test/ModelTest.cpp $(AUXSOURCES)
KINECTTESTSOURCES := $(SRCDIR)/Test/KinectTest.cpp $(AUXSOURCES)
KINECTIMAGECAPTURESOURCES := $(SRCDIR)/Test/KinectImageCapture.cpp $(AUXSOURCES)
TRACKINGTESTSOURCES := $(SRCDIR)/Test/TrackingTest.cpp $(AUXSOURCES)
PIDTESTSOURCES := $(SRCDIR)/PID/PID.cpp $(AUXSOURCES)

# Object files:
MAINOBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,\
$(MAINSOURCES:.$(SRCEXT)=.o)) $(BUILDDIR)/PhysicsModel/extApi.o  \
	$(BUILDDIR)/PhysicsModel/extApiPlatform.o \
$(BUILDDIR)/PhysicsModel/extApiCustom.o
MODELTESTOBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,\
$(MODELTESTSOURCES:.$(SRCEXT)=.o)) $(BUILDDIR)/PhysicsModel/extApi.o \
	$(BUILDDIR)/PhysicsModel/extApiPlatform.o \
$(BUILDDIR)/PhysicsModel/extApiCustom.o
KINECTTESTOBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,\
$(KINECTTESTSOURCES:.$(SRCEXT)=.o)) $(BUILDDIR)/PhysicsModel/extApi.o \
	$(BUILDDIR)/PhysicsModel/extApiPlatform.o \
$(BUILDDIR)/PhysicsModel/extApiCustom.o
KINECTIMAGECAPTUREOBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,\
$(KINECTIMAGECAPTURESOURCES:.$(SRCEXT)=.o)) $(BUILDDIR)/PhysicsModel/extApi.o \
	$(BUILDDIR)/PhysicsModel/extApiPlatform.o \
$(BUILDDIR)/PhysicsModel/extApiCustom.o
TRACKINGTESTOBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,\
$(TRACKINGTESTSOURCES:.$(SRCEXT)=.o)) $(BUILDDIR)/PhysicsModel/extApi.o \
	$(BUILDDIR)/PhysicsModel/extApiPlatform.o \
$(BUILDDIR)/PhysicsModel/extApiCustom.o
PIDTESTOBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,\
$(PIDTESTSOURCES:.$(SRCEXT)=.o)) $(BUILDDIR)/PhysicsModel/extApi.o \
	$(BUILDDIR)/PhysicsModel/extApiPlatform.o \
$(BUILDDIR)/PhysicsModel/extApiCustom.o


# Rules:
$(MAINTARGET): $(MAINOBJECTS)
	@mkdir -p $(BINDIR)
	@echo " Linking..."
	@echo " $(CXX) $^ -o $(MAINTARGET) $(LIB)"; $(CXX) $^ -o \
$(BINDIR)/$(MAINTARGET) $(LIB)

$(MODELTESTTARGET): $(MODELTESTOBJECTS)
	@mkdir -p $(TESTDIR)
	@echo " Linking..."
	@echo " $(CXX) $^ -o $(MODELTESTTARGET) $(LIB)"; $(CXX) $^ -pg \
-fprofile-arcs -ftest-coverage --coverage -o $(TESTDIR)/$(MODELTESTTARGET) $(LIB)

$(KINECTTESTTARGET): $(KINECTTESTOBJECTS)
	@mkdir -p $(TESTDIR)
	@echo " Linking..."
	@echo " $(CXX) $^ -o $(KINECTTESTTARGET) $(LIB)"; $(CXX) $^ -o \
$(TESTDIR)/$(KINECTTESTTARGET) $(LIB)

$(KINECTIMAGECAPTURETARGET): $(KINECTIMAGECAPTUREOBJECTS)
	@mkdir -p $(TESTDIR)
	@echo " Linking..."
	@echo " $(CXX) $^ -o $(KINECTIMAGECAPTURETARGET) $(LIB)"; $(CXX) $^ -o \
$(TESTDIR)/$(KINECTIMAGECAPTURETARGET) $(LIB)

$(TRACKINGTESTTARGET): $(TRACKINGTESTOBJECTS)
	@mkdir -p $(TESTDIR)
	@echo " Linking..."
	@echo " $(CXX) $^ -o $(TRACKINGTESTTARGET) $(LIB)"; $(CXX) $^ -o \
$(TESTDIR)/$(TRACKINGTESTTARGET) $(LIB)

$(PIDTESTTARGET): $(PIDTESTOBJECTS)
	@mkdir -p $(TESTDIR)
	@echo " Linking..."
	@echo " $(CXX) $^ -o $(PIDTESTTARGET) $(LIB)"; $(CXX) $^ -o \
$(TESTDIR)/$(PIDTESTTARGET) $(LIB)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	@mkdir -p $(BUILDDIR)/PhysicsModel
	@mkdir -p $(BUILDDIR)/Tx
	@mkdir -p $(BUILDDIR)/Kinect
	@mkdir -p $(BUILDDIR)/Controller
	@mkdir -p $(BUILDDIR)/Test
	@mkdir -p $(BUILDDIR)/PID
	@echo " $(CXX) $(CFLAGS) $(INC) -c -o $@ $<"; $(CXX) $(CFLAGS) \
$(INC) -c -o $@ $<

$(BUILDDIR)/PhysicsModel/extApi.o: $(SRCDIR)/PhysicsModel/extApi.c
	@echo "Compiling $< to $@" 
	@echo " $(CC) $(CFLAGS) $(INC) -c -o $@ $<"; $(CC) $(CFLAGS) \
$(INC) -c -o $@ $<

$(BUILDDIR)/PhysicsModel/extApiPlatform.o: $(SRCDIR)/PhysicsModel/extApiPlatform.c 
	@echo "Compiling $< to $@" 
	@echo " $(CC) $(CFLAGS) $(INC) -c -o $@ $<"; $(CC) $(CFLAGS) \
$(INC) -c -o $@ $<

$(BUILDDIR)/PhysicsModel/extApiCustom.o: $(SRCDIR)/PhysicsModel/extApiCustom.c
	@echo "Compiling $< to $@" 
	@echo " $(CC) $(CFLAGS) $(INC) -c -o $@ $<"; $(CC) $(CFLAGS) \
$(INC) -c -o $@ $<

clean:
	@echo " Cleaning..."; 
	@echo " $(RM) -r $(BUILDDIR) $(TARGET)"; $(RM) -r $(BUILDDIR) $(TARGET)

.PHONY: clean
