#ifndef WANGLANDAUPARALLEL_H
#define WANGLANDAUPARALLEL_H

#include "wanglandau.h"
#include "wanglandauparallelwalker.h"
#include "PartArray.h"
#include "random.h"

#include <QDebug>
#include <cmath>
#include <vector>
#include <iostream>
#include <omp.h>

using namespace std;
class WangLandauParallelWalker;
class WangLandauParallel
{
public:
    /**
     * @brief WangLandauParallel Базовый конструктор Ванг-Ландау метода
     * @param system Исследуемая система
     * @param intervals Количество интервалов, на которые делится все пространство состояний
     * @param gaps Число энергетических интервалов
     * @param walkersByGap Количество простых блуждателей на каждый интервал
     */
    WangLandauParallel(PartArray *system, unsigned int intervals, unsigned int gaps=10, double overlap=0.75, unsigned int walkersByGap=5);
    ~WangLandauParallel();

    vector<double> dos();


private:
    bool swapWalkers(WangLandauParallelWalker *walker1, WangLandauParallelWalker *walker2);
    WangLandauParallelWalker * takeRandomFromGap(unsigned int gapNumber);

    PartArray *sys;
    vector<WangLandauParallelWalker> walkers;
    double eMin,eMax;
    double eInit;
    unsigned int intervals, gaps, walkersByGap;
    double overlap;
};

#endif // WANGLANDAUPARALLEL_H
