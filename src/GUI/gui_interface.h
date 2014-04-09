#ifndef GUI_INTERFACE_H
#define GUI_INTERFACE_H

#include <QWidget>

namespace Ui {
class GUI_interface;
}

class GUI_interface : public QWidget
{
    Q_OBJECT
    
public:
    explicit GUI_interface(QWidget *parent = 0);
    ~GUI_interface();
    
private:
    Ui::GUI_interface *ui;
};

#endif // GUI_INTERFACE_H
