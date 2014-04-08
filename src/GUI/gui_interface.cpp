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

    QThread *thread = new QThread(this);
    PollThread *poll = new PollThread(mon);
    poll->moveToThread(thread);

    connect(thread, SIGNAL(started()), poll, SLOT(poll()));
    connect(poll, SIGNAL(setNewLoc(vector<float>)), this, SLOT(updateLoc(vector<float>)));
    connect(thread, SIGNAL(destroyed()), poll, SLOT(deleteLater()));

    thread->start();

}

GUI_interface::~GUI_interface()
{
    delete ui;
}


void GUI_interface::updateLoc(std::vector<float> new_location){

}
