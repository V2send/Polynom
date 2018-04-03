#include "dialog.h"
#include "ui_dialog.h"
#include <QLabel>
#include "dragpoint.h"
#include <QPainter>
#include <QDebug>
#include <time.h>
#include <QPaintEvent>

const int pointCount = 5;

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog),
    k(pointCount),
    points(pointCount)
{
    ui->setupUi(this);

//    QLabel *point = new QLabel(this);
//    point->setText("POINTtttttttttttttttttttttttt");
////    point->setPixmap(QPixmap(":/images/point.png"));
////    point->setMinimumSize(30, 30);
//    point->move(100, 50);
//    point->show();
    srand (time(NULL));
    for (int i = 0; i < pointCount; ++i) {
        DragPoint *point = new DragPoint(this);
        point->move(125 + i * 100, rand() % (size().height() - 200) + 100);
        points[i] = point;
        connect(points[i], SIGNAL(positionChanged()), this, SLOT(calcAll()));
    }
    calcAll();

    std::vector<std::vector<double>> m =  {{4, 3, 7, 4},
                                         {10, -5, 2, 3},
                                         {-3, 7, 6, 8},
                                         {2, 8, 9, -4}};
    qDebug()<<"determinator:"<<getDeterminant(m, 4);
    qDebug()<<"det2:"<<getDeterminant(replaseColumn(m, {2, -8, 7, 5}, 0), 4);
}

Dialog::~Dialog()
{
    for (auto p: points)
        delete p;
    delete ui;
}

void Dialog::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    int bottom = size().height() - 50;
    int left = 50;
    painter.setPen(QPen(Qt::black, 4));
    painter.drawLine(left, 20, left, size().height() - 20);
    painter.drawLine(left - 30, bottom, size().width() - 20, bottom);

    for (int i = 1; i <= pointCount + 1; ++i) {
        painter.drawLine(left + 100 * i, bottom - 10, left + 100 * i, bottom + 10);
    }
    drawLagrange(painter);
    drawLeastSquares(painter);
}

void Dialog::calcAll()
{
    calcCoefficients();
    calcLeastSquares();
    update();
}

void Dialog::calcCoefficients()
{
    for (int i = 0; i < pointCount; ++i) {
        k[i] = points[i]->cy();
        for (int j = 0; j < pointCount; ++j) {
            if (i != j)
                k[i] /= points[i]->cx() - points[j]->cx();
        }
    }
}

void Dialog::drawLagrange(QPainter& painter)
{
    painter.setPen(QPen(Qt::blue, 2));
    double prevX = 0;
    double prevY = 0;
    for (int x = 50; x < 650; ++x) {
        double y = 0;
        for (int i = 0; i < pointCount; ++i) {
            double t = 1;
            for (int j = 0; j < pointCount; ++j) {
                if (i != j) {
                    t *= x - points[j]->cx();
                }
            }
            y += t * k[i];
        }
        if (x != 50) {
            painter.drawLine(prevX, prevY, x, y);
        }
        prevX = x;
        prevY = y;
    }
}

void Dialog::calcLeastSquares()
{
//    std::vector<std::vector<double>> matrix(4);
//    std::vector<double> b(4);
//    for (int i = 0; i < 4; ++i)
//        matrix[i].resize(4);
    int degree = 4;

    double sum_x1 = 0;
    double sum_x2 = 0;
    double sum_x3 = 0;
    double sum_x4 = 0;
    double sum_x5 = 0;
    double sum_x6 = 0;
    double sum_y = 0;
    double sum_yx1 = 0;
    double sum_yx2 = 0;
    double sum_yx3 = 0;
    for (auto point: points) {
        double x = point->x();
        double y = point->y();

        double x2 = x * x;
        double x3 = x * x2;
        sum_x1 += x;
        sum_x2 += x2;
        sum_x3 += x3;
        sum_x4 += x3 * x;
        sum_x5 += x3 * x2;
        sum_x6 += x3 * x3;

        sum_y += y;
        sum_yx1 += y * x;
        sum_yx2 += y * x2;
        sum_yx3 += y * x3;
    }
    std::vector<std::vector<double>> matrix = {{   3.0, sum_x1, sum_x2, sum_x3},
                                               {sum_x1, sum_x2, sum_x3, sum_x4},
                                               {sum_x2, sum_x3, sum_x4, sum_x5},
                                               {sum_x3, sum_x4, sum_x5, sum_x6}};
    std::vector<double> b = {sum_y, sum_yx1, sum_yx2, sum_yx3};

    double det = getDeterminant(matrix, degree);
    lsValues.resize(degree);
    for (int i = 0; i < degree; ++i) {
        double detB = getDeterminant(replaseColumn(matrix, b, i), degree);
        lsValues[i] = detB / det;
    }
}

void Dialog::drawLeastSquares(QPainter &painter)
{
    painter.setPen(QPen(Qt::red, 2));
    double prevX = 0;
    double prevY = 0;
    for (int x = 50; x < 650; ++x) {
        double x2 = x * x;
        double x3 = x * x2;

        double y = lsValues[0] + lsValues[1] * x + lsValues [2] * x2 + lsValues[3] * x3;
        if (x != 50) {
            painter.drawLine(prevX, prevY, x, y);
        }
        prevX = x;
        prevY = y;
    }
}

double Dialog::getDeterminant(const std::vector<std::vector<double>>& matrix, int m_size)
{
    int c, subi, i, j, subj;
    double d = 0;
    std::vector<std::vector<double>> submat(m_size - 1);
    for (c = 0; c < m_size - 1; ++c)
        submat[c].resize(m_size - 1);
//    qDebug()<<"###";
    if (m_size == 2)
        return (matrix[0][0] * matrix[1][1]) - (matrix[1][0] * matrix[0][1]);
    else
    {
        for(c = 0; c < m_size; c++)
        {
//            qDebug()<<"c:"<<c;
            subi = 0;
            for(i = 1; i < m_size; i++)
            {
//                qDebug()<<"i:"<<i;
                subj = 0;
                for(j = 0; j < m_size; j++)
                {
//                    qDebug()<<"j:"<<j;
                    if (j == c)
                        continue;
//                    qDebug()<<"subi:"<<subi<<"subj:"<<subj<<"size:"<<submat.size();
                    if (submat.size() >= subi && submat[subi].size() >= subj)
                        submat[subi][subj] = matrix[i][j];
                    subj++;
                }
                subi++;
            }
            d = d + (pow(-1 ,c) * matrix[0][c] * getDeterminant(submat, m_size - 1));
        }
    }
    return d;
}

std::vector<std::vector<double> > Dialog::replaseColumn(std::vector<std::vector<double> > matrix, const std::vector<double> &col, int num)
{
    if (matrix.size() == col.size())
        for (int i = 0; i < matrix.size(); ++i) {
            matrix[i][num] = col[i];
        }
    return matrix;
}

















