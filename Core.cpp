#include "Core.h"

Core::Core(int number,QColor color,QChart *chart,QCategoryAxis *axisX,QCategoryAxis *axisY,QCheckBox *check_box,QObject *parent  ):QObject(parent)
{
    this->number=number;
    this->chart=chart;
    this->old_idle=0;
    this->old_sum=0;

    //why qt
    this->series= new QLineSeries;
    if(number){
        QString name = "core" ;
        name.append(QString::number(number-1));
        this->series->setName(name);
        this->series->setColor(color);

    }
    else{
        this->series->setName("CPU");
        this->series->setColor(QColor().fromHsv(300,255,0,255));


    }




    //this is kinda ugly but it only happens once
    //also i think the compiler optimises this away
    for(int i=0;i<100;i++){
        this->data_points.push_back(1);
        series->append(i,1);
    }



    chart->addSeries(this->series);
    series->attachAxis(axisX);
    series->attachAxis(axisY);

    QObject::connect(check_box,&QCheckBox::toggled,this,&Core::release_attach_series);




}
Core::~Core(){
    //why qt
    delete this->series;

}

float Core::get_curr_value()
{
    return this->data_points.back();
}

void Core::update_series()
{
    std::string line;
    std::ifstream stat("/proc/stat");
    //ignore first line
    //stat.ignore(100000,'\n');
    //ignore all further until the right one
    for(int i =0;i<this->number;i++){
        stat.ignore(100000,'\n');
    }

    int sum=0;
    int idle=0;
    int help;
    //ignore "cpu"
     stat.ignore(4);
    //getline(stat,line);
    //std::cout<<line<<std::endl;

    //summ all time paramenters
    for(int i=0;i<10;i++){
        stat >> help;
        //std::cout<<"help: "<<help<<std::endl;
        //get idle
        if(i==3){
            idle = help;
            //std::cout<<"idle: "<<help<<std::endl;
        }
        sum += help;
    }
    //calc load percent
    double percent;
    if(this->old_sum==0){
        this->old_idle=idle;
        this->old_sum=sum;
        percent=0;
    }
    else{
        int delta_idle = idle - this->old_idle;
        float delta_sum = sum - this->old_sum;
        percent = delta_sum - delta_idle;
        percent =100*(percent/delta_sum);
         // std::cout<<this->number<<" Core percent: "<<percent<<std::endl;

        //safe for next time
        this->old_idle=idle;
        this->old_sum=sum;
    }

    //update datapoints
    this->data_points.pop_front();
    this->data_points.push_back(percent);

    //clear series we cant remove because
    //qt doesnt like that dunno why
    this->series->clear();
    int i=0;
    for(auto it = this->data_points.begin();it!=this->data_points.end();it++){
        this->series->append(i,*it);
        i++;
    }

    stat.close();


}

void Core::release_attach_series(bool state)
{
    if(state){
         this->chart->addSeries(this->series);
    }
    else{
        this->chart->removeSeries(this->series);
    }

}


