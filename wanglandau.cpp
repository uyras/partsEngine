#include "wanglandau.h"

WangLandau::WangLandau(PartArray *sys, unsigned intervals, double accuracy, double fmin):
    showMessages(false),
    sys(sys),
    intervals(intervals),
    accuracy(accuracy),
    fMin(fmin),
    f(0)
{

    //считаем минимум и максимум системы
    if (sys->Minstate().size()==0 || sys->Maxstate().size()==0)
        qFatal("Min or max state is unknown. DOS calculation is impossible.");

    //инициируем DOS
    h.resize(sys->E(sys->Minstate()), sys->E(sys->Maxstate()), intervals);
    g.resize(sys->E(sys->Minstate()), sys->E(sys->Maxstate()), intervals);
}

WangLandau::~WangLandau()
{

}

void WangLandau::run(unsigned steps)
{
    const StateMachineFree initState = sys->state;
    this->f = exp(1);
    long unsigned accepted=0, rejected=0;
    this->resetH();
    updateGH(sys->E());

    sys->state = sys->Minstate();

    qDebug()<<"steps="<<steps;

    double eOld = sys->E(),eNew=sys->E();

    while (f>fMin){
        //повторяем алгоритм сколько-то шагов
        for (unsigned i=0;i<steps;i++){
            int partNum = sys->state.randomize();

            eNew = sys->E();
            if (Random::Instance()->nextDouble() <= exp(g[eOld]-g[eNew])) {
                eOld = eNew;
                accepted++;
            } else {
                sys->parts[partNum]->rotate(true); //откатываем состояние
                rejected++;
            }

            updateGH(eOld);
        }

        //проверяем ровность диаграммы
        if (this->isFlat()){
            this->normalizeG();
            f=sqrt(f);
            this->resetH();
            msg("accepted ",(int)accepted);
            msg("rejected ",(int)rejected);
            msg("h is flat, new f is ",f);
            accepted=0; rejected=0;

        }
    }


    sys->state = initState;
    return;
}

void WangLandau::runWithSave(unsigned steps, unsigned saveEach)
{
    const StateMachineFree initState = sys->state;
    this->f = exp(1);
    this->resetH();
    unsigned long long totalSteps=0;
    ostringstream fn;

    sys->state = sys->Minstate();

    qDebug()<<"steps="<<steps;

    double eOld = sys->E(),eNew=sys->E();

    while (f>fMin){
        //повторяем алгоритм сколько-то шагов
        for (unsigned i=0;i<steps;i++){
            int partNum = sys->state.randomize();

            eNew = sys->E();
            if (Random::Instance()->nextDouble() <= exp(g[eOld]-g[eNew])) {
                eOld = eNew;
            } else {
                sys->parts[partNum]->rotate(true); //откатываем состояние
            }

            updateGH(eOld);
            ++totalSteps;

            if (totalSteps%saveEach==0){
                fn.str("");
                fn.clear();
                fn<<"g_"<<totalSteps<<".dat";
                saveG(fn.str());

                fn.str("");
                fn.clear();
                fn<<"h_"<<totalSteps<<".dat";
                saveH(fn.str());
                qDebug()<<totalSteps;
            }
        }

        //проверяем ровность диаграммы
        if (this->isFlat()){
            this->normalizeG();
            f=sqrt(f);
            this->resetH();
            qDebug()<<"h is flat, new f is "<<f;
        }
    }


    sys->state = initState;
    return;
}

void WangLandau::saveH(const string filename) const
{
    ofstream f(filename);
    for (unsigned i=0;i<h.Intervals();i++){
        f<<i<<"\t"<<h.val(i)<<"\t"<<h.at(i)<<endl;
    }

    f.close();
}

void WangLandau::saveG(const string filename) const
{
    ofstream f(filename);
    for (unsigned i=0;i<g.Intervals();i++){
        f<<i<<"\t"<<g.val(i)<<"\t"<<g.at(i)<<endl;
    }

    f.close();
}

//критерий плоскости гистограммы
bool WangLandau::isFlat()
{
    for (unsigned i=0; i<h.Intervals(); i++){//плоскость гистограммы только в своем интервале
        if (h.at(i)!=0. && fabs(h.at(i)-average)/average > (1.0 - accuracy)) //критерий плоскости
            return false;
    }
    return true;
}

void WangLandau::updateGH(double E)
{
    if (E==0.){
        E = this->sys->E();
    }

    g[E]+=log(f);

    bool increased = (h[E]+=1) == 1;

    if (increased){ //прибавляем h и одновременно считаем среднее значение
        //случай если изменилось число ненулевых элементов
        hCount++;
        average = (average * (hCount-1) + 1) / hCount;
    } else {
        average += (1./(double)hCount);
    }
}

void WangLandau::resetH()
{
    for (unsigned i=0; i<h.Intervals(); i++){
        h.at(i)=0;
    }
    average=0.0;
    hCount=0;
}

void WangLandau::normalizeG()
{
    const double gFirst=g.at(0);
    for (unsigned i=0; i<g.Intervals(); i++){
        g.at(i)-=gFirst;
    }
}
