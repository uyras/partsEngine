#ifndef SQUARELATTICE_H
#define SQUARELATTICE_H

#include "PartArray.h"
#include "Part.h"
#include "sysloader.h"


class SquareLattice : public PartArray
{
public:
    SquareLattice();

    /**
     * @brief dropSquareLattice Метод генерации массива квадратной решетки
     * @param m Количество частиц по горизонтали
     * @param n Количество частиц по вертикали
     * @param l Расстояние между соседними решетками
     * @param len длина вектора намагниченности по умолчанию
     * @param example Шаблон добавляемой частицы. Будут перезаписываться координаты и направление магнитного момента (длина сохраняется)
     */
    void dropSquareLattice(int m, int n, double l=1, double len=1, Part * example=0);

    StateMachineFree groundState();
    StateMachineFree maximalState();

    virtual void load(QString file);
    virtual void save(QString file);

private:
    int m,n;
    double l;
};

//SysLoader::reg(new SquareLattice());

#endif // SQUARELATTICE_H
