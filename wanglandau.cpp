#include "wanglandau.h"

WangLandau::WangLandau()
{

}

WangLandau::~WangLandau()
{

}

vector<double> WangLandau::dos(PartArray &sys,const int intervals,const int steps,const double accuracy)
{
    const StateMachineFree initState = sys.state;

    unsigned long long int totalSteps=0;

    sys.setToGroundState();
    const double eMin=sys.calcEnergy1();
    sys.setToMaximalState();
    const double eMax = sys.calcEnergy1();
    const double dE = (eMax-eMin)/(intervals-1);
    sys.state->reset();

    const double eInit = sys.calcEnergy1FastIncrementalFirst();

    qDebug()<<"eMin="<<eMin;
    qDebug()<<"eMax="<<eMax;
    qDebug()<<"dE="<<dE;
    qDebug()<<"steps="<<steps;

    //создать две гистограммы
    vector<double> g,h; //g - логарифм плотности состояний (энтропия), h - вспомогательная гистограмма, которая должна быть плоской

    for (int i=0;i<intervals;i++){
        g.push_back(0);
        h.push_back(0);
    }

    const double fMin=1.000001;
    double f = exp(1);
    double eOld = eInit,eNew=eInit;
    const int count = sys.count();

    while (f>fMin){

        //повторяем алгоритм сколько-то шагов
        for (int i=0;i<steps;i++){
            double rand = (double)config::Instance()->rand()/(double)config::Instance()->rand_max;
            int partNum = (int)floor(rand*(double)count);
            sys.parts[partNum]->rotate();
            eNew = sys.calcEnergy1FastIncremental(eInit);

            if (
                    (double)config::Instance()->rand()/(double)config::Instance()->rand_max <=
                    exp(g[WangLandau::getIntervalNumber(eOld,eMin,dE)]-g[WangLandau::getIntervalNumber(eNew,eMin,dE)])
                    ) {
                eOld = eNew;
            }
            else {
                sys.parts[partNum]->rotate(); //откатываем состояние
            }

            g[WangLandau::getIntervalNumber(eOld,eMin,dE)]+=log(f);
            h[WangLandau::getIntervalNumber(eOld,eMin,dE)]+=1;
            totalSteps+=1;
        }

        //проверяем ровность диаграммы
        if (WangLandau::isFlat(h,accuracy)){
            WangLandau::normalize(g);
            f=sqrt(f);
            WangLandau::setValues(h,0);
            qDebug()<<"h is flat, new f is "<<f;
        }
    }

    qDebug()<<"Total steps: "<<totalSteps;


    (*sys.state) = initState;
    return g;
}

vector<double> WangLandau::scale(PartArray &sys, const int intervals)
{
    const StateMachineFree initState = sys.state;
    sys.setToGroundState();
    const double eMin=sys.calcEnergy1();
    sys.setToMaximalState();
    const double eMax = sys.calcEnergy1();
    const double dE = (eMax-eMin)/(intervals-1);
    vector<double> scale;
    for (int i=0;i<intervals;i++){
        scale.push_back(eMin+((double)i * dE));
    }
    (*sys.state) = initState;
    return scale;
}

unsigned int WangLandau::getIntervalNumber(double Energy,const double eMin,const double dE)
{
    return round((Energy-eMin)/dE);
}

void WangLandau::setValues(vector<double> &histogramm, double value)
{
    vector<double>::iterator iter = histogramm.begin();
    while (iter!=histogramm.end()){
        (*iter)=value;
        iter++;
    }
}

//критерий плоскости гистограммы
bool WangLandau::isFlat(vector<double> &histogramm, const double accuracy)
{
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
        if ((*iter)!=0 && fabs(*iter-average)/average > (1.0 - accuracy)) //критерий плоскости
            return false;
        iter++;
    }
    return true;
}

//выровнять гистограмму, чтобы сумма значений была равна 1
void WangLandau::normalize(vector<double> &histogramm)
{
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
