#ifndef GUI_INTERFACE_H
#define GUI_INTERFACE_H

#include <QMainWindow>
#include "../../include/Monitor/monitor.hpp"
#include "qcustomplot.h"
#include "../../include/datatypes.hpp"
#include "pollthread.h"

//visible range
const int XMIN = -1000;
const int XMAX = 1000;
const int YMIN = -1000;
const int YMAX = 1000;
const int ZMIN = 50;
const int ZMAX = 2500;

namespace Ui {
class GUI_interface;
}


class GUI_interface : public QMainWindow
{

    Q_OBJECT
    
private:
    Ui::GUI_interface *ui;
    Monitor* mon;

    //required for multithreading
    QThread *thread;
    PollThread *poll;

    //style for plot
    QCPScatterStyle locationStyle;
    QCPScatterStyle targetStyle;

    //valid target locations
    KinectFrustum validSpace;

    //vector to hold values in GUI, as monitor doesn't auto-update
    Location targetBuff;


    //plotting functions
    void init_Plot();
    void plot_Location(Location location);
    void plot_Target(Location target);
    void valid_target(bool valid);

public:
    //constructor/destructor
    GUI_interface(Monitor *_mon);
    ~GUI_interface();

private slots:

    //button slots
    void on_ButtonUpdateValues_clicked();
    void on_ButtonLights_toggled(bool checked);
    void on_ButtonLand_clicked();
    void on_ButtonSTOP_clicked();
    void on_AdvCheck_toggled(bool checked);

    //value update slots
    void target_upd_x(int newval);
    void target_upd_y(int newval);
    void target_upd_z(int newval);
    void update_location(Location new_location);

    //Plot mouse event slots
    void plotMousePress(QMouseEvent* event);
    void plotWheel(QWheelEvent *wheelEvent);
};

#endif // GUI_INTERFACE_H
