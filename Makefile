# Compiler options: 
SHELL := /bin/bash
CXX := g++
CC := gcc
DEFINES := -DQT_QML_DEBUG -DQT_DECLARATIVE_DEBUG -DQT_PRINTSUPPORT_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_CORE_LIB
CFLAGS := -fPIC -m64 -g -Wall -DNON_MATLAB_PARSING -DMAX_EXT_API_CONNECTIONS=255 \
-D__linux `pkg-config --cflags opencv` `sdl2-config --cflags` -O3 \
-m64 -pipe -D_REENTRANT -fPIE $(DEFINES)
LIB :=  -L/usr/X11R6/lib64 -lQt5PrintSupport -L/usr/lib/x86_64-linux-gnu -lQt5Widgets -lQt5Gui -lQt5Core `pkg-config --libs opencv` -lboost_system  -lncurses -lfreenect `sdl2-config --libs` -lpthread -lX11 -lGL
TESTFLAGS := -fPIC -g -pg -fprofile-arcs -ftest-coverage -Wall \
-DNON_MATLAB_PARSING -DMAX_EXT_API_CONNECTIONS=255 -D__linux \
`pkg-config --cflags opencv` `sdl2-config --cflags`
TESTLIB := `pkg-config --libs opencv` -lboost_system  -lncurses -lfreenect \
`sdl2-config --libs` -lpthread -lX11 -lgcov -fprofile-arcs
INC :=  -I/usr/share/qt5/mkspecs/linux-g++-64 -I /usr/include/qt5 -I /usr/include/qt5/QtWidgets \
-I /usr/include/qt5/QtGui -I /usr/include/qt5/QtCore -I include \
-I /usr/include/boost -I /usr/local/include/libfreenect \
-I src/GUI -I /usr/include/qt5/QtPrintSupport
QMAKE := /usr/lib/x86_64-linux-gnu/qt5/bin/qmake

# Directories:
SRCDIR := src
BUILDDIR := build
BINDIR := bin
TESTDIR := test

# Targets:
MAINTARGET := superquad_cascade
MAIN2TARGET := superquad_pid
MODELTESTTARGET := modeltest
KINECTTESTTARGET := kinecttest
KINECTIMAGECAPTURETARGET := kinectimagecapture
TRACKINGTESTTARGET := trackingtest
PIDTESTTARGET := pidtest
CONTROLLERTESTTARGET := controllertest
TESTTARGETS = $(MODELTESTTARGET) $(KINECTTESTTARGET) \
$(KINECTIMAGECAPTURETARGET) $(TRACKINGTESTTARGET) \
$(PIDTESTTARGET) $(CONTROLLERTESTTARGET)

# Option for testing;
ifneq (,$(findstring $(MAKECMDGOALS),$(TESTTARGETS)))
	CFLAGS = $(TESTFLAGS) 
	LIB = $(TESTLIB)
	BUILDDIR := testbuild
endif

# Source files:
SRCEXT := cpp
AUXSOURCES := $(SRCDIR)/Controller/boost_xbox_controller.cpp $(SRCDIR)/Kinect/Kinect.cpp $(SRCDIR)/Kinect/camera.cpp $(SRCDIR)/PhysicsModel/PhysicsModel.cpp  $(SRCDIR)/Tx/Tx.cpp  $(SRCDIR)/PID/PID.cpp $(SRCDIR)/datatypes.cpp $(SRCDIR)/CascadeControl/CascadeControl.cpp $(SRCDIR)/CascadeControl/AccelerationControl.cpp $(SRCDIR)/CascadeControl/VelocityControl.cpp $(SRCDIR)/CascadeControl/PositionControl.cpp $(SRCDIR)/CascadeControl/Control.cpp  $(SRCDIR)/Monitor/monitor.cpp
MAINSOURCES := $(SRCDIR)/SuperQuadCascade.cpp $(AUXSOURCES)
MAIN2SOURCES := $(SRCDIR)/SuperQuadPID.cpp $(AUXSOURCES)
MODELTESTSOURCES :=  $(SRCDIR)/Test/ModelTest.cpp $(AUXSOURCES)
KINECTTESTSOURCES := $(SRCDIR)/Test/KinectTest.cpp $(AUXSOURCES)
KINECTIMAGECAPTURESOURCES := $(SRCDIR)/Test/KinectImageCapture.cpp $(AUXSOURCES)
TRACKINGTESTSOURCES := $(SRCDIR)/Test/TrackingTest.cpp $(AUXSOURCES)
PIDTESTSOURCES := $(SRCDIR)/Test/PIDTest.cpp $(AUXSOURCES)
CONTROLLERTESTSOURCES := $(SRCDIR)/Test/ControllerTest.cpp $(SRCDIR)/PS2ControllerV2/V2.cpp $(AUXSOURCES)

# Object files:
MAINOBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,\
$(MAINSOURCES:.$(SRCEXT)=.o)) $(BUILDDIR)/PhysicsModel/extApi.o  \
	$(BUILDDIR)/PhysicsModel/extApiPlatform.o \
	$(BUILDDIR)/PhysicsModel/extApiCustom.o
MAIN2OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,\
$(MAIN2SOURCES:.$(SRCEXT)=.o)) $(BUILDDIR)/PhysicsModel/extApi.o  \
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
CONTROLLERTESTOBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,\
$(CONTROLLERTESTSOURCES:.$(SRCEXT)=.o)) $(BUILDDIR)/PhysicsModel/extApi.o \
	$(BUILDDIR)/PhysicsModel/extApiPlatform.o \
	$(BUILDDIR)/PhysicsModel/extApiCustom.o

# Rules:
default:
	cd src/GUI && qmake
	cd src/GUI && $(MAKE)
	$(MAKE) $(MAINTARGET)
	$(MAKE) $(MAIN2TARGET)

$(MAINTARGET): $(MAINOBJECTS)
	@mkdir -p $(BINDIR)
	@echo " Linking..."
	@echo " $(CXX) $^ -o $(MAINTARGET) $(LIB)"; $(CXX) $^ \
	$(SRCDIR)/GUI/qcustomplot.o \
	$(SRCDIR)/GUI/moc_qcustomplot.o \
	$(SRCDIR)/GUI/gui_interface.o \
	$(SRCDIR)/GUI/pollthread.o \
	$(SRCDIR)/GUI/moc_gui_interface.o \
	$(SRCDIR)/GUI/moc_pollthread.o \
	-o $(BINDIR)/$(MAINTARGET) $(LIB)

$(MAIN2TARGET): $(MAIN2OBJECTS)
	@mkdir -p $(BINDIR)
	@echo " Linking..."
	@echo " $(CXX) $^ -o $(MAIN2TARGET) $(LIB)"; $(CXX) $^ \
	$(SRCDIR)/GUI/qcustomplot.o \
	$(SRCDIR)/GUI/moc_qcustomplot.o \
	$(SRCDIR)/GUI/gui_interface.o \
	$(SRCDIR)/GUI/pollthread.o \
	$(SRCDIR)/GUI/moc_gui_interface.o \
	$(SRCDIR)/GUI/moc_pollthread.o \
	-o $(BINDIR)/$(MAIN2TARGET) $(LIB)

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

$(CONTROLLERTESTTARGET): $(CONTROLLERTESTOBJECTS)
	@mkdir -p $(TESTDIR)
	@echo " Linking..."
	@echo " $(CXX) $^ -o $(CONTROLLERTESTTARGET) $(LIB)"; $(CXX) $^ -o \
$(TESTDIR)/$(CONTROLLERTESTTARGET) $(LIB)

#cascade: $(SRCDIR)/CascadeControl/CascadeControl.cpp include/CascadeControl/CascadeControl.hpp
#	$$(CXX) $(CFLAGS) $(INC) -o $(BUILDDIR)/CascadeControl/CascadeControl.o -c $(SRCDIR)/CascadeControl/CascadeControl.cpp
#	$(MAKE) superquad

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	@mkdir -p $(BUILDDIR)/PhysicsModel
	@mkdir -p $(BUILDDIR)/Tx
	@mkdir -p $(BUILDDIR)/Kinect
	@mkdir -p $(BUILDDIR)/Controller
	@mkdir -p $(BUILDDIR)/Test
	@mkdir -p $(BUILDDIR)/PID
	@mkdir -p $(BUILDDIR)/CascadeControl
	@mkdir -p $(BUILDDIR)/PS2ControllerV2
	@mkdir -p $(BUILDDIR)/Monitor
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
.FORCE: