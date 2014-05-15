#ifndef POLLTHREAD_H
#define POLLTHREAD_H

#include "../../include/Monitor/monitor.hpp"
#include <QObject>
#include "../../include/datatypes.hpp"

//A thread that continuously polls monitor to get new object location

class PollThread : public QObject
{
    Q_OBJECT

private:
    Monitor* mon;

    void delay(int ms);

public slots:
    void poll();

public:
    PollThread(Monitor* _mon);

signals:
    void set_new_loc(Location newLoc);
};

#endif // POLLTHREAD_H
