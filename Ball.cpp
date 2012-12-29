#include "Ball.h"

#include <QtGui/QPainter>

Ball::Ball(QGraphicsItem *parent) :
    QGraphicsEllipseItem(parent)
{
    _init_();
}

Ball::Ball(const QRectF &rect, QGraphicsItem *parent) :
    QGraphicsEllipseItem(rect, parent)
{
    _init_();
}

Ball::Ball(qreal x, qreal y, qreal w, qreal h, QGraphicsItem *parent) :
    QGraphicsEllipseItem(x, y, w, h, parent)
{
    _init_();
}

void Ball::setSettng(const Setting &setting)
{
    _setting = setting;
}

Setting Ball::setting() const
{
    return _setting;
}

void Ball::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->setPen(QPen(Qt::black, 1));
    painter->setBrush(QBrush(_setting.color()));

    painter->drawEllipse(this->rect());
}

void Ball::setXDirection(qreal xDirection)
{
    if (xDirection == 1 || xDirection == -1) {
        _xDirection = xDirection;
    }
}

qreal Ball::xDirection() const
{
    return _xDirection;
}

void Ball::setYDirection(qreal yDirection)
{
    if (yDirection == 1 || yDirection == -1) {
        _yDirection = yDirection;
    }
}

qreal Ball::yDirection() const
{
    return _yDirection;
}

void Ball::advance(int phase)
{
    if (phase != 0) {
        qreal speed = _setting.speed();
        this->setPos(this->x() + (_xDirection *speed), this->y() + (_yDirection *speed));
    }
}

void Ball::_init_()
{
    _xDirection = 0;
    _yDirection = 0;
}
