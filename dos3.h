#ifndef DOS3_H
#define DOS3_H

#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <cmath>

using namespace std;

class Dos3
{
public:
    Dos3(double min, double max, unsigned interval);

    int operator[](std::pair<double,double> a);
    void reg (double a, double b);
    void add (double a, double b, double c);
    void set(double a, double b, double c);
    void save(string file);
    string toString();

    unordered_map<double,double> at(unsigned i);
    double at(unsigned i, double j);

    inline double val(unsigned i){ return min + (max-min)*((double)i/(double)(intervals-1)); }

    inline unsigned num(double a){
        if (a<min)
            return 0;
        if(a>max)
            return intervals-1;
        unsigned r= (unsigned)floor(((a-min)*(double)intervals-1)/(max-min));
        if (r>intervals-1)
            return unsigned(intervals-1);
        else
            return r;
    }

private:
    double min, max;
    unsigned intervals;
    vector< unordered_map<double,double> > data;
};

#endif // DOS3_H
