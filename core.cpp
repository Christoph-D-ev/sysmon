#include "core.h"

core::core()
{

}
core::~core(){

}

void core::update_series()
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
    stat.ignore(3);
    //summ all time paramenters
    for(int i=0;i<10;i++){
        stat >> help;
        //get idle
        if(i==3){
            idle = help;
        }
        sum += help;
    }
    //calc load percent
    double percent;
    int delta_idle = idle - this->old_idle;
    float delta_sum = sum - this->old_sum;
     percent = delta_sum - delta_idle;
     percent =100*(percent/delta_sum);

     //safe for next time
    this->old_idle=idle;
    this->old_sum=sum;

     //update datapoints
     this->data_points.pop_front();
     this->data_points.push_back((int)percent);

     //clear series we cant remove because
     //qt doesnt like that dunno why
     this->series.clear();
     int i=0;
     for(auto it = this->data_points.begin();it!=this->data_points.end();it++){
         this->series.append(i,*it);
         i++;
     }

     stat.close();


}
