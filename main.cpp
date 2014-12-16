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

void moveSystemPosRandomly(PartArray* sys, double d){
    Vect dir; //направление, в которое двигать частицу.
    for(int i=0;i<sys->count();i++){
        double longitude = ((double)config::Instance()->rand()/(double)config::Instance()->rand_max) * 2. * M_PI;
        double lattitude;
        if (config::Instance()->U2D)
            lattitude=0; // если частица 2-х мерная то угол отклонения должен быть 0
        else
            lattitude=(double)config::Instance()->rand()/(double)config::Instance()->rand_max*2.-1.;
        dir.x = d * cos(longitude) * sqrt(1-lattitude*lattitude);
        dir.y = d * sin(longitude) * sqrt(1-lattitude*lattitude);
        dir.z = 0 * lattitude;
        sys->parts[i]->pos += dir;
    }
}

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
    /*3D
    config::Instance()->set3D();

    config::Instance()->srand(time(NULL));

    int x=3, y=3, z=3; //количество частиц в линейке
    double space = config::Instance()->partR*4.;//расстояние между центрами частиц
    double dMax = space/2.-config::Instance()->partR;

    PartArray *sys1, *sys2,*sys3, *example;
    example = new PartArray((double)x*space,(double)y*space,(double)z*space);//размер образца 4 радиуса, или 2 диаметра

    //бросаем частицы в шахматном порядке на линию и запоминаем состояние
    example->dropChain(space);
    StateMachineFree oldState(example->state);

    sys1 = example->copy();
    moveSystemMRandomly(sys1,85.*(M_PI_2/90.));
    example->savePVPythonAnimation(sys1,"3_180_grades.py",50,50);

    sys2 = sys1->copy();
    sys2->setToPTGroundState(20,3000);
    sys1->savePVPythonAnimation(sys2,"3_180_grades_gs.py",50,50);

    sys1 = example->copy();
    moveSystemPosRandomly(sys1,dMax);
    example->savePVPythonAnimation(sys1,"3_180_pos.py",50,50);

    sys2 = sys1->copy();
    sys2->setToPTGroundState(20,3000);
    sys1->savePVPythonAnimation(sys2,"3_180_pos_gs.py",50,50);
*/

    config::Instance()->set2D();
    config::Instance()->srand(time(NULL));
    int n=3, m=7, //количество частиц в линейке
            experimentCount=1000;
    double intervalCount = 100.;
    double space = config::Instance()->partR*4.;//расстояние между центрами частиц
    double dMax = space/2.-config::Instance()->partR;

    PartArray *sys1, *sys2,*sys3, *example;
    example = new PartArray((double)n*space,(double)m*space,1);//размер образца 4 радиуса, или 2 диаметра

    //бросаем частицы в шахматном порядке на линию и запоминаем состояние
    example->dropChain(space);

    sys1 = example->copy();
    moveSystemMRandomly(sys1,85.*(M_PI_2/90.));
    example->savePVPythonAnimation(sys1,"2_180_grades.py",50,50);

    sys2 = sys1->copy();
    sys2->setToGroundState();
    sys1->savePVPythonAnimation(sys2,"2_180_grades_gs.py",50,50);

    sys1 = example->copy();
    moveSystemPosRandomly(sys1,dMax);
    example->savePVPythonAnimation(sys1,"2_180_pos.py",50,50);

    sys2 = sys1->copy();
    sys2->setToGroundState();
    sys1->savePVPythonAnimation(sys2,"2_180_pos_gs.py",50,50);




    cout<<"finish";
    return 0;
}
