#ifndef GRAPHICSCOUNTER_H
#define GRAPHICSCOUNTER_H

#include "Counter.h"

#include <QtWidgets/QGraphicsSimpleTextItem>

class GraphicsCounter : public Counter
{
    Q_OBJECT

public:
    GraphicsCounter(int initValue = 0, int endValue = 0, QObject *parent = 0) :
        Counter(initValue, endValue, parent), _text(0) { }

    void setTextItem(QGraphicsSimpleTextItem *item) { _text = item; }
    QGraphicsSimpleTextItem *textItem() const { return _text; }

    void setValue(int newValue) { Counter::setValue(newValue); this->_valueToText(); }

public slots:
    void increase() { Counter::increase(); this->_valueToText(); }
    void decrease() { Counter::decrease(); this->_valueToText(); }

private:
    QGraphicsSimpleTextItem *_text;

    void _valueToText() { if (_text != 0) { _text->setText(QString().setNum(this->value())); } }
};

#endif // GRAPHICSCOUNTER_H
