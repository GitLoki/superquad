#ifndef POLLTHREAD_H
#define POLLTHREAD_H

#include "../../include/Monitor/monitor.hpp"
#include <QObject>
#include <vector>

Q_DECLARE_METATYPE(std::vector<float>);

class PollThread : public QObject
{
    Q_OBJECT

private:

    Monitor* mon;

public slots:
    void poll();

public:
    PollThread(Monitor* _mon);

signals:
    void setNewLoc(std::vector<float> new_loc);
};

#endif // POLLTHREAD_H
