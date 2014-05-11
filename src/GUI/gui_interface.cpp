#include "gui_interface.h"
#include "ui_gui_interface.h"

#include "pollthread.h"
#include <QThread>
#include <QDebug>


/*
GUI_interface::GUI_interface(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GUI_interface)
{
    ui->setupUi(this);
}
*/

GUI_interface::GUI_interface(Monitor *_mon) :
    mon(_mon),
    QMainWindow(),
    ui(new Ui::GUI_interface)
{
    ui->setupUi(this);

    qRegisterMetaType<std::vector<float> >("stdvec");

    connect(ui->scrollBarX,SIGNAL(valueChanged(int)),ui->spinBoxX,SLOT(setValue(int)));
    connect(ui->scrollBarY,SIGNAL(valueChanged(int)),ui->spinBoxY,SLOT(setValue(int)));
    connect(ui->scrollBarZ,SIGNAL(valueChanged(int)),ui->spinBoxZ,SLOT(setValue(int)));

    connect(ui->spinBoxX,SIGNAL(valueChanged(int)),ui->scrollBarX,SLOT(setValue(int)));
    connect(ui->spinBoxY,SIGNAL(valueChanged(int)),ui->scrollBarY,SLOT(setValue(int)));
    connect(ui->spinBoxZ,SIGNAL(valueChanged(int)),ui->scrollBarZ,SLOT(setValue(int)));

    connect(ui->quitButton,SIGNAL(clicked()),qApp,SLOT(quit()));

    init_Plot();

    QThread *thread = new QThread(this);
    PollThread *poll = new PollThread(mon);
    poll->moveToThread(thread);

    connect(thread, SIGNAL(started()), poll, SLOT(poll()));
    connect(poll, SIGNAL(setNewLoc(stdvec)), this, SLOT(updateLoc(stdvec)));
    connect(thread, SIGNAL(destroyed()), poll, SLOT(deleteLater()));

    thread->start();

}

GUI_interface::~GUI_interface()
{
    delete ui;
}


void GUI_interface::updateLoc(std::vector<float> new_location){

    //update text boxes on GUI
    ui->positionX->setNum(new_location[0]);
    ui->positionY->setNum(new_location[1]);
    ui->positionZ->setNum(new_location[2]);

    //refresh location plot
    refresh_Plot(new_location);
}

void GUI_interface::on_ButtonUpdateValues_clicked()
{

    //update target values in monitor
    std::vector<float> target(3);
    target[0] = (float) ui->spinBoxX->value();
    target[1] = (float) ui->spinBoxY->value();
    target[2] = (float) ui->spinBoxZ->value();

    mon->set_target(target);

    //FOR TESTING ONLY - DELETE LATER
    mon->set_location(target);
    //FOR TESTING ONLY - DELETE LATER

}

void GUI_interface::on_ButtonLights_clicked()
{
    mon->lightswitch();
}

void GUI_interface::init_Plot(){

    //add graph to plot
    ui->Plot->addGraph();

    //do not show lines
    ui->Plot->graph(0)->setLineStyle(QCPGraph::lsNone);

    //set range
    ui->Plot->xAxis->setRange(-3000, 3000);
    ui->Plot->yAxis->setRange(-3000, 3000);

    //set plot style
    plotStyle.setShape(QCPScatterStyle::ssDisc);
    plotStyle.setBrush(Qt::white);

    //pass location into plot
    std::vector<float> location(3);
    mon->get_location(location);
    refresh_Plot(location);
}

void GUI_interface::refresh_Plot(std::vector<float> location){

    //setData requires a vector even for a single point
    QVector<double> x(1), y(1);
    x[0] = location[0];
    y[0] = location[1];

    //set size based on distance
    double size = std::max(23-(double)location[2]/100,3.0);
    plotStyle.setSize(size);

    //set the data and style, then refresh the plot
    ui->Plot->graph(0)->setData(x, y);
    ui->Plot->graph(0)->setScatterStyle(plotStyle);
    ui->Plot->replot();
}
