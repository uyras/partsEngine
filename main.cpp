#include <iostream>
#include <fstream>
#include <cstdlib>
#include "config.h"
#include <ctime>

#include "StateMachine.h"
#include "StateMachineFree.h"
#include "PartArray.h"
#include "Part.h"

using namespace std;

void moveSystemMRandomly(PartArray* sys, double fi){
    if (config::Instance()->U2D){
        double side = 1.;
        double oldFi;

        for(int i=0;i<sys->count();i++){
            double oldLen = sys->parts[i]->m.length();
            if ((double)config::Instance()->rand()/(double)config::Instance()->rand_max>0.5)
                side = -1.;
            else
                side = 1.;
            oldFi = sys->parts[i]->m.angle();
            double longitude = oldFi+(fi*side);
            sys->parts[i]->m.x = oldLen * cos(longitude);
            sys->parts[i]->m.y = oldLen * sin(longitude);
            sys->parts[i]->m.z = 0;
        }
    } else {
        for(int i=0;i<sys->count();i++){
            Part* temp = sys->parts[i];
            Vect ox,oy,oz,newV;
            //1. нормализуем вектор частицы, считаем его длину
            oz = temp->m.normalize();

            //2. генерируем ортонормированный базис, где oz-магнитный момент частицы
            oy = Vect::crossProduct(oz,Vect(0.5,0,0));
            ox = Vect::normal(oy,oz);
            oy = Vect::normal(ox,oz);

            //3. генерируем направление сдвига вектора
            double longitude = ((double)config::Instance()->rand()/(double)config::Instance()->rand_max) * 2. * M_PI; //[0;2pi]

            //4. получаем положение вдоль оси z
            //double lattitude=(double)config::Instance()->rand()/(double)config::Instance()->rand_max*(1-cos(fi))+cos(fi); //[cos(fi);1]
            double lattitude=cos(fi); //задано параметром функции, cos(fi)

            //5. получаем сдвинутый вектор
            newV = oz*lattitude + (ox*cos(longitude) + oy*sin(longitude)) * sqrt(1-lattitude*lattitude);
            newV *= temp->m.length();
            temp->m = newV;
        }
    }
}

int main(){
    config::Instance()->set3D();

    config::Instance()->srand(time(NULL));
    PartArray *sys1, *sys2, *ex;
    ex = new PartArray(3,3,3);
    ex->dropChain();
    ex->savePVPython("2.py");
    StateMachineFree init(ex->state);
    //for (int i=0;i<10;i++){
        sys1 = ex->copy();
        moveSystemMRandomly(sys1,10.*(M_PI/180.));
        sys2 = sys1->copy();
        sys1->setToPTGroundState(20,2000);
        sys1->state->draw();
        sys2->state->draw();
        sys1->savePVPython("1.py");
        sys2->savePVPython("2.py");
        if (!(*sys1->state == init)){
            cout<<"ERR"<<endl<<endl;
        } else
            cout<<"fine"<<endl<<endl;

        delete sys1, sys2;
    //}
    //sys1->setMAllUp();
    //sys1->savePVPython("2.py");

    cout<<"finish";
    return 0;
}
