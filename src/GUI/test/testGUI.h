#ifndef TESTGUI_H
#define TESTGUI_H

#include <QtTest/QtTest>
#include "gui_interface.h"

// Test suite for GUI

class TestGUI : public QObject {
    Q_OBJECT

private slots:


    // functions executed by QtTest before and after test suite
    //void initTestCase();
    //void cleanupTestCase();

    // functions executed by QtTest before and after each test
    //void init();
    //void cleanup();

    // test functions
    void testConnections();
};

#endif
