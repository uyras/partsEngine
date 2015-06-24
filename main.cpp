#include <QCoreApplication>
#include "config.h"
#include "PartArray.h"
#include "squarespinicearray.h"

#include "StateMachine.h"
#include "StateMachineFree.h"
#include "wanglandau.h"

using namespace std;

double eMin,eMax, dE;
int intervals = 999;
int steps=10000; //число шагов для нормализации гистограммы

unsigned int getIntervalNumber(double Energy){
    return round((Energy-eMin)/dE);
}

void setToValue(vector<double> & histogramm, double value){
    vector<double>::iterator iter = histogramm.begin();
    while (iter!=histogramm.end()){
        (*iter)=value;
        iter++;
    }
}

//критерий плоскости гистограммы
bool isFlat(vector<double> &histogramm){
    vector<double>::iterator iter;

    //считаем среднее значение
    double average=0; int step=0;
    iter = histogramm.begin();
    while (iter!=histogramm.end()){
        if (*iter != 0){
            average = (average*step+(*iter))/(step+1);
            step++;
        }
        iter++;
    }

    iter = histogramm.begin();
    while (iter!=histogramm.end()){
        if ((*iter)!=0 && fabs(*iter-average)/average > 0.2) //критерий плоскости
            return false;
        iter++;
    }
    return true;
}

//выровнять гистограмму, чтобы сумма значений была равна 1
void normalize(vector<double> &histogramm){
    double d = histogramm[0]-log(2);

    vector<double>::iterator iter;

    //равняем на единицу
    iter = histogramm.begin();
    while (iter!=histogramm.end()){
        if (*iter!=0)
            (*iter)-=d;
        iter++;
    }
}

void save(vector<double> &histogramm, char* filename){
    ofstream f(filename);
    vector<double>::iterator iter;

    //считаем сумму значений
    iter = histogramm.begin();
    int i=0;
    while (iter!=histogramm.end()){
        f<<(eMin+i*dE)<<"\t"<<exp(*iter)<<endl;
        iter++; i++;
    }
    f.close();
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);


    config::Instance()->srand(time(NULL));
    config::Instance()->m = 1;

    SquareSpinIceArray sys;
    sys.dropSpinIce(3,3);
    cout<<"Parts: "<<sys.count()<<endl;
    vector <double> g = WangLandau::dos(sys,1000,10000,0.8);
    vector <double> scale = WangLandau::scale(sys,1000);


    ofstream f(filename);
    vector<double>::iterator iter,iter2;

    //считаем сумму значений
    iter = histogramm.begin();
    iter2 = scale.begin();
    while (iter!=histogramm.end()){
        f<<(*scale)<<"\t"<<exp(*iter)<<endl;
        iter++; iter2++;
    }
    f.close();
    save(g,"hist.dat");

    cout<<"finish"<<endl;
    return 0;
    //return a.exec();
}
