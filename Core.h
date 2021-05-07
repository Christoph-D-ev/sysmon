#ifndef CORE_H
#define CORE_H

#include <string>
#include <QtCharts/QtCharts>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <fstream>
#include <QTimer>
#include <iostream>
#include <unistd.h>

class Core
{
public:
    Core(int number,QColor color,QChart *chart,QCategoryAxis *axisX,QCategoryAxis *axisY );
    ~Core();
    QLineSeries *series;
public slots:
    void update_series();

private:


    int hue;
    int number;
    int old_idle;
    int old_sum;
    std::list<int> data_points;

};

#endif // CORE_H
