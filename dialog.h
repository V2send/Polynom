#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QList>
#include <vector>

namespace Ui {
class Dialog;
}

class DragPoint;

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

protected:
    void paintEvent(QPaintEvent* event);

private slots:
    void calcAll();

private:
    void calcCoefficients();
    void drawLagrange(QPainter &painter);
    void calcLeastSquares();
    void drawLeastSquares(QPainter &painter);

    double getDeterminant(const std::vector<std::vector<double>>& matrix, int m_size);
    std::vector<std::vector<double>> replaseColumn(std::vector<std::vector<double>> matrix, const std::vector<double>& col, int num);

    Ui::Dialog *ui;
    std::vector<DragPoint*> points;
    std::vector<double> k;
    std::vector<double> lsValues;
};

#endif // DIALOG_H
