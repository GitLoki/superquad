#include <QtTest/QtTest>
#include "testGUI.h"

int main(int argc, char** argv) {

    TestGUI testGUI;

    // multiple test suites can be ran like this
    return QTest::qExec(&testGUI, argc, argv);
}
