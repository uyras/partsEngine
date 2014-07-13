
/*
* File:   Vect.h
* Author: uyras
*
* Created on 20 Ноябрь 2012 г., 15:55
*/
#pragma once
class Vect {
public:
    double x, y, z; //основные параметры векторов

    double scalar(Vect); //Скалярное произведение двух векторов

    double space(Vect b); //расстояние между векторами
    Vect radius(Vect b); //возвращает радиус-вектор
    double length(); //длина вектора
    void setXYZ(double x, double y, double z); //задать вектор
    void rotate(); //вращать вектор на 180 градусов

    void toAbs(); //переводит вектор в абсолютные величины из относительных

    void toRel(); //переводит вектор в относительные величины из абсолютных (не точно, что работает)
    void draw(); //выводит вектор на экран

	void setUnit(); //делает вектор едииничной длины
    Vect normalize(); //возвращает нормализованный вектор

    double angle(); //возвращает угол наклона полярной системы координат в радианах, работает только в 2D
    double grade(); //возвращает угол наклона полярной системы координат в градусвх

    Vect(); //конструктор нулевого вектора
    Vect(double x, double y, double z); //конструктор направленного вектора
    Vect operator=(const Vect&); //присваивание вектора
    Vect operator+=(const Vect&); //оператор сложения
    Vect operator+(const Vect&);
    Vect operator*(const double num);
    Vect operator*=(const double num);
    Vect operator/(const double num);
    Vect operator/=(const double num);
    static Vect crossProduct(const Vect& vect1, const Vect& vect2);  //векторное произведение двух векторов
    static Vect normal(const Vect& vect1, const Vect& vect2); //нормаль к плоскости векторов
};

