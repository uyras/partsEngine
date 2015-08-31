#ifndef HONEYCOMBSPINICECELL_H
#define HONEYCOMBSPINICECELL_H

#include "Part.h"
#include "Vect.h"
#include <vector>

using namespace std;
class HoneycombSpinIceCell
{
public:
    HoneycombSpinIceCell();
    ~HoneycombSpinIceCell();


    void rotateClockWise();
    void rotateAntiClockWise();
    void rotateChaotic();

    vector<Part*> parts;
    Vect pos;
};

#endif // HONEYCOMBSPINICECELL_H
