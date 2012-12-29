#ifndef SETTING_H
#define SETTING_H

#include <QtCore/QtGlobal>
#include <QtGui/QColor>

struct Setting
{
    Setting()
    {
        _speed = 1;
        _color = Qt::black;
    }

    Setting(qreal speed, const QColor &color)
    {
        if (_isValidSpeed(speed) == true) {
            _speed = speed;
        }

        _color = color;
    }

    void setSpeed(qreal speed)
    {
        if (_isValidSpeed(speed) == true) {
            _speed = speed;
        }
    }

    qreal speed() const
    {
        return _speed;
    }

    void setColor(const QColor &color)
    {
        _color = color;
    }

    QColor color() const
    {
        return _color;
    }

private:
    qreal _speed;
    QColor _color;

    bool _isValidSpeed(qreal speed)
    {
        if (speed > 0) {
            return true;
        }

        return false;
    }
};

#endif // SETTING_H
