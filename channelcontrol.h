#ifndef VOLTAGEGRAPH_H
#define VOLTAGEGRAPH_H

#include <QWidget>
#include <QVector>

namespace Ui {
class channelControl;
}

class channelControl : public QWidget
{
    Q_OBJECT

public:
    explicit channelControl(char inChannelIndex = 0 , QWidget *parent = nullptr);
    ~channelControl();
    bool addPoint(double x, double y);
    bool addPoints(QVector<double> &x, QVector<double> &y);
    void clearAxes(void);
    bool setYMax(double max);
    bool setYMin(double min);
    bool setXMax(double max);
    bool setXMin(double min);

private:
    char channelIndex;
    double xCnt;

private slots:
    void on_pushButtonClearAxes_clicked();
    void on_pushButtonSetAxesRanges_clicked();
    void on_pushButtonStop_clicked();
    void on_pushButtonStartClockWise_clicked();
    void on_pushButtonStartContrClockWise_clicked();

private:
    Ui::channelControl *ui;

signals:
    void stop(char channelIndex);
    void startClockWise(char channelIndex);
    void startCounterClockwise(char channelIndex);

};

#endif // VOLTAGEGRAPH_H
