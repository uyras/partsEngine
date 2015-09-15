#ifndef RANDOM_H
#define RANDOM_H

#define _USE_MATH_DEFINES true
#include <math.h>
#include <cstdlib>
#include <ctime>
#include <iostream>

using namespace std;

class Random
{
public:

    //паттерн синглтон
    static Random *Instance(int srand=0);

    //установка режимов random
    void standart();

    /**
     * @brief next Cлучайное число в интервале от 0 (включительно) до maxValue (исключительно)
     * @param maxValue Максимальное значение случайного числа.
     * @return Возвращает случайное число. Например, если maxValue=3, то вернет одно из чисел 0,1,2
     */
    int next(const int maxValue);

    /**
     * @brief next Cлучайное число в интервале от minValue (включительно) до maxValue (исключительно)
     * @param minValue Минимальное значение случайного числа.
     * @param maxValue Максимальное значение случайного числа.
     * @return озвращает случайное число. Например, если minValue=1, maxValue=4, то вернет одно из чисел 1,2,3
     */
    int next(const int minValue,const int maxValue);


    int next();
    double nextDouble();

    unsigned long int max();
    void srand(int value);

private:
    static Random* _self;
    Random(int srand);
    virtual ~Random();

    /*режим рэндома
    0 - стандартный
    1 - нативный
    */
    int _randmode;
    unsigned long int randmax;
};

#endif // RANDOM_H
