#ifndef HONEYCOMBSPINICEARRAY_H
#define HONEYCOMBSPINICEARRAY_H

#include "PartArray.h"
#include "honeycombspinicecell.h"
#include "sysloader.h"

class HoneycombSpinIceArray : public PartArray
{
public:
    HoneycombSpinIceArray();
    ~HoneycombSpinIceArray();

    /**
     * @brief dropHoneyComb Создать решетку гексагональной формы
     * @param m Количество ячеек по горизонтали
     * @param n Количество ячеек по вертикали
     * @param a Расстояние между двумя соседними частицами honeycomb решетки
     * @param tmp Шаблон добавляемой частицы. Будут перезаписываться координаты и направление магнитного момента (длина сохраняется)
     */
    void dropHoneyComb(int m, int n, double a, Part * tmp = 0);

    StateMachineFree groundState();
    StateMachineFree maximalState();

    virtual void clear();

    virtual void load(QString file);
    virtual void save(QString file);

    PartArray* beforeCopy();
    void afterCopy(PartArray* temp2);

    vector<HoneycombSpinIceCell*> cells;

private:
    int m,n;
    void clearCells();
};

#endif // HONEYCOMBSPINICEARRAY_H
