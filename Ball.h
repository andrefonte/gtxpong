#ifndef GRAPHICSITEM_H
#define GRAPHICSITEM_H

#include <QtWidgets/QGraphicsEllipseItem>

#include "Setting.h"

class Ball : public QGraphicsEllipseItem
{
public:
    Ball(QGraphicsItem *parent = 0);
    Ball(const QRectF &rect, QGraphicsItem *parent = 0);
    Ball(qreal x, qreal y, qreal w, qreal h, QGraphicsItem *parent = 0);

    void setSettng(const Setting &setting);
    Setting setting() const;

    void setXDirection(qreal xDirection);
    qreal xDirection() const;

    void setYDirection(qreal yDirection);
    qreal yDirection() const;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

protected:
    void advance(int phase);

private:
    Setting _setting;
    qreal _xDirection;
    qreal _yDirection;

    void _init_();
};

#endif // GRAPHICSITEM_H
