#ifndef DOS2_H
#define DOS2_H

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <cmath>
#include <unordered_map>

using namespace std;


template <typename T>
class Dos2
{
public:
    Dos2():min(0),max(0),intervals(0){}

    Dos2(double min, double max, unsigned intervals)
    {
        this->resize(min,max,intervals);
    }

    inline void resize(double min, double max, unsigned intervals){
        this->min = min;
        this->max= max;
        this->intervals = intervals;
        data.resize(intervals);
    }

    //virtual void save(string file);
    //virtual void load(string file);
    //string toString();

    inline T& at(unsigned i){return data[i];}
    inline const T& at(unsigned i) const {return data[i];}
    inline T& operator[](double a){return data[num(a)];}
    inline const T& operator[](double a) const {return data[num(a)];}

    inline unsigned Intervals() const {return intervals;}
    inline double Min() const {return this->min;}
    inline double Max() const {return this->max;}


    /**
     * @brief val Возращает значение, с которого начинается i интервал
     * @param i Интервал, для которого надо получить соответствующее double значение
     * @return
     */
    inline double val(unsigned i) const{ return min + (max-min)*((double)i/(double)(intervals)); }

    /**
     * @brief num Возращает номер интервала для соответствующего значения
     * @param a
     */
    inline unsigned num(double a) const{
        if (a<min)
            return 0;
        if(a>=max)
            return intervals-1;
        unsigned r= (unsigned)floor((a-min)/(max-min)*double(intervals));
        if (r>intervals-1)
            return unsigned(intervals-1);
        else
            return r;
    }

    /**
     * @brief clear Reset all values of DOS to 0
     */
    inline void clear(){
        for (auto i: data){
            i=0.;
        }
    }

protected:
    double min, max, delta;
    unsigned intervals;
    vector< T > data;
};

#endif // DOS2_H
