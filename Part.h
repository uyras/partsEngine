/*
 * File:   Part.h
 * Author: uyras
 * класс частицы
 * Created on 20 Ноябрь 2012 г., 15:50
 */

#pragma once
#include <vector>
#include "Vect.h"

class Part {
public:
    Vect pos; //координаты, для удобства в относительных величинах
    Vect m; //магнитный момент (координаты вектора относительно центра частицы)
    Vect h; //поле взаимодействия
    double e; //энергия частицы (произведение HM)
    std::vector<double> eArray;
    double volume; //объем частицы (для 2D - площадь)
    unsigned short int sector; //сектор образца, в котором расположена частица

    bool state; //состояние частицы. Если 0 то состояние начальное

    Part();
    ~Part();
    Vect interact(Part* elem); //считает поле взаимодействия частицы elem на текущую частиуц и возвращает вектор H
    void rotate(float angle=180.); //вращаем магнитный момент частицы и меняем ее состояние
    Part* copy();
};




