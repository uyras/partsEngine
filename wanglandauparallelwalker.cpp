#include "wanglandauparallelwalker.h"

WangLandauParallelWalker::WangLandauParallelWalker(PartArray *system,
        unsigned int intervals,
        double eMin,
        double eMax, double overlap, unsigned int gaps,
        double gap,
        int number):
    sys(system),
    intervals(intervals),
    eMin(eMin),
    eMax(eMax),
    overlap(overlap),
    gaps(gaps),
    number(number),
    accuracy(0.8),
    stepsPerWalk(10000) //число шагов на одно блуждание
{
    for (unsigned i=0;i<intervals;i++){
        g.push_back(0);
        h.push_back(0);
    }
    this->setGap(gap);
    this->dE = (eMax-eMin)/(intervals-1);
    this->fMin=1.000001;
    this->f = exp(1);

    sys->state->reset();
    eInit = sys->calcEnergy1FastIncrementalFirst();
    ofstream ffile(QString("e_%1.txt").arg(number).toStdString().c_str());
    ffile.close();
}

WangLandauParallelWalker::~WangLandauParallelWalker()
{
}

void WangLandauParallelWalker::setGap(unsigned int gap)
{
    double x = (this->eMax-this->eMin)/((double)gaps*(1.-overlap)+overlap);
    this->from = this->eMin + (double)gap * x * (1.-overlap);
    this->to = this->from+x;
    this->gapNumber = gap;
}

unsigned int WangLandauParallelWalker::gap()
{
    return this->gapNumber;
}

double WangLandauParallelWalker::getG(double e)
{
    return this->g[WangLandau::getIntervalNumber(e, this->eMin, this->dE)];
}

bool WangLandauParallelWalker::walk()
{
    ofstream ffile(QString("e_%1.txt").arg(number).toStdString().c_str(),ios_base::out|ios_base::app);

    if (f<=fMin)
        return false;

    double eOld = eInit,eNew=eInit;
    const int count = sys->count();

    //повторяем алгоритм сколько-то шагов
    for (unsigned i=1;i<=stepsPerWalk;i++){
        double rand = double(config::Instance()->rand())/double(config::Instance()->rand_max+1);
        int partNum = (int)floor(rand*(double)count);
        sys->parts[partNum]->rotate();
        eNew = sys->calcEnergy1FastIncremental(eInit);

        if (eNew<=to && eNew>=from){

            if (
                    double(config::Instance()->rand())/double(config::Instance()->rand_max) <=
                    exp(g[this->getIntervalNumber(eOld)]-g[this->getIntervalNumber(eNew)])
                    ) {
                eOld = eNew;
                ffile<<eNew<<endl;
            }
            else {
                sys->parts[partNum]->rotate(); //откатываем состояние
            }

            g[this->getIntervalNumber(eOld)]+=log(f);
            h[this->getIntervalNumber(eOld)]+=1;

        } else {
            i--;
        }
    }

    //проверяем ровность диаграммы
    if (this->isFlat()){
        WangLandau::normalize(g);
        WangLandau::setValues(h,0);
        f=sqrt(f);
        ffile.close();
        qDebug()<<"modify f="<<f<<" on "<<this->number<<" walker";
    }
    return f>fMin;
}

unsigned int WangLandauParallelWalker::getIntervalNumber(double Energy)
{
    return round((Energy-this->eMin)/this->dE);
}

//критерий плоскости гистограммы
bool WangLandauParallelWalker::isFlat()
{
    vector<double>::iterator iter;

    //считаем среднее значение
    double average=0; int step=0;
    iter = h.begin()+this->getIntervalNumber(this->from);
    while (iter!=h.begin()+this->getIntervalNumber(this->to)+1){
        if (*iter != 0){
            average = (average*step+(*iter))/(step+1);
            step++;
        }
        iter++;
    }

    iter = h.begin()+this->getIntervalNumber(this->from);
    while (iter!=h.begin()+this->getIntervalNumber(this->to)+1){
        if ((*iter)!=0 && fabs(*iter-average)/average > (1.0 - accuracy)) //критерий плоскости
            return false;
        iter++;
    }
    return true;
}

