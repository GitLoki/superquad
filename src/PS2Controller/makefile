ps2: ../../include/Tx/Tx.hpp ../../src/Tx/Tx.cpp PS2ControllerMainV2.cpp Controller.o
	g++ -Wall ../../src/Tx/Tx.cpp PS2ControllerMainV2.cpp -I/usr/include/boost -lncurses -lboost_system Controller.o

Controller.o: V2.cpp V2.hpp
	g++ -Wall V2.cpp -c
