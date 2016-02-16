#include "dos.h"

Dos::Dos(double min, double max, unsigned intervals):
    min(min),
    max(max),
    intervals(intervals)
{
    data.resize(intervals);
}

void Dos::save(string file)
{
    ofstream f(file);
    for (unsigned i=0; i<data.size(); i++){
        for(pair<double,int> d : data[i]){
            f<< min + (max-min)*((double)i/(double)intervals)<<"\t";
            f<< d.second<<"\t"<<d.first<<endl;
        }
    }
    f.close();
}

string Dos::toString()
{
    stringstream f;
    for (unsigned i=0; i<data.size(); i++){
        for(pair<double,int> d : data[i]){
            f<< min + (max-min)*((double)i/(double)intervals)<<"\t";
            f<< d.second<<"\t"<<d.first<<endl;
        }
    }
    return f.str();
}

void Dos::reg(double a, double b)
{
    data[num(a)][b]+=1;
}

void Dos::add(double a, double c)
{
    this->add(a,0.,c);
}

void Dos::add(double a, double b, double c)
{
    data[num(a)][b]+=c;
}

int Dos::operator[](double a)
{
    return data[a][0.];
}

int Dos::operator[](pair<double,double> a)
{
    return data[a.first][a.second];
}
