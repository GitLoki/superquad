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

    ui->positionX->setNum(new_location[0]);
    ui->positionY->setNum(new_location[1]);
    ui->positionZ->setNum(new_location[2]);
}

void GUI_interface::on_ButtonUpdateValeus_clicked()
{

    std::vector<float> target(3);
    target[0] = (float) ui->spinBoxX->value();
    target[1] = (float) ui->spinBoxY->value();
    target[2] = (float) ui->spinBoxZ->value();

    mon->set_target(target);

}

void GUI_interface::on_ButtonLights_clicked()
{
    mon->lightswitch();
}
