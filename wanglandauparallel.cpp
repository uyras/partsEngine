#include "wanglandauparallel.h"

WangLandauParallel::WangLandauParallel(PartArray &system, unsigned int intervals, unsigned int gaps, double overlap, unsigned int walkersByGap):
    sys(system),
    intervals(intervals),
    gaps(gaps),
    walkersByGap(walkersByGap),
    overlap(overlap)
{
    sys.setToGroundState();
    this->eMin = sys.calcEnergy1();
    sys.setToMaximalState();
    this->eMax = sys.calcEnergy1();

    sys.state->reset();
    this->eInit = sys.calcEnergy1FastIncrementalFirst();

    for (unsigned int i=0;i<gaps;i++){
        for (unsigned int j=0;j<walkersByGap;j++){
            walkers.push_back(
                        WangLandauParallelWalker(
                            system,
                            this->intervals,
                            this->eMin,
                            this->eMax,
                            i,
                            i*walkersByGap+j));
        }
    }
}

WangLandauParallel::~WangLandauParallel()
{

}

vector<double> WangLandauParallel::dos()
{
    bool continueFlag=true;
    //выполняем несколько шагов WL на каждом walker'е. Каждый walker имеет свой критерий плоскости
    while (continueFlag){
        continueFlag=false;
        for (unsigned w=0;w<walkers.size();w++){
            if (walkers[w].walk())
                continueFlag=true;
        }

        for (unsigned int gap1=0; gap1 < this->gaps-1; gap1++){
            //подбираем двух случайных walker из двух соседних блуждателей и переворачиваем
            unsigned int gap2=gap1+1;
            WangLandauParallelWalker *walker1=takeRandomFromGap(gap1), *walker2=takeRandomFromGap(gap2);
            this->swapWalkers(walker1,walker2);
        }
    }

    //сохраняем walker в файл
    vector<double>::iterator iter;
    for (unsigned w=0;w<walkers.size();w++){
        ofstream f(QString("g_%1.txt").arg(w).toStdString().c_str());
        iter=walkers[w].g.begin();
        int i=0;
        while (iter!=walkers[w].g.end()){
            f<<i<<"\t"<<*iter<<endl;
            iter++; i++;
        }
        f.close();
    }
    return walkers[0].g;
}

bool WangLandauParallel::swapWalkers(WangLandauParallelWalker *walker1, WangLandauParallelWalker *walker2)
{
    double ex = walker1->sys.calcEnergy1FastIncremental(eInit), ey = walker2->sys.calcEnergy1FastIncremental(eInit);

    double p = (walker1->getG(ex)+walker2->getG(ey)) - (walker1->getG(ey)+walker2->getG(ex));

    if (double(config::Instance()->rand()) / double(config::Instance()->rand_max) <= p){
            unsigned tempGap = walker1->gap();
            walker1->setGap(walker2->gap());
            walker2->setGap(tempGap);
            return true;
    } else
        return false;
}

WangLandauParallelWalker *WangLandauParallel::takeRandomFromGap(unsigned int gapNumber)
{
    unsigned int randnum = unsigned(floor(double(config::Instance()->rand()) / double(config::Instance()->rand_max) * (double)walkersByGap))+1;
    vector<WangLandauParallelWalker>::iterator iter = walkers.begin();
    while (iter!=walkers.end()){
        if (randnum>0 && (*iter).gap()==gapNumber){
            randnum--;
            if (randnum==0)
                return &(*iter);
        }

        iter++;
    }
    return &(*iter);
}

