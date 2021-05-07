#include "Core.h"

Core::Core(int number,QColor color,QChart *chart,QCategoryAxis *axisX,QCategoryAxis *axisY  )
{
    this->number=number;

    this->hue=hue;
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




}
Core::~Core(){
    //why qt
    delete this->series;

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
            std::cout<<"idle: "<<help<<std::endl;
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
          std::cout<<this->number<<" Core percent: "<<(int)percent<<std::endl;

        //safe for next time
        this->old_idle=idle;
        this->old_sum=sum;
    }
    int intpart = (int)percent;

    //update datapoints
    this->data_points.pop_front();
    this->data_points.push_back(intpart);

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
