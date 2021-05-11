#ifndef CORE_H
#define CORE_H
#include <QObject>
#include <string>
#include <QtCharts/QtCharts>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <fstream>
#include <QTimer>
#include <iostream>
#include <unistd.h>

QT_BEGIN_NAMESPACE
namespace Ui { class Core; }
QT_END_NAMESPACE

class Core : public QObject
{
public:
    Core(int number,QColor color,QChart *chart,QCategoryAxis *axisX,QCategoryAxis *axisY,QCheckBox *check_box,QObject *parent = nullptr);
    ~Core();
    QLineSeries *series;
    float get_curr_value();
public slots:
    void update_series();
    void release_attach_series(bool state);


private:

    QChart *chart;
    int hue;
    int number;
    int old_idle;
    int old_sum;
    std::list<float> data_points;



};

#endif // CORE_H
