#ifndef SQUARESPINICEARRAY_H
#define SQUARESPINICEARRAY_H

#include "PartArray.h"
#include "squarespinicecell.h"
#include "Vect.h"
#include <QtDebug>


class SquareSpinIceArray : public PartArray
{

public:
    SquareSpinIceArray();
    virtual ~SquareSpinIceArray();

    void dropSpinIce(int hCells, int vCells, double l=1);

    /**
     * @brief findByPosition Найти частицу по ее позиции
     * @param pos Позиция частицы, которую надо найти
     * @param epsilon Точность поиска
     * @return
     */
    Part* findByPosition(const Vect& pos, double epsilon=1e-15);

    double setToGroundState();
    double setToMaximalState();

    vector<SquareSpinIceCell*> cells;

    PartArray* beforeCopy();
    void afterCopy(PartArray *temp2);

    virtual void clear();

    virtual void load(QString file);
    virtual void save(QString file);

private:
    void clearCells(); //чистим информацию о ячейках
};

#endif // SQUARESPINICEARRAY_H
