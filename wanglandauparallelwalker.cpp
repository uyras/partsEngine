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
    double x = (this->eMax-this->eMin)/((double)gaps*(1.-overlap)+overlap); //ширина одного интервала
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
    double saveToFile=false;
    if (f<=fMin)
        return false;

    ofstream ffile(QString("e_%1.txt").arg(number).toStdString().c_str(),ios_base::out|ios_base::app);

    const int count = sys->count();

    double eOld = sys->calcEnergy1FastIncremental(eInit);
    double eNew = eOld;

    do {
        //повторяем алгоритм сколько-то шагов
        for (unsigned i=1;i<=stepsPerWalk;i++){
            int partNum = sys->state->randomize();

            eNew = sys->calcEnergy1FastIncremental(eInit);

            if (eNew>to || eNew<from){
                sys->parts[partNum]->rotate(); //откатываем состояние
                i--;
                continue;
            }

            if (saveToFile) ffile<<eNew<<endl;

            double randnum = Random::Instance()->nextDouble();
            if (randnum==0.0)
                randnum=10e-20;
            randnum = std::log(randnum);

            if (
                    randnum <=
                    this->getG(eOld)-this->getG(eNew)
                    ) {
                eOld = eNew;
            }
            else {
                sys->parts[partNum]->rotate(); //откатываем состояние
            }

            h[this->getIntervalNumber(eOld)]+=1;
            g[this->getIntervalNumber(eOld)]+=log(f);

        }
    } while (!this->isFlat());//повторяем пока гистограмма не станет плоской

    //WangLandau::normalize(g);
    WangLandau::setValues(h,0); //обнуляем гистограмму
    f=sqrt(f);
    qDebug()<<"modify f="<<f<<" on "<<this->number<<" walker";

    ffile.close();
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
    while (iter!=h.begin()+this->getIntervalNumber(this->to)+1){ //плоскость гистограммы только в своем интервале
    //iter = h.begin(); //плоскость гистограммы целиком
    //while (iter!=h.end()){
        if (*iter != 0){
            average = (average*step+(*iter))/(step+1);
            step++;
        }
        iter++;
    }

    iter = h.begin()+this->getIntervalNumber(this->from);
    while (iter!=h.begin()+this->getIntervalNumber(this->to)+1){ //плоскость гистограммы только в своем интервале
    //iter = h.begin(); //плоскость гистограммы целиком
    //while (iter!=h.end()){
        if ((*iter)!=0. && fabs(*iter-average)/average > (1.0 - accuracy)) //критерий плоскости
            return false;
        iter++;
    }
    return true;
}

void WangLandauParallelWalker::makeNormalInitState()
{
    unsigned long int i=0;
    double eTemp = sys->calcEnergy1FastIncremental(eInit);

    while (floor(eTemp*10.0e10) < floor(this->from*10.0e10) || eTemp > this->to){
        this->sys->state->randomize();
        eTemp = sys->calcEnergy1FastIncremental(eInit);
        i++;
    }
    qDebug()<<this->number<<": normalize init state takes "<<i<<" steps";
}

