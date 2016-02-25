#include "dos2.h"

Dos2::Dos2(double min, double max, unsigned intervals):
    min(min),
    max(max),
    intervals(intervals)
{
    data.resize(intervals);
}

void Dos2::save(string file)
{
    ofstream f(file);
    for (unsigned i=0; i<data.size(); i++){
            f<< val(i)<<"\t";
            f<< data[i] <<endl;
    }
    f.close();
}

void Dos2::load(string file)
{
    ifstream f(file);
    data.clear();
    double a=0, b=0;
    f>>a; f>>b;
    data.push_back(b);
    this->min = a;
    while (f >> a >> b){
        data.push_back(b);
    }
    this->max = a;
    this->intervals = data.size();
    f.close();
}

string Dos2::toString()
{
    stringstream f;
    for (unsigned i=0; i<data.size(); i++){
            f<< min + val(i)<<"\t";
            f<< data[i] <<endl;
    }
    return f.str();
}

void Dos2::reg(double a)
{
    data[num(a)]+=1;
}

void Dos2::set(double a, double b)
{
    data[num(a)]=b;
}

void Dos2::add(double a, double b)
{
    data[num(a)]+=b;
}

double Dos2::at(unsigned i)
{
    return data[i];
}

int Dos2::operator[](double a)
{
    return data[num(a)];
}
