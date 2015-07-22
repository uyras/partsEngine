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
    return this->g[this->getIntervalNumber(e)];
}

void WangLandauParallelWalker::walk(unsigned stepsPerWalk)
{
    double saveToFile=false;

    ofstream ffile(QString("e_%1.txt").arg(number).toStdString().c_str(),ios_base::out|ios_base::app);

    double eOld = sys->calcEnergy1FastIncremental(eInit);
    double eNew;

    unsigned long int
            totalRotations=0, //всего вращений системы
            discartedRotations=0; //отмененных вращений системы

    //повторяем алгоритм сколько-то шагов
    for (unsigned i=1;i<=stepsPerWalk;i++){

        int partNum = sys->state->randomize(); totalRotations++;

        eNew = sys->calcEnergy1FastIncremental(eInit);

        /*if (eNew>to || eNew<from){
                sys->parts[partNum]->rotate(); //откатываем состояние
                discartedRotations++;
                continue;
            }*/

        if (saveToFile) ffile<<eNew<<endl;

        double randnum = Random::Instance()->nextDouble();
        if (randnum==0.0)
            randnum=10e-20;
        randnum = std::log(randnum);

        if (
                (eNew>=from && eNew<=to) &&
                randnum <= this->getG(eOld)-this->getG(eNew)
                ) {
            eOld = eNew;
        }
        else {
            sys->parts[partNum]->rotate(); //откатываем состояние
        }

        this->updateGH(eOld);
    }

    ffile.close();
}

bool WangLandauParallelWalker::processWalk()
{
    WangLandau::setValues(h,0); //обнуляем гистограмму
    f=sqrt(f);
    average = 0;
    hCount = 0;
    qDebug()<<"modify f="<<f<<" on "<<this->number<<" walker";
    return this->finished();
}

unsigned int WangLandauParallelWalker::getIntervalNumber(double Energy)
{
    return round((Energy-this->eMin)/this->dE);
}

//критерий плоскости гистограммы
bool WangLandauParallelWalker::isFlat()
{
    vector<double>::iterator iter;

    if (average==0.0){
        return false;
        /*//считаем среднее значение, устаревший код
        int step=0;
        iter = h.begin()+this->getIntervalNumber(this->from);
        while (iter!=h.begin()+this->getIntervalNumber(this->to)+1){ //плоскость гистограммы только в своем интервале
        //iter = h.begin(); //плоскость гистограммы целиком
        //while (iter!=h.end()){
            if (*iter != 0){
                average = (average*step+(*iter))/(step+1);
                step++;
            }
            iter++;
        }*/
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

bool WangLandauParallelWalker::finished()
{
    return f<=fMin;
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

void WangLandauParallelWalker::updateGH(double E)
{
    if (E==0.){
        E = this->sys->calcEnergy1FastIncremental(this->eInit);
    }

    g[this->getIntervalNumber(E)]+=log(f);

    bool increased = (h[this->getIntervalNumber(E)]+=1) == 1;

    if (E>=from && E<=to){
        if (increased){ //прибавляем h и одновременно считаем среднее значение
            //случай если изменилось число ненулевых элементов
            hCount++;
            average = (average * (hCount-1) + 1) / hCount;
        } else {
            average += (1./(double)hCount);
        }
    }
}

