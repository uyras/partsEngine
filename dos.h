#ifndef DOS_H
#define DOS_H

#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <cmath>

using namespace std;

class Dos
{
public:
    Dos(double min, double max, unsigned intervals);

    void save(string file);
    string toString();
    void reg(double a, double b=0.);

    int& operator[](double a);
    int& operator[](std::pair<double,double> a);

private:
    inline unsigned num(double a){
        unsigned r= (unsigned)floor(((a-min)*(double)intervals)/(max-min));
        if (r>intervals-1)
            return unsigned(intervals-1);
        else
            return r;
    }
    double min, max;
    unsigned intervals;
    vector< unordered_map<double,int> > data;
};

#endif // DOS_H
