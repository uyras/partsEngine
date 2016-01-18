#include <QCoreApplication>
#include <ctime>
#include <sstream>

#include "config.h"
#include "StateMachine.h"
#include "StateMachineFree.h"
#include "PartArray.h"
#include "squarespinicearray.h"
#include "honeycombspinicearray.h"

#include "squarespinicearray.h"
#include "squarelattice.h"

#include "random.h"
#include <cmath>
#include <QElapsedTimer>


using namespace std;


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    config::Instance()->set2D();




    PartArray *sys = new PartArray();

    sys->setInteractionRange(1.1);
    for (int i=0;i<4;i++){
        for (int j=0;j<4;j++){
            Part temp;
            temp.pos.setXYZ(i,j,0);
            temp.m.setXYZ(0,1,0);
            sys->insert(temp);
        }
    }

    for (int i=0;i<10;i++){
        cout<<sys->EComplete()<<", ";
        sys->state.next();
    }

    delete sys;

    cout<<"finish"<<endl;
    return 0;
    //return a.exec();
}
