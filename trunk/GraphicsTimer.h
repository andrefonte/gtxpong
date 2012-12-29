#ifndef GRAPHICSTIMER_H
#define GRAPHICSTIMER_H

#include "Timer.h"

#include <QtWidgets/QGraphicsSimpleTextItem>

class GraphicsTimer : public Timer
{
    Q_OBJECT

public:
    GraphicsTimer(QObject *parent = 0) :
        Timer(parent), _s(true), _m(true), _h(true), _text(0)
    {
    }

    void setTextItem(QGraphicsSimpleTextItem *item)
    {
        _text = item;
    }

    QGraphicsSimpleTextItem *textItem() const
    {
        return _text;
    }

    void setFomat(bool s = true, bool m = true, bool h = true)
    {
        _s = s; _m = m; _h = h;
    }

    void restart()
    {
        Timer::restart();
        this->_updateTextItem();
    }

protected slots:
    void step()
    {
        Timer::step(); this->_updateTextItem();
    }

private:
    bool _s;
    bool _m;
    bool _h;

    QGraphicsSimpleTextItem *_text;

    void _updateTextItem()
    {
        QString data;

        if (_h == true) {
            if (this->hours() < 10) { data.append("0"); }
            data.append(QString().setNum(this->hours()));
            if (_m == true || _s == true) { data.append(":"); }
        }

        if (_m == true) {
            if (this->minutes() < 10) { data.append("0"); }
            data.append(QString().setNum(this->minutes()));
        }

        if (_h == true || _m == true) { data.append(":"); }

        if (_s == true) {
            if (this->seconds() < 10) { data.append("0"); }
            data.append(QString().setNum(this->seconds()));
        }

        if (_text != 0) { _text->setText(data); }
    }
};

#endif // GRAPHICSTIMER_H
