#ifndef RACQUET_H
#define RACQUET_H

#include <QtWidgets/QGraphicsRectItem>

#include "Setting.h"

class Racquet : public QGraphicsRectItem
{
public:
    Racquet(QGraphicsItem *parent = 0);
    Racquet(const QRectF &rect, QGraphicsItem *parent = 0);
    Racquet(qreal x, qreal y, qreal w, qreal h, QGraphicsItem *parent = 0);

    void setSettng(const Setting &setting);
    Setting setting() const;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    Setting _setting;
};

#endif // RACQUET_H
