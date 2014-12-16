
#ifndef H_CONFIG
#define H_CONFIG

#define _USE_MATH_DEFINES true
#include <math.h>
#include <fstream>
#include <cstdlib>
#include <iostream>

class config
{
public:
    bool U2D; //use 2 directions, если true - то используется плоскость, иначе пространство
    double
        I0s, //намагниченность насыщения в гаусс,
        hc, //критическое поле в эрстедах,
        partR, //радиус частицы в нанометрах
        m, //длина вектора магнитного момента частицы
        vol, //объем частицы как сферы, в нанометрах
        dstep, //параметр решетки в сантиметрах (множитель)
        kv; //перевод в сантиметры (для отображения)
    int
        size, //количество потоков MPI
        rank; //номер текущего потока MPI
    int rand_max;


    //переключаем режимы симуляции
    void set2D();
    void set3D();

    //функции рэндома
    int rand(); //генерация рэндом-числа
    void srand(int); //установка начальной точки рэндома
    void randmode_file(const char* file,int srand = 0); //переключить режим ГПСЧ в считывание последовательности с файла
    void randmode_standart(int srand = 0); //переключить в СИ-шный режим рэндома



    //паттерн синглтон
    static config* Instance();


private:
    static config* _self;
    config();
    ~config(){};

    void rand_file_open(const char* file);
    void rand_file_close();

    /*режим рэндома
    0 - стандартный
    1 - нативный
    */
    int _randmode;
    std::ifstream _randfile;
};

#endif
