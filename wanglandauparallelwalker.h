#ifndef WANGLANDAUPARALLELWALKER_H
#define WANGLANDAUPARALLELWALKER_H

#include "PartArray.h"
#include "wanglandau.h"
#include "wanglandauparallel.h"
#include "random.h"

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
     * @return true если гистограмма плоская, следующий walk(), false если невозможен
     */
    void walk(unsigned stepsPerWalk); //число шагов на одно блуждание

    /**
     * @brief processWalk Выполняется только когда h стала плоской
     */
    void processWalk();

    /**
     * @brief getIntervalNumber Получить номер интервала
     * @param Energy
     * @return
     */
    unsigned int getIntervalNumber(double Energy);
    double getEnergyByInterval(unsigned int interval);

    bool isFlat(); //true если g(E) - плоская
    bool finished(); //true если f меньше порогового значения

    void makeNormalInitState();

    void updateGH(double E=0); //обновляет гистограммы g и h, обновляет среднее значение гистограмм

    double calcAverageH(); //считает среднее H

    bool inRange(double E);//Входит ли число в интервал
protected:
    PartArray *sys;
    unsigned intervals;
    double eMin, eMax, dE, eInit, gaps;
    int number;
    double from,to;
    unsigned nFrom,nTo;//номера интервалов ОТ и ДО
    double overlap;
    unsigned int gapNumber;
    double f,fMin, accuracy;

    double average; //подсчитывает среднее число для h
    unsigned hCount; //количество ненулевых элементов h, нужно для подсчета среднего

    vector<double> h,g;//g - логарифм плотности состояний (энтропия), h - вспомогательная гистограмма, которая должна быть плоской
};

#endif // WANGLANDAUPARALLELWALKER_H
