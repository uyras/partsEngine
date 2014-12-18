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
        switch (config::Instance()->dimensions()==2) {
        case 2:
            lattitude=0; // если частица 2-х мерная то угол отклонения должен быть 0
            break;
        case 3:
            lattitude=(double)config::Instance()->rand()/(double)config::Instance()->rand_max*2.-1.;
            break;
        }
        dir.x = d * cos(longitude) * sqrt(1-lattitude*lattitude);
        dir.y = d * sin(longitude) * sqrt(1-lattitude*lattitude);
        dir.z = 0 * lattitude;
        sys->parts[i]->pos += dir;
    }
}

void moveSystemMRandomly(PartArray* sys, double fi){
    if (config::Instance()->dimensions()==2){
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

void dropSpinIce(double partW, double partH, double lattice, PartArray *pa){
    if (config::Instance()->dimensions()!=2)
        return;

    if (partW+partH>lattice) //если параметр решетки слишком маленький, увеличиваем до нужного
        lattice=partH+partW;

    double a = fmin(partW, partH); //которкая сторона овала
    double b = fmax(partW, partH); //длиная сторона овала

    double firstSpace = lattice/2.+a/2.;
    Vect center = Vect(firstSpace,firstSpace,0);

    Part *temp;

    while (center.y < pa->size.y) {
        while(center.x < pa->size.x){
            temp = new Part();
            //добавляем горизонтальные
            temp->pos = Vect(center.x, center.y-lattice/2.,0);
            temp->m = Vect(config::Instance()->m,0,0);
            pa->insert(temp);

            //добавляем вертикальные
            temp = new Part();
            temp->pos = Vect(center.x-lattice/2.,center.y,0);
            temp->m = Vect(0,config::Instance()->m,0);
            pa->insert(temp);
            center.x+=lattice;
        }
        //обрабатываем крайние частицы (возможно запихнуть еще один ряд вертикальных)
        if (center.x - lattice/2. + a/2. <= pa->size.x){
            temp = new Part();
            temp->pos = Vect(center.x-lattice/2.,center.y,0);
            temp->m = Vect(0,config::Instance()->m,0);
            pa->insert(temp);
        }
        center.y+=lattice;
    }
    //обрабатываем крайние частицы (возможно запихнуть еще один ряд горизонтальных)
    if (center.y - lattice/2. + a/2. <= pa->size.y){
        temp = new Part();
        temp->pos = Vect(center.x-lattice/2.,center.y,0);
        temp->m = Vect(0,config::Instance()->m,0);
        pa->insert(temp);
    }

}

int main(){

    config::Instance()->set2D();
    config::Instance()->srand(time(NULL));
    config::Instance()->ergGauss = 3e7 * 9274e-24; //3*10^-7 - намагниченность одной частицы (магн. Бора), 9274e-24 - эрг/Гс в обном Боре

    PartArray sys1 (1000,1000,1);

    ofstream f("spinIceField.dat");
    cout<<config::Instance()->ergGauss/pow(config::Instance()->santiMeter,3.)<<endl;
    for (int i=320;i<881;i++){
        sys1.clear();
        dropSpinIce(80,220,i,&sys1);
        Vect field = sys1.parts[0]->interact(sys1.parts[1]);
        //cout<<i<<": ";field.draw();
        field*=config::Instance()->ergGauss/pow(config::Instance()->santiMeter,3.);
        cout<<i<<": ";field.draw();
        f<<i<<"\t"<<field.length()<<endl;
    }

    cout<<"finish";
    return 0;
}
