#ifndef WANGLANDAUPARALLELWALKER_H
#define WANGLANDAUPARALLELWALKER_H

#include "PartArray.h"
#include "wanglandau.h"
#include "wanglandauparallel.h"

#include <iostream>
#include <QString>

class WangLandauParallel;

class WangLandauParallelWalker
{
public:
    friend class WangLandauParallel;
    WangLandauParallelWalker(PartArray *system,
            unsigned int intervals,
            double eMin,
            double eMax,
                             double overlap,
                             unsigned int gaps,
            double gap,
            int number
            );
    ~WangLandauParallelWalker();

    void setGap(unsigned int gap);
    unsigned int gap();
    void getRangeFrom();
    void getRangeTo();
    double getG(double e);

    /**
     * @brief walk Проводит 100 WL-шагов до тех пор, пока f не достигнет fmin
     * @return true если возможен следующий walk(), false если невозможен
     */
    bool walk();

    /**
     * @brief getIntervalNumber Получить номер интервала
     * @param Energy
     * @return
     */
    unsigned int getIntervalNumber(double Energy);

    bool isFlat();
protected:
    PartArray *sys;
    unsigned intervals;
    double eMin, eMax, dE, eInit, gaps;
    int number;
    double from,to;
    double overlap;
    unsigned int gapNumber,stepsPerWalk;
    double f,fMin, accuracy;

    vector<double> h,g;//g - логарифм плотности состояний (энтропия), h - вспомогательная гистограмма, которая должна быть плоской
};

#endif // WANGLANDAUPARALLELWALKER_H
