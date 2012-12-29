#ifndef TIMER_H
#define TIMER_H

#include <QtCore/QObject>

#include <QtCore/QTimer>

class Timer : public QObject
{
    Q_OBJECT

public:
    Timer(QObject *parent = 0) :
        QObject(parent), _s(0), _m(0), _h(0)
            { QObject::connect(&_timer, SIGNAL(timeout()), this, SLOT(step())); }

    void start() { _timer.start(1000); }
    void stop() { _timer.stop(); }
    virtual void restart() { _s = 0; _m = 0; _h = 0; }

    unsigned int seconds() const { return _s; }
    unsigned int minutes() const { return _m;}
    unsigned int hours() const { return _h; }

protected slots:
    virtual void step()
    { _s++; this->_check(_s, _m, 60); this->_check(_m, _h, 60); }

private:
    QTimer _timer;

    unsigned int _s;
    unsigned int _m;
    unsigned int _h;

    void _check(unsigned int &a, unsigned int &b, unsigned int i)
    { if (a == i) { a = 0; b++; } }
};

#endif // TIMER_H
