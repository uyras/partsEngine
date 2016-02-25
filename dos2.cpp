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
    for (unsigned i=0; i<data.size()-1; i++){
            f<< val(i)<<"\t";
            f<< val(i+1)<<"\t";
            f<< data[i] <<endl;
    }


    f<< val(data.size()-1)<<"\t";
    f<< max <<"\t";
    f<< data[data.size()-1] <<endl;

    f.close();
}

void Dos2::load(string file)
{
    ifstream f(file);
    data.clear();
    double a=0, b=0, c=0;
    f>>a; f>>b; f>>c;
    data.push_back(c);
    this->min = a;
    while (f >> a >> b >> c){
        data.push_back(c);
    }
    this->max = b;
    this->intervals = data.size();
    f.close();
}

string Dos2::toString()
{
    stringstream f;
    for (unsigned i=0; i<data.size()-1; i++){
            f<< val(i)<<"\t";
            f<< val(i+1)<<"\t";
            f<< data[i] <<endl;
    }


    f<< val(data.size()-1)<<"\t";
    f<< max <<"\t";
    f<< data[data.size()-1] <<endl;
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

double& Dos2::at(unsigned i)
{
    return data[i];
}

double& Dos2::operator[](double a)
{
    return data[num(a)];
}
