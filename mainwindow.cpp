#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<iostream>
#include <list>

int core_count=1;
std::list<int> data_points(100,0);
int last_idle=0;
int last_sum=0;
QLineSeries *main_series;
std::vector<QLineSeries*> core_series;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    main_series = new QLineSeries();
    //prime main_series
    int i=0;
    for(std::list<int>::iterator it=data_points.begin(); it != data_points.end(); it++){
       main_series->append(i,*it);
       i++;
    }



    QChart *chart = new QChart();

     chart->addSeries(main_series);

    QCategoryAxis *axisX = new QCategoryAxis();
    axisX->setRange(0, 100);
    axisX->setGridLineVisible(true);
    chart->addAxis(axisX, Qt::AlignBottom);

    main_series->attachAxis(axisX);
    main_series->color().black();

    QCategoryAxis *axisY = new QCategoryAxis();


    axisY->append("10", 10);
    axisY->append("20", 20);
    axisY->append("30", 30);
    axisY->append("40", 40);
    axisY->append("50", 50);
    axisY->append("60", 60);
    axisY->append("70", 70);
    axisY->append("80", 80);
    axisY->append("90", 90);
    axisY->append("100", 100);
    axisY->setRange(0, 100);
    axisY->setGridLineVisible(true);
    chart->addAxis(axisY, Qt::AlignLeft);
    main_series->attachAxis(axisY);



    chart->setTitle("line1");
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setParent(ui->horizontalFrame);


    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(get_new_info()));
    timer->start(1000);
    core_count=get_core_count();

    for(int i=0;i<core_count;i++){
        core_series.push_back(new QLineSeries);
    }






}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::get_new_info()
{
    int sum=0;
    int idle=0;
    int help;
    std::string line;
    std::ifstream stat("/proc/stat");
    //remove "cpu"
    stat.ignore(3);
    for(int i=0;i<10;i++){
        stat >> help;
        std::cout<<"help: "<<help<<std::endl;
        if(i==3){
            idle = help;
        }
        sum += help;
    }
    std::cout<<"idle: "<<idle<<std::endl;
    std::cout<<"sum: "<<sum<<std::endl;
    double percent;
    if(last_sum==0){
        last_idle=idle;
        last_sum=sum;
        percent=0;
    }
    else{
        int delta_idle = idle - last_idle;
        float delta_sum = sum - last_sum;
         percent = delta_sum - delta_idle;
         percent =100*(percent/delta_sum);
        last_idle=idle;
        last_sum=sum;
    }
    int intpart = (int)percent;


    data_points.pop_front();
    data_points.push_back(intpart);
    std::cout<<"percent: "<<percent<<std::endl;

    main_series->clear();
    int i=0;
    for(std::list<int>::iterator it=data_points.begin(); it != data_points.end(); it++){
       main_series->append(i,*it);
       i++;
    }

    stat.close();

}

void MainWindow::get_core_info()
{

}

int MainWindow::get_core_count()
{
    int core_count=0;
    char r ;
    std::string line;
    std::ifstream stat("/proc/stat");
    //ignore first line
    stat.ignore(100000,'\n');

    while (true) {
        //ignore first three chars
        stat.ignore(3);
        stat >> r;
        if(r!='r'){
            core_count++;
        }
        else{
            break;
        }
        //skip rest of the line
        stat.ignore(100000,'\n');
    }

    return core_count;


}

std::vector<int> MainWindow::get_colors(int n)
{
    //only 300 in hsv to not get looping colors
    int step = 300/n;

    std::vector<int> hues;
    for (int i=0;i<=300 ;i=i+step ) {
        hues.push_back(i);
    }
    return hues;
}

