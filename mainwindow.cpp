#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<iostream>
#include <list>


std::list<int> data_points(100,1);


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

     //prime main_series
    main_series = new QLineSeries();

    main_series->setName("CPU");

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
    main_series->setColor(QColor().black());


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

//    QLogValueAxis *axisY = new QLogValueAxis();
//    axisY->setTitleText("Values");
//    axisY->setLabelFormat("%g");
//    axisY->setBase(8.0);
//    axisY->setMinorTickCount(-1);
//    chart->addAxis(axisY, Qt::AlignLeft);



    main_series->attachAxis(axisY);



    chart->setTitle("line1");
    chart->legend()->setVisible(true);
     chart->legend()->setAlignment(Qt::AlignRight);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setParent(ui->horizontalFrame);



    core_count=get_core_count();
    int hue_counter=0;

    for(int i=1;i<=core_count;i++){
        //remember
        core_series.push_back( new Core(i+1,hue_counter,chart,axisX,axisY));

        //hsv hue
        hue_counter=hue_counter+(300/core_count);
    }





    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update_series()));
    timer->start(1000);




}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::update_series(){
    MainWindow::get_new_info();
    for(Core * c:core_series){
        c->update_series();
    }
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

        if(i==3){
            idle = help;
        }
        sum += help;
    }

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
    std::cout<<"Main percent: "<<percent<<std::endl;

    main_series->clear();
    int i=0;
    for(std::list<int>::iterator it=data_points.begin(); it != data_points.end(); it++){
       main_series->append(i,*it);
       i++;
    }

    stat.close();

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

