#include "gui_interface.h"
#include <QApplication>
#include <QHBoxLayout>
#include <QLabel>
#include <QSpinBox>
#include <QSlider>
#include <QGroupBox>

#include "pollthread.h"
#include "../../include/Monitor/monitor.hpp"

int main(int argc, char *argv[])
{
    Monitor* mon = new Monitor;

    QApplication quadcop(argc, argv);
    GUI_interface w(mon);

    w.show();
    
    return quadcop.exec();
}
