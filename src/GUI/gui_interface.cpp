#include "gui_interface.h"
#include "ui_gui_interface.h"

GUI_interface::GUI_interface(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GUI_interface)
{
    ui->setupUi(this);
}

GUI_interface::~GUI_interface()
{
    delete ui;
}
