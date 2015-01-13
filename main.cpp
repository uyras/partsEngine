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

int main(){

    config::Instance()->set2D();
    config::Instance()->srand(time(NULL));
    config::Instance()->ergGauss = 3e7 * 9274e-24; //3*10^-7 - намагниченность одной частицы (магн. Бора), 9274e-24 - эрг/Гс в обном Боре

    PartArray sys1 (1000,1000,1), sys2;
    dropSpinIce(80,220,320,&sys1);

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


    cout<<"finish"<<endl;
    return 0;
}
