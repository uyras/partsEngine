#include "wanglandauparallel.h"

WangLandauParallel::WangLandauParallel(PartArray *system, unsigned int intervals, unsigned int gaps, double overlap, unsigned int walkersByGap):
    sys(system->copy()),
    intervals(intervals),
    gaps(gaps),
    walkersByGap(walkersByGap),
    overlap(overlap)
{
    qDebug()<<"calculate min and max";
    sys->setToMaximalState();
    this->eMax = sys->calcEnergy1();
    sys->setToGroundState();
    this->eMin = sys->calcEnergy1();

    sys->state->reset();
    this->eInit = sys->calcEnergy1FastIncrementalFirst();

    qDebug()<<"Start creating walkers";
    for (unsigned int i=0;i<gaps;i++){
        for (unsigned int j=0;j<walkersByGap;j++){
            walkers.push_back(
                        WangLandauParallelWalker(
                            sys->copy(),
                            this->intervals,
                            this->eMin,
                            this->eMax,
                            this->overlap,
                            this->gaps,
                            i,
                            i*walkersByGap+j));
        }
    }
    qDebug()<<"Finish creating walkers";
}

WangLandauParallel::~WangLandauParallel()
{

}

vector<double> WangLandauParallel::dos()
{
    for (int i=0;i<walkers.size();i++){
        (walkers[i]).makeNormalInitState();
    }

    qDebug()<<"start DOS";
    bool continueFlag=true;
    //выполняем несколько шагов WL на каждом walker'е. Каждый walker имеет свой критерий плоскости
    while (continueFlag){
        //analys
        ofstream f("analys.txt");
        f<<"#\tgap\tfrom\tto\tf"<<endl;
        for (unsigned w=0;w<walkers.size();w++){
            f<<
                walkers[w].number<<"\t"<<
                walkers[w].gap()<<"\t"<<
                walkers[w].from<<"\t"<<
                walkers[w].to<<"\t"<<
                walkers[w].f<<"\t"<<endl;
        }
        f.close();

        qDebug()<<"start step";
        continueFlag=false;
        for (unsigned w=0;w<walkers.size();w++){
            if (walkers[w].walk())
                continueFlag=true;
        }

        qDebug()<<"swap gaps";
        for (unsigned int gap1=0; gap1 < this->gaps-1; gap1++){
            //подбираем двух случайных walker из двух соседних блуждателей и переворачиваем
            unsigned int gap2=gap1+1;
            WangLandauParallelWalker *walker1=takeRandomFromGap(gap1), *walker2=takeRandomFromGap(gap2);
            this->swapWalkers(walker1,walker2);
        }
    }

    qDebug()<<"save to file";
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
    double ex = walker1->sys->calcEnergy1FastIncremental(eInit), ey = walker2->sys->calcEnergy1FastIncremental(eInit);

    double p = (walker1->getG(ex)+walker2->getG(ey)) - (walker1->getG(ey)+walker2->getG(ex));

    if (double(config::Instance()->rand()) / double(config::Instance()->rand_max) <= p){
        qDebug()<<"swap "<<walker1->number<<" and "<<walker2->number;
        unsigned tempGap = walker1->gap();
        walker1->setGap(walker2->gap());
        walker2->setGap(tempGap);
        return true;
    } else
        return false;
}

WangLandauParallelWalker *WangLandauParallel::takeRandomFromGap(unsigned int gapNumber)
{
    unsigned int randnum = Random::Instance()->next(walkersByGap)+1;
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

