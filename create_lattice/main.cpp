#include <iostream>
#include <cmath>
#include <map>
#include "random.h"
#include "PartArray.h"
#include "config.h"
#include "hc_flower.h"
#include "triangularlattice.h"
#include "cagomelattice.h"
#include "squareisinglattice.h"



//double l=1.8; // параметр решетки
double l=1; // параметр решетки


double hamiltonianIsing2(Part *a, Part *b)
{
    if (a->m.scalar(b->m)>0)
        return -1;
    else
        return 1;
}

double hamiltonianDipolar2(Part *a, Part *b)
{
    double rijx = b->pos.x - a->pos.x,
        rijy = b->pos.y - a->pos.y,
        rijz = b->pos.z - a->pos.z, r2, r, r5,E;
    if (config::Instance()->dimensions()==2)
        r2 = rijx*rijx+rijy*rijy;
    else
        r2 = rijx*rijx+rijy*rijy+rijz*rijz;
    r = sqrt(r2); //трудное место, заменить бы
    r5 = r2 * r2 * r; //радиус в пятой
    if (config::Instance()->dimensions()==2)
        E = //энергия считается векторным методом, так как она не нужна для каждой оси
                (( (a->m.x * b->m.x + a->m.y * b->m.y) * r2)
                 -
                 (3 * (b->m.x * rijx + b->m.y * rijy) * (a->m.x * rijx + a->m.y * rijy)  )) / r5;
    else
        E = //энергия считается векторным методом, так как она не нужна для каждой оси
                (( (a->m.x*b->m.x+a->m.y*b->m.y+a->m.z*b->m.z) * r2)
                 -
                 (3 * (b->m.x * rijx + b->m.y * rijy + b->m.z * rijz) * (a->m.x * rijx + a->m.y * rijy + a->m.z * rijz)  )) / r5;
    return E;
}

int main(int argc, char *argv[])
{
    config::Instance()->set2D();
    //SquareIsingLattice sys;
    //sys.setHamiltonian(hamiltonianDipolar2);
    //sys.dropSymmetricLattice(l,Vect(2,0,0));
    //sys.dropSquareLattice(4,4);
    //cout<<"size:"<<sys.size()<<endl;
    //cout<<"neighbours[0].Id:"<<sys.neighbours[1].Id();<<endl;
    HC_Flower sys;
    sys.dropSymmetricLattice(1,Vect(2,0,0));

    sys.csv("new_csv.csv", hamiltonianDipolar2);
    cout<<sys.size();




//    ofstream f("lattice.csv");
//    for (Part* p1 : sys.parts){
//        for (Part* p2: sys.parts){
//            if (sys.isNeighbours(p1,p2)){
//                    f<<hamiltonianDipolar2(p1,p2);
//            }
//            int qwert=sys.parts.size();
//            long IDqwert=p2->Id();
//            if(IDqwert<qwert-1)
//            f<<";";
//        }
//        f<<endl;
//     }
// f.close();


 sys.save("lattice.mfsys");


 //ofstream fq("Bruteforce_HC_lattice.csv");
 //fq<< sys.state.toString()<<";"<<sys.E()<<";"<<sys.M()<<endl;

// double minimalE=0;
// double maximumE=-10000000;
// bool bruteforce=0;

//  if(bruteforce){

//      while (sys.state.halfNext()) {
//          if(sys.E()<minimalE){
//              minimalE=sys.E();
//              fq<<sys.E()<<";"<<sys.M()<<endl;
//          }
//          if(sys.E()>maximumE){
//              maximumE=sys.E();
//              fq<< sys.state.toString()<<";"<<sys.E()<<";"<<sys.M()<<endl;
//          }

//      }
//      fq<<"!!!minimalE="<<minimalE;
//      fq<<"!!!maximumE="<<maximumE;
//  }
//  cout<<"finish!"<<endl;


}
