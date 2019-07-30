#ifndef VOLTAGEGRAPH_H
#define VOLTAGEGRAPH_H

#include <QWidget>
#include <QVector>

namespace Ui {
class voltageGraph;
}

class voltageGraph : public QWidget
{
    Q_OBJECT

public:
    explicit voltageGraph(QWidget *parent = nullptr);
    ~voltageGraph();
    bool addPoint(double x, double y);
    bool addPoints(QVector<double> &x, QVector<double> &y);
    bool setYMax(double max);
    bool setYMin(double min);
    bool setXMax(double max);
    bool setXMin(double min);

private:
    Ui::voltageGraph *ui;
};

#endif // VOLTAGEGRAPH_H
