#include "dragpoint.h"
#include <QtWidgets>
#include <QWidget>
#include <QDebug>

DragPoint::DragPoint(QWidget *parent)
    : QLabel(parent)
{
    QPixmap pixmap(":/images/point.png");
//    pixmap.scaled(30, 30, Qt::IgnoreAspectRatio, Qt::FastTransformation);
    setScaledContents(true);
    setPixmap(pixmap);
    setFixedSize(50, 50);
}

int DragPoint::cx()
{
    return x() + width() / 2;
}
int DragPoint::cy()
{
    return y() + height() / 2;
}

void DragPoint::mousePressEvent(QMouseEvent *event)
{
    QWidget *w = dynamic_cast<QWidget*>(parent());
    if (!w)
        return;
    pos = event->pos() + w->pos();
}

void DragPoint::mouseMoveEvent(QMouseEvent *event)
{
//    qDebug()<<"mouseMoveEvent:"<<event->globalPos()<<event->pos()<<pos;
    move(event->globalPos() - QPoint(7, 29) - pos);
//    move(x(), event->globalY() - 29 - pos.y());
    emit positionChanged();
}
