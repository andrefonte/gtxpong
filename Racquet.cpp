#include "Racquet.h"

#include <QtGui/QPainter>

Racquet::Racquet(QGraphicsItem *parent) :
    QGraphicsRectItem(parent)
{
}

Racquet::Racquet(const QRectF &rect, QGraphicsItem *parent) :
    QGraphicsRectItem(rect, parent)
{
}

Racquet::Racquet(qreal x, qreal y, qreal w, qreal h, QGraphicsItem *parent) :
    QGraphicsRectItem(x, y, w, h, parent)
{
}

void Racquet::setSettng(const Setting &setting)
{
    _setting = setting;
}

Setting Racquet::setting() const
{
    return _setting;
}

void Racquet::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->setPen(QPen(Qt::black, 1));
    painter->setBrush(QBrush(_setting.color()));

    painter->drawRect(this->rect());
}
