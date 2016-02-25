#ifndef DOS_H
#define DOS_H

#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <cmath>

using namespace std;

class Dos2
{
public:
    Dos2(double min, double max, unsigned intervals);

    void save(string file);
    void load(string file);
    string toString();
    void reg(double a);
    void set(double a, double b);

    //добавить значение к DOS
    void add (double a, double b);

    double at(unsigned i);

    inline unsigned Intervals() const {return intervals;}
    inline double Min() const {return this->min;}
    inline double Max() const {return this->max;}

    int operator[](double a);

    inline double val(unsigned i){ return min + (max-min)*((double)i/(double)(intervals-1)); }

    inline unsigned num(double a){
        if (a<min)
            return 0;
        if(a>max)
            return intervals-1;
        unsigned r= (unsigned)floor(((a-min)*(double)(intervals-1))/(max-min));
        if (r>intervals-1)
            return unsigned(intervals-1);
        else
            return r;
    }

private:
    double min, max;
    unsigned intervals;
    vector< double > data;
};

#endif // DOS_H
