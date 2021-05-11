#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<iostream>
#include <list>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //make chart
    QChart *chart = new QChart();


    //make x axis
    QCategoryAxis *axisX = new QCategoryAxis();
    axisX->setRange(0, 100);
    axisX->setGridLineVisible(true);
    chart->addAxis(axisX, Qt::AlignBottom);


    //make y axis and add labels
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


    //make chartview and add it to the gui
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setParent(ui->horizontalFrame);



    //setup the core objects
    core_count=get_core_count();
    //reserve mem
    core_series.reserve(core_count+1);




    //hue_counter is the HSV hue devided by the number of neede colors
    //this produces adequately distinct colors
    int hue_counter=0;
     QChar c = QChar(0xF0);
    for(int i=0;i<=core_count;i++){

        QCheckBox *button = new QCheckBox (this);
        //QPushButton *button = new QPushButton(this);

        button->setText( QString(c) +  "core" + QString::number(i));
        button->setChecked(true);



        QLabel *lable = new QLabel();
        lable->setText("");




        QHBoxLayout *hbox = new QHBoxLayout();
        hbox->setAlignment(Qt::AlignLeft);

        hbox->addWidget(button);
        hbox->addWidget(lable);

        ui->vertical->addLayout(hbox);

        //remember
        if(i==0){
            //Special for CPU
             button->setText( QString(c) +  "CPU");
            core_series.push_back(std::make_pair(new Core(0,QColor().black(),chart,axisX,axisY,button,this),lable));
        }
        else{
            //normal cores
            button->setText( QString(c) +  "core " + QString::number(i));
            core_series.push_back(std::make_pair( new Core(i,QColor().fromHsv(hue_counter,204,255,200),chart,axisX,axisY,button,this),lable));
        }


        //hsv hue
        hue_counter=hue_counter+(300/core_count);





    }

    //finish chart
    chart->setTitle("cpu utilisation");
    chart->legend()->setVisible(false);
    //chart->legend()->setAlignment(Qt::AlignBottom);

    ui->vertical->setAlignment(Qt::AlignLeft);







    //add the update function to a timer

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update_series()));
    //send signal every second
    timer->start(500);



}

//destuctor
MainWindow::~MainWindow()
{
    for(std::pair<Core *,QLabel*>c:core_series){
        //dont delete the second part because the QT Layout takes ownership of it
        delete c.first;
    }
    delete ui;
}

// updates the data and series for every core object
void MainWindow::update_series(){

    for(std::pair<Core *,QLabel*>c:core_series){
        c.first->update_series();
        //std::cout<<"test: "<<c.first->get_curr_value()<<std::endl;
        c.second->setText(QString::number(c.first->get_curr_value(),'f',2)+" %");
    }

}



//extracts the number of cores of the cpu (including virt)
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


