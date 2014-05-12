#ifndef GUI_INTERFACE_H
#define GUI_INTERFACE_H

#include <QMainWindow>
#include <vector>
#include "../../include/Monitor/monitor.hpp"
#include "qcustomplot.h"

namespace Ui {
class GUI_interface;
}

class GUI_interface : public QMainWindow
{
    Q_OBJECT
    
private:
    Ui::GUI_interface *ui;
    Monitor* mon;
    QCPScatterStyle locationStyle;
    QCPScatterStyle targetStyle;

    //vector to hold values in GUI, as monitor doesn't auto-update
    std::vector<float> target_buff;

    //plotting functions
    void init_Plot();
    void plot_Location(std::vector<float> location);
    void plot_Target(std::vector<float> target);

public:
    GUI_interface(Monitor *_mon);
    ~GUI_interface();

private slots:

    //button slots
    void on_ButtonUpdateValues_clicked();
    void on_ButtonLights_clicked();
    void on_ButtonLand_clicked();
    void on_ButtonSTOP_clicked();

    //value update slots
    void target_upd_x(int newval);
    void target_upd_y(int newval);
    void target_upd_z(int newval);
    void updateLoc(std::vector<float> new_location);

    //Plot mouse event slots
    void plotMousePress(QMouseEvent* event);
    void plotWheel(QWheelEvent *wheelEvent);

};

#endif // GUI_INTERFACE_H
