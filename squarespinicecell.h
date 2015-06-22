#ifndef SQUARESPINICECELL_H
#define SQUARESPINICECELL_H

#include <vector>
#include <iostream>
#include "Part.h"

using namespace std;
/**
 * @brief The SpinIceCell class Одна ячейка спинового льда
 */
class SquareSpinIceCell
{
public:
    SquareSpinIceCell();
    virtual ~SquareSpinIceCell();

    int type();

    Part
    *top,
    *bottom,
    *left,
    *right;
    Vect pos;
    int column,row;

private:
    struct oneCell{
        Vect
        top, bottom, left, right;
        unsigned short int type;
    };

    void makeTypes();
    vector<oneCell> types;
};

#endif // SQUARESPINICECELL_H
