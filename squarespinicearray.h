#ifndef SQUARESPINICEARRAY_H
#define SQUARESPINICEARRAY_H

#include "PartArray.h"
#include "squarespinicecell.h"
#include "Vect.h"

struct oneCell{
    Part
    *top,
    *bottom,
    *left,
    *right;
    Vect pos;
    int column,row;
    int type;
};

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

    bool setToGroundState();
    bool setToMaximalState();

    vector<SquareSpinIceCell*> cells;
};

#endif // SQUARESPINICEARRAY_H
