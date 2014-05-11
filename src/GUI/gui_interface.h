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
    
public:
    //explicit GUI_interface(QWidget *parent = 0);
    GUI_interface(Monitor *_mon);
    ~GUI_interface();

private slots:
    void updateLoc(std::vector<float> new_location);
    void on_ButtonUpdateValues_clicked();
    void on_ButtonLights_clicked();

private:
    Ui::GUI_interface *ui;
    Monitor* mon;
    QCPScatterStyle plotStyle;

    void init_Plot();
    void refresh_Plot(std::vector<float> location);
};

#endif // GUI_INTERFACE_H
