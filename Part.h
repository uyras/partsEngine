/*
 * File:   Part.h
 * Author: uyras
 * класс частицы
 * Created on 20 Ноябрь 2012 г., 15:50
 */

#ifndef PART_H
#define PART_H

#include <vector>
#include "Vect.h"

class PartArray;

class Part {
public:
    friend PartArray;
    enum form{CIRCLE,ELLIPSE,SQUARE};

    Vect pos; //координаты, для удобства в относительных величинах
    Vect m; //магнитный момент (координаты вектора относительно центра частицы)
    Vect h; //поле взаимодействия
    double e; //энергия частицы (произведение HM)
    std::vector<double> eArray;
    double r; //радиус частицы (для диполя минимальное расстояние, на которое может приближаться система
    double _vol; //объем частицы (для 2D - площадь)
    unsigned short int sector; //сектор образца, в котором расположена частица

    bool state; //состояние частицы. Если 0 то состояние начальное
    form shape; //форма частицы
    double w1,h1; //высота и ширина частицы

    Part();
    virtual ~Part();
    Vect interact(Part* elem); //считает поле взаимодействия частицы elem на текущую частиуц и возвращает вектор H
    void rotate(float angle=180.); //вращаем магнитный момент частицы и меняем ее состояние
    Part* copy();
    double volume(); //Возвращает объем частицы в относительных единицах

    int Id();

protected:
    unsigned int id;
    Part(unsigned int id); //конструктор с установленным ИДом разрешен только для дружественных классов
};

#endif // PART_H
