#ifndef CORE_H
#define CORE_H

#include <string>
#include <QtCharts/QtCharts>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <fstream>

class core
{
public:
    core();
    ~core();
public slots:
    void update_series();
private:
    std::string name;
    QLineSeries series;
    int hue;
    int number;
    int old_idle;
    int old_sum;
    std::list<int>data_points;

};

#endif // CORE_H
