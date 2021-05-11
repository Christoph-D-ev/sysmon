#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCharts/QtCharts>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QTimer>
#include <QPushButton>
#include <unistd.h>
#include <fstream>
#include "Core.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
     void get_new_info();

public slots:
     void update_series();

private:
    Ui::MainWindow *ui;

    int get_core_count();


    int core_count=1;

    int last_idle=0;
    int last_sum=0;
    QLineSeries *main_series;
    std::vector<std::pair<Core*,QLabel*>> core_series;



};
#endif // MAINWINDOW_H
