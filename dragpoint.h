#ifndef DRAGPOINT_H
#define DRAGPOINT_H

#include <QLabel>


class DragPoint : public QLabel
{
    Q_OBJECT
public:
    DragPoint(QWidget * parent = 0);

    int cx(); //central x
    int cy(); //central y

signals:
    void positionChanged();
protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    QPoint pos;
};

#endif // DRAGPOINT_H
