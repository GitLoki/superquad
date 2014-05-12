#include "gui_interface.h"
#include "ui_gui_interface.h"

#include "pollthread.h"
#include <QThread>
#include <QDebug>


//constructor
GUI_interface::GUI_interface(Monitor *_mon) :
    mon(_mon),
    target_buff(3,0),
    QMainWindow(),
    ui(new Ui::GUI_interface)
{
    ui->setupUi(this);

    qRegisterMetaType<std::vector<float> >("stdvec");

    //connect scrollbars and spinboxes
    connect(ui->scrollBarX,SIGNAL(valueChanged(int)),ui->spinBoxX,SLOT(setValue(int)));
    connect(ui->scrollBarY,SIGNAL(valueChanged(int)),ui->spinBoxY,SLOT(setValue(int)));
    connect(ui->scrollBarZ,SIGNAL(valueChanged(int)),ui->spinBoxZ,SLOT(setValue(int)));

    connect(ui->spinBoxX,SIGNAL(valueChanged(int)),ui->scrollBarX,SLOT(setValue(int)));
    connect(ui->spinBoxY,SIGNAL(valueChanged(int)),ui->scrollBarY,SLOT(setValue(int)));
    connect(ui->spinBoxZ,SIGNAL(valueChanged(int)),ui->scrollBarZ,SLOT(setValue(int)));

    //update target buffer when target is moved
    connect(ui->scrollBarX,SIGNAL(valueChanged(int)),this,SLOT(target_upd_x(int)));
    connect(ui->scrollBarY,SIGNAL(valueChanged(int)),this,SLOT(target_upd_y(int)));
    connect(ui->scrollBarZ,SIGNAL(valueChanged(int)),this,SLOT(target_upd_z(int)));

    //buttons
    connect(ui->quitButton,SIGNAL(clicked()),qApp,SLOT(quit()));

    //connect clicking on plot
    connect(ui->Plot, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(plotMousePress(QMouseEvent*)));

    //connect mousewheel to z
    connect(ui->Plot, SIGNAL(mouseWheel(QWheelEvent*)), this, SLOT(plotWheel(QWheelEvent*)));

    //intialise the plot
    init_Plot();

    //set up polling thread
    QThread *thread = new QThread(this);
    PollThread *poll = new PollThread(mon);
    poll->moveToThread(thread);

    connect(thread, SIGNAL(started()), poll, SLOT(poll()));
    connect(poll, SIGNAL(setNewLoc(stdvec)), this, SLOT(updateLoc(stdvec)));
    connect(thread, SIGNAL(destroyed()), poll, SLOT(deleteLater()));

    thread->start();

}

//destructor
GUI_interface::~GUI_interface()
{
    delete ui;
}



/*****************
 *-----SLOTS-----*
 *****************/

//SLOT: update current quad location on display
void GUI_interface::updateLoc(std::vector<float> new_location)
{
    //update text boxes on GUI
    ui->positionX->setNum(new_location[0]);
    ui->positionY->setNum(new_location[1]);
    ui->positionZ->setNum(new_location[2]);

    //refresh location plot
    plot_Location(new_location);
}

//SLOT: "Update Values" clicked
void GUI_interface::on_ButtonUpdateValues_clicked()
{
    //update target values in monitor
    std::vector<float> target(3);
    target[0] = (float) ui->spinBoxX->value();
    target[1] = (float) ui->spinBoxY->value();
    target[2] = (float) ui->spinBoxZ->value();

    mon->set_target(target);
}

//SLOT: "Lights" clicked
void GUI_interface::on_ButtonLights_clicked()
{
    mon->lightswitch();
}

//SLOT: "Land" clicked
void GUI_interface::on_ButtonLand_clicked()
{
    mon->land();
}

//SLOT: "EMERGENCY STOP" clicked
void GUI_interface::on_ButtonSTOP_clicked()
{
    mon->stop();
}

//SLOT: target x value changed
void GUI_interface::target_upd_x(int newval)
{
    target_buff[0] = newval;
    plot_Target(target_buff);
}

//SLOT: target y value changed
void GUI_interface::target_upd_y(int newval)
{
    target_buff[1] = newval;
    plot_Target(target_buff);
}

//SLOT: target z value changed
void GUI_interface::target_upd_z(int newval)
{
    target_buff[2] = newval;
    plot_Target(target_buff);
}



/*************************
 *-----PLOT & TARGET-----*
 *************************/

//Initialise 2D plot
void GUI_interface::init_Plot()
{
    //add graph to plot
    ui->Plot->addGraph();
    ui->Plot->addGraph();

    //do not show lines
    ui->Plot->graph(0)->setLineStyle(QCPGraph::lsNone);
    ui->Plot->graph(1)->setLineStyle(QCPGraph::lsNone);

    //set range
    ui->Plot->xAxis->setRange(-3000, 3000);
    ui->Plot->yAxis->setRange(-3000, 3000);

    //set plot style
    locationStyle.setShape(QCPScatterStyle::ssDisc);
    locationStyle.setPen(QPen(Qt::blue));

    targetStyle.setShape(QCPScatterStyle::ssDisc);
    targetStyle.setPen(QPen(Qt::red));

    //pass location and target into plot
    std::vector<float> target(3);
    std::vector<float> location(3);
    mon->get_target(target);
    mon->get_location(location);
    plot_Target(target);
    plot_Location(location);
}

//Update the displayed target on the plot
void GUI_interface::plot_Target(std::vector<float> target)
{
    //setData requires a vector even for a single point
    QVector<double> x(1), y(1);
    x[0] = target[0];
    y[0] = target[1];

    //set size based on distance
    double size = std::max(20-(double)target[2]/150,3.0);
    targetStyle.setSize(size);

    //set the data and style, then refresh the plot
    ui->Plot->graph(0)->setData(x, y);
    ui->Plot->graph(0)->setScatterStyle(targetStyle);

    ui->Plot->replot();
}

//Update the displayed location on the plot
void GUI_interface::plot_Location(std::vector<float> location){

    //setData requires a vector even for a single point
    QVector<double> x(1), y(1);
    x[0] = location[0];
    y[0] = location[1];

    //set size based on distance
    double size = std::max(20-(double)location[2]/150,3.0);
    locationStyle.setSize(size);

    //set the data and style, then refresh the plot
    ui->Plot->graph(1)->setData(x, y);
    ui->Plot->graph(1)->setScatterStyle(locationStyle);

    ui->Plot->replot();
}

//Update target when plot is clicked
void GUI_interface::plotMousePress(QMouseEvent* event)
{
    ui->spinBoxX->setValue(ui->Plot->xAxis->pixelToCoord(event->x()));
    ui->spinBoxY->setValue(ui->Plot->yAxis->pixelToCoord(event->y()));
}

//target zooming using mouse wheel on plot
void GUI_interface::plotWheel(QWheelEvent *wheelEvent)
{
    //update spinbox value
    ui->spinBoxZ->setValue(ui->spinBoxZ->value() - wheelEvent->delta());

    //ensure that within limits
    if(ui->spinBoxZ->value() > ui->spinBoxZ->maximum())
        ui->spinBoxZ->setValue(ui->spinBoxZ->maximum());
    else if(ui->spinBoxZ->value() < ui->spinBoxZ->minimum())
        ui->spinBoxZ->setValue(ui->spinBoxZ->minimum());
}
