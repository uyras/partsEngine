#ifndef PART_H
#define PART_H

#include <vector>
#include "Vect.h"
#include "PartArray.h"

class PartArray;
class Part {
public:
    friend PartArray;
    enum form{CIRCLE,ELLIPSE,SQUARE};

    Vect pos; //координаты, для удобства в относительных величинах
    Vect m; //магнитный момент (координаты вектора относительно центра частицы)
    Vect h; //поле взаимодействия
    double e; //энергия частицы (произведение HM)
    double r; //радиус частицы (для диполя минимальное расстояние, на которое может приближаться система
    double _vol; //объем частицы (для 2D - площадь)
    unsigned short int sector; //сектор образца, в котором расположена частица

    bool state; //состояние частицы. Если 0 то состояние начальное
    form shape; //форма частицы
    double w1,h1; //высота и ширина частицы

    Part();
    Part(const Part& obj); //конструктор копирования
    bool operator==(const Part& one) const;
    inline bool operator!=(const Part& one) const{ return !this->operator ==(one); }

    virtual ~Part();
    Vect interact(Part* elem); //считает поле взаимодействия частицы elem на текущую частиуц и возвращает вектор H
    void rotate(float angle=180.); //вращаем магнитный момент частицы и меняем ее состояние
    double volume(); //Возвращает объем частицы в относительных единицах

    inline long int Id(){
        return this->id;
    }

protected:
    std::vector<Part*> neighbours;
    std::vector<double> eArray;
    long int id;
    Part(unsigned int id); //конструктор с установленным ИДом разрешен только для дружественных классов
};

#endif // PART_H
