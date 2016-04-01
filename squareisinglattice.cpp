#include "squareisinglattice.h"

SquareIsingLattice::SquareIsingLattice():
    SquareLattice()
{
    this->SquareLattice::setInteractionRange(1.1);
    this->SquareLattice::hamiltonianIsing();
}

void SquareIsingLattice::dropSquareLattice(int m, int n)
{
    this->dropSquareLattice(m,n);
//замыкаем соседей
    Part* temp1, *temp2;
    for (int i=0; i<m; i++){
        temp1 = this->at(i,0);
        temp2 = this->at(i,n-1);
        neighbours[temp1->Id()].push_front(temp2);
        neighbours[temp2->Id()].push_front(temp1);
    }
    for (int i=0; i<n; i++){
        temp1 = this->at(0,i);
        temp2 = this->at(m-1,i);
        neighbours[temp1->Id()].push_front(temp2);
        neighbours[temp2->Id()].push_front(temp1);
    }
}
