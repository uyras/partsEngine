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

    //Создаем файл с энергиями
    ofstream f("energies.txt");
    double x = (this->eMax-this->eMin)/((double)gaps*(1.-overlap)+overlap); //ширина перекрытия
    for (unsigned int i=0;i<gaps;i++){
        double from=this->eMin + (double)i * x * (1.-overlap);
        f<<from<<endl<<from+x<<endl;
    }
    f.close();

    qDebug()<<"Finish creating walkers";
}

WangLandauParallel::~WangLandauParallel()
{

}

vector<double> WangLandauParallel::dos()
{
    for (unsigned i=0;i<walkers.size();i++){
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
        for (unsigned w=0;w<walkers.size();w++){
            walkers[w].walk(1000);
        }

        qDebug()<<"swap configurations";
        for (unsigned int gap1=0; gap1 < this->gaps-1; gap1++){
            //подбираем двух случайных walker из двух соседних блуждателей и переворачиваем
            unsigned int gap2=gap1+1;
            WangLandauParallelWalker *walker1=takeRandomFromGap(gap1), *walker2=takeRandomFromGap(gap2);
            this->swapWalkers(walker1,walker2);
        }


        unsigned int finished = 0;
        for (unsigned int gap=0; gap < this->gaps; gap++){
            vector<WangLandauParallelWalker*> gapwalkers = getByGap(gap);
            unsigned flatted=0;
            for (unsigned w=0;w<gapwalkers.size();w++){
                if (gapwalkers[w]->isFlat())
                    flatted++;
            }

            if (flatted==gapwalkers.size()){ //если все блуждатели плоские, уменьшаем f, обнуляем h и усредняем g
                averageHistogramms(gap);
                for (unsigned w=0;w<gapwalkers.size();w++){
                    if (gapwalkers[w]->processWalk())
                        finished++;
                }
            }
        }

        //qDebug()<<finished<<" of "<<walkers.size()<<" walkers has already finished";

        //если все блуждатели завершили работу, сохраняем результат и выходим
        if (finished==walkers.size()){
            continueFlag=false;

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
        }

    }

    return walkers[0].g;
}

bool WangLandauParallel::swapWalkers(WangLandauParallelWalker *walker1, WangLandauParallelWalker *walker2)
{
    double ex = walker1->sys->calcEnergy1FastIncremental(walker1->eInit), ey = walker2->sys->calcEnergy1FastIncremental(walker2->eInit);
    //double ex = walker1->sys->calcEnergy1(), ey = walker2->sys->calcEnergy1();

    double p = (walker1->getG(ex)+walker2->getG(ey)) - (walker1->getG(ey)+walker2->getG(ex));

    double randnum = Random::Instance()->nextDouble();
    if (0.0==randnum) //логарифма нуля не существует
        randnum=10e-20;
    randnum = std::log(randnum);

    StateMachineFree tempState;

    if ( randnum <= p){
        //qDebug()<<"swap "<<walker1->number<<" ("<<walker1->gapNumber<<")"<<" and "<<walker2->number<<" ("<<walker2->gapNumber<<")";

        tempState = *(walker1->sys->state);
        *(walker1->sys->state)=*(walker2->sys->state);
        *(walker2->sys->state)=tempState;

        walker1->updateGH(ey);
        walker2->updateGH(ex);
        return true;
    } else
        return false;
}

WangLandauParallelWalker *WangLandauParallel::takeRandomFromGap(unsigned int gapNumber)
{
    return this->getByGap(gapNumber)[Random::Instance()->next(walkersByGap)];
}

void WangLandauParallel::averageHistogramms(unsigned gapNumber)
{
    vector<WangLandauParallelWalker*> w;
    double average=0;
    w = getByGap(gapNumber);
    for (unsigned i=0;i<this->intervals;i++){
        average=0;
        for (unsigned j=0;j<walkersByGap;j++){
            average = (average * (double)j + w[j]->g[i]) / (double)(j+1);
        }

        for (unsigned j=0;j<walkersByGap;j++){
            w[j]->g[i] = average;
        }
    }
}

vector<WangLandauParallelWalker *> WangLandauParallel::getByGap(unsigned gapNumber)
{
    vector<WangLandauParallelWalker*> w;
    vector<WangLandauParallelWalker>::iterator iter = this->walkers.begin();
    while (w.size()<walkersByGap && iter!=walkers.end()){
        if ((*iter).gapNumber==gapNumber)
            w.push_back(&(*iter));
        iter++;
    }
    return w;
}

