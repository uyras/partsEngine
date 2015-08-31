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
    average(0),
    hCount(0)
{
    for (unsigned i=0;i<intervals;i++){
        g.push_back(0);
        h.push_back(0);
    }
    this->dE = (eMax-eMin)/(intervals-1);
    this->fMin=1.00001;
    this->f = exp(1);
    this->setGap(gap);

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
    double x = (this->eMax-this->eMin)/((double)gaps*(1.-overlap)+overlap); //ширина одного интервала
    this->from = this->eMin + (double)gap * x * (1.-overlap);
    this->to = this->from+x;
    this->nFrom = getIntervalNumber(from);
    this->nTo = getIntervalNumber(to);
    this->gapNumber = gap;
}

unsigned int WangLandauParallelWalker::gap()
{
    return this->gapNumber;
}

double WangLandauParallelWalker::getG(double e)
{
    return this->g[this->getIntervalNumber(e)];
}

void WangLandauParallelWalker::walk(unsigned stepsPerWalk)
{
    double saveToFile=false;

    ofstream ffile(QString("e_%1.txt").arg(number).toStdString().c_str(),ios_base::out|ios_base::app);

    double eOld = sys->calcEnergy1FastIncremental(eInit);
    double eNew;

    unsigned long int totalRotations=0; //всего вращений системы

    //повторяем алгоритм сколько-то шагов 10000
    for (unsigned i=1;i<=stepsPerWalk;i++){

        int partNum = sys->state->randomize(); totalRotations++;

        eNew = sys->calcEnergy1FastIncremental(eInit);

        if (saveToFile) ffile<<eNew<<endl;

        double randnum = Random::Instance()->nextDouble();
        if (randnum==0.0)
            randnum=10e-20;
        randnum = std::log(randnum);

        if (inRange(eNew) && randnum <= this->getG(eOld)-this->getG(eNew) ) {
            eOld = eNew;
        } else {
            sys->parts[partNum]->rotate(); //откатываем состояние
        }

        this->updateGH(eOld);
    }

    ffile.close();
}

void WangLandauParallelWalker::processWalk()
{
    WangLandau::setValues(h,0); //обнуляем гистограмму
    f=sqrt(f);
    average = 0;
    hCount = 0;
    qDebug()<<"modify f="<<f<<" on "<<this->number<<" walker";
}

unsigned int WangLandauParallelWalker::getIntervalNumber(double Energy)
{
    return floor((Energy-this->eMin)/this->dE);
}

double WangLandauParallelWalker::getEnergyByInterval(unsigned int interval)
{
    return dE*(double)interval+eMin;
}

//критерий плоскости гистограммы
bool WangLandauParallelWalker::isFlat()
{
    vector<double>::iterator iter;

    if (average==0.0){
        average = this->calcAverageH();
    }

    iter = h.begin()+nFrom;
    while (iter!=h.begin()+this->nTo+1){ //плоскость гистограммы только в своем интервале
        if ((*iter)!=0. && fabs(*iter-average)/average > (1.0 - accuracy)) //критерий плоскости
            return false;
        iter++;
    }
    return true;
}

bool WangLandauParallelWalker::finished()
{
    return f<=fMin;
}

void WangLandauParallelWalker::makeNormalInitState()
{
    unsigned long int i=0;
    double eTemp=0;
    while (!inRange(eTemp=sys->calcEnergy1FastIncremental(eInit))){
        this->sys->state->randomize();
        i++;
    }
    qDebug()<<this->number<<": normalize init state takes "<<i<<" steps";
    this->updateGH(eTemp);
}

void WangLandauParallelWalker::updateGH(double E)
{
    if (E==0.){
        E = this->sys->calcEnergy1FastIncremental(this->eInit);
    }

    g[this->getIntervalNumber(E)]+=log(f);

    bool increased = (h[this->getIntervalNumber(E)]+=1) == 1;

    if (inRange(E)){//считаем среднее только в разрешенном интервале
        if (increased){ //прибавляем h и одновременно считаем среднее значение
            //случай если изменилось число ненулевых элементов
            hCount++;
            average = (average * (hCount-1) + 1) / hCount;
        } else {
            average += (1./(double)hCount);
        }
    }
}

double WangLandauParallelWalker::calcAverageH()
{
    double avg=0;
    //считаем среднее значение
    int step=0;
    vector<double>::iterator iter = h.begin()+this->getIntervalNumber(this->from);
    while (iter!=h.begin()+this->getIntervalNumber(this->to)+1){ //плоскость гистограммы только в своем интервале
        if (*iter != 0){
            avg = (avg*step+(*iter))/(step+1);
            step++;
        }
        iter++;
    }
    return avg;
}

bool WangLandauParallelWalker::inRange(double E)
{
    unsigned i = getIntervalNumber(E);
    return i<=nTo && i>=nFrom;
}

