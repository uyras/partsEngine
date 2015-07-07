#ifndef WANGLANDAU_H
#define WANGLANDAU_H

#include "PartArray.h"
#include "StateMachineFree.h"

#include <QDebug>
#include <cmath>

using namespace std;

class WangLandau
{
public:
    static vector<double> dos(PartArray & sys, const int intervals=1000, const int steps=10000, const double accuracy=0.8);
    static vector<double> scale(PartArray & sys, const int intervals=1000);


    static unsigned int getIntervalNumber(double Energy, const double eMin, const double dE);
    static void setValues(vector<double> & histogramm, double value);
    static bool isFlat(vector<double> &histogramm, const double accuracy);
    static void normalize(vector<double> &histogramm);


    WangLandau();
    ~WangLandau();
};

#endif // WANGLANDAU_H
