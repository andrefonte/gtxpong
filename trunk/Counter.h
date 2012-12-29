#ifndef COUNTER_H
#define COUNTER_H

#include <QtCore/QObject>

class Counter : public QObject
{
    Q_OBJECT

public:
    Counter(int initValue = 0, int endValue = 0, QObject *parent = 0) :
        QObject(parent), _value(initValue), _endValue(endValue) { }

    virtual void setValue(int newValue) { _value = newValue; }
    int value() const { return _value; }

    void setEndValue(int value) { _endValue = value; }
    int endValue() const { return _endValue; }

    bool isEnd() { if (_value == _endValue) { return true; } return false;}

public slots:
    virtual void increase() { _value++; this->_checkValue(); }
    virtual void decrease() { _value--; this->_checkValue(); }

signals:
    void gotEndValue();

private:
    int _value;
    int _endValue;

    void _checkValue() { if(_value == _endValue) { emit gotEndValue(); } }
};

#endif // COUNTER_H
