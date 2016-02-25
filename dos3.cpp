#include "dos3.h"

Dos3::Dos3(double min, double max, unsigned interval):
    min(min),
    max(max),
    intervals(interval)
{
    data.resize(intervals);
}

int Dos3::operator[](std::pair<double, double> a)
{
    return data[num(a.first)][a.second];
}

void Dos3::reg(double a, double b)
{
    data[num(a)][b]+=1;
}

void Dos3::add(double a, double b, double c)
{
    data[num(a)][b]+=c;
}

void Dos3::set(double a, double b, double c)
{
    data[num(a)][b]=c;
}

void Dos3::save(string file)
{
    ofstream f(file);
    for (unsigned i=0; i<data.size(); i++){
        for(pair<double,double> d : data[i]){
            f<< val(i)<<"\t";
            f<< d.second<<"\t"<<d.first<<endl;
        }
    }
    f.close();
}

string Dos3::toString()
{
    stringstream f;
    for (unsigned i=0; i<data.size(); i++){
        for(pair<double,double> d : data[i]){
            f<< val(i)<<"\t";
            f<< d.second<<"\t"<<d.first<<endl;
        }
    }
    return f.str();
}

unordered_map<double, double> Dos3::at(unsigned i)
{
    return data[i];
}

double Dos3::at(unsigned i, double j)
{
    return data[i][j];
}
