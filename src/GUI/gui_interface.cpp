#include "gui_interface.h"
#include "ui_gui_interface.h"

#include "pollthread.h"
#include <QThread>
#include <QDebug>


//constructor
GUI_interface::GUI_interface(Monitor *_mon) :
    QMainWindow(),
    ui(new Ui::GUI_interface),
    mon(_mon),
    valid_space(ZMIN, ZMAX)
{
    ui->setupUi(this);

    ui->spinBoxX->setMinimum(XMIN);
    ui->spinBoxX->setMaximum(XMAX);
    ui->spinBoxY->setMinimum(YMIN);
    ui->spinBoxY->setMaximum(YMAX);
    ui->spinBoxZ->setMinimum(ZMIN);
    ui->spinBoxZ->setMaximum(ZMAX);

    ui->scrollBarX->setMinimum(XMIN);
    ui->scrollBarX->setMaximum(XMAX);
    ui->scrollBarY->setMinimum(YMIN);
    ui->scrollBarY->setMaximum(YMAX);
    ui->scrollBarZ->setMinimum(ZMIN);
    ui->scrollBarZ->setMaximum(ZMAX);

    qRegisterMetaType<Location>("Location");

    ui->WarnLabel->setStyleSheet("QLabel { color : red; }");

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
    connect(poll, SIGNAL(setNewLoc(Location)), this, SLOT(updateLoc(Location)));
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
void GUI_interface::updateLoc(Location new_location)
{
    //update text boxes on GUI
    ui->positionX->setNum(new_location.X);
    ui->positionY->setNum(new_location.Y);
    ui->positionZ->setNum(new_location.Z);

    //refresh location plot
    plot_Location(new_location);
}

//SLOT: "Update Values" clicked
void GUI_interface::on_ButtonUpdateValues_clicked()
{
    //update target values in monitor
    Location target;
    target.X = (float) ui->spinBoxX->value();
    target.Y = (float) ui->spinBoxY->value();
    target.Z = (float) ui->spinBoxZ->value();

    mon->set_target(target);

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

//SLOT: "Lights" clicked
void GUI_interface::on_ButtonLights_toggled(bool checked)
{
    mon->lightswitch(checked);

}

//SLOT: "Advanced mode" clicked
void GUI_interface::on_AdvCheck_toggled(bool checked)
{
    mon->snap(!checked);
}

//SLOT: target x value changed
void GUI_interface::target_upd_x(int newval)
{
    target_buff.X = newval;
    plot_Target(target_buff);

    valid_warning(!valid_space.inside(target_buff));
}

//SLOT: target y value changed
void GUI_interface::target_upd_y(int newval)
{
    target_buff.Y = newval;
    plot_Target(target_buff);

    valid_warning(!valid_space.inside(target_buff));
}

//SLOT: target z value changed
void GUI_interface::target_upd_z(int newval)
{
    target_buff.Z = newval;
    plot_Target(target_buff);

    valid_warning(!valid_space.inside(target_buff));
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
    ui->Plot->xAxis->setRange(ui->spinBoxX->minimum(), ui->spinBoxX->maximum());
    ui->Plot->yAxis->setRange(ui->spinBoxY->minimum(), ui->spinBoxY->maximum());

    //set plot style
    locationStyle.setShape(QCPScatterStyle::ssDisc);
    locationStyle.setPen(QPen(Qt::blue));

    targetStyle.setShape(QCPScatterStyle::ssDisc);
    targetStyle.setPen(QPen(Qt::red));

    //pass location and target into plot
    Location target;
    Location location;
    mon->get_target(target);
    mon->get_location(location);
    plot_Target(target);
    plot_Location(location);
}

//Update the displayed target on the plot
void GUI_interface::plot_Target(Location target)
{
    //setData requires a vector even for a single point
    QVector<double> x(1), y(1);
    x[0] = target.X;
    y[0] = target.Y;

    //set size based on distance
    double size = std::max(20-(double)target.Z/150,3.0);
    targetStyle.setSize(size);

    //set the data and style, then refresh the plot
    ui->Plot->graph(0)->setData(x, y);
    ui->Plot->graph(0)->setScatterStyle(targetStyle);

    ui->Plot->replot();
}

//Update the displayed location on the plot
void GUI_interface::plot_Location(Location location)
{

    //setData requires a vector even for a single point
    QVector<double> x(1), y(1);
    x[0] = location.X;
    y[0] = location.Y;

    //set size based on distance
    double size = std::max(20-(double)location.Z/150,3.0);
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

//Ensure that target is visible by kinect
void GUI_interface::valid_warning(bool warn)
{
    if(warn)
        ui->WarnLabel->setText("WARNING: INVALID TARGET");
    else
        ui->WarnLabel->setText("");
}
