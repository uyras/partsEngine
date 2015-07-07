#include "wanglandauparallelwalker.h"

WangLandauParallelWalker::WangLandauParallelWalker(PartArray system,
        unsigned int intervals,
        double eMin,
        double eMax,
        double gap,
        int number):
    sys(system),
    intervals(intervals),
    eMin(eMin),
    eMax(eMax),
    number(number)
{
    for (unsigned i=0;i<intervals;i++){
        g.push_back(0);
        h.push_back(0);
    }
    this->setGap(gap);
    this->dE = (eMax-eMin)/(intervals-1);
    this->fMin=1.000001;
    this->f = exp(1);

    sys.state->reset();
    eInit = sys.calcEnergy1FastIncrementalFirst();
    ofstream ffile(QString("e_%1").arg(number).toStdString().c_str());
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
    ofstream ffile(QString("e_%1").arg(number).toStdString().c_str(),ios_base::out|ios_base::app);

    if (f<=fMin)
        return false;

    double eOld = eInit,eNew=eInit;
    const int count = sys.count();

    while (true){

        //повторяем алгоритм сколько-то шагов
        for (unsigned i=0;i<stepsPerWalk;i++){
            double rand = (double)config::Instance()->rand()/(double)config::Instance()->rand_max;
            int partNum = (int)floor(rand*(double)count);
            sys.parts[partNum]->rotate();
            eNew = sys.calcEnergy1FastIncremental(eInit);

            if (
                    eNew<=to && eOld>=from &&
                    (double)config::Instance()->rand()/(double)config::Instance()->rand_max <=
                    exp(g[WangLandau::getIntervalNumber(eOld,eMin,dE)]-g[WangLandau::getIntervalNumber(eNew,eMin,dE)])
                    ) {
                eOld = eNew;
                ffile<<eNew<<endl;
            }
            else {
                sys.parts[partNum]->rotate(); //откатываем состояние
            }

            g[WangLandau::getIntervalNumber(eOld,eMin,dE)]+=log(f);
            h[WangLandau::getIntervalNumber(eOld,eMin,dE)]+=1;
        }

        //проверяем ровность диаграммы
        if (WangLandau::isFlat(h,accuracy)){
            WangLandau::normalize(g);
            WangLandau::setValues(h,0);
            f=sqrt(f);
            ffile.close();
            return f>fMin;
        }
    }
}

