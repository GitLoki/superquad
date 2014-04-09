#include "gui_interface.h"
#include <QApplication>
#include <QHBoxLayout>
#include <QLabel>
#include <QSpinBox>
#include <QSlider>
#include <QGroupBox>

int main(int argc, char *argv[])
{
    QApplication quadcop(argc, argv);
    GUI_interface w;

    QApplication prog(argc, argv);

    // generate some data:
    QVector<double> x(101), y(101); // initialize with entries 0..100
    for (int i=0; i<101; ++i)
    {
      x[i] = i/50.0 - 1; // x goes from -1 to 1
      y[i] = x[i]*x[i]; // let's plot a quadratic function
    }

    /*// create graph and assign data to it:
    QGroupBox *XY_diagram = new QGroupBox;

    XY_diagram->addGraph();
    XY_diagram->graph(0)->setData(x, y);
    // give the axes some labels:
    XY_diagram->xAxis->setLabel("x");
    XY_diagram->yAxis->setLabel("y");
    // set axes ranges, so we see all data:
    XY_diagram->xAxis->setRange(-1, 1);
    XY_diagram->yAxis->setRange(0, 1);
    XY_diagram->replot();
    */
    QSlider *ScrollBarX = new QSlider;
    QSpinBox *spinBoxX = new QSpinBox;
    ScrollBarX->setRange(30,200);
    spinBoxX->setRange(30,200);

    QObject::connect(ScrollBarX, SIGNAL(valueChanged(int)), spinBoxX,SLOT(setValue(int)));

    QObject::connect(spinBoxX, SIGNAL(valueChanged(int)), ScrollBarX,SLOT(setValue(int)));
    ScrollBarX->setValue(30);
    spinBoxX->setValue(30);

    w.show();
    
    return quadcop.exec();
}
