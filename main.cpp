#include <QCoreApplication>
#include <ctime>

#include "config.h"
#include "PartArray.h"
#include "squarespinicearray.h"
#include "honeycombspinicearray.h"

#include "StateMachine.h"
#include "StateMachineFree.h"
#include "wanglandauparallel.h"

#include "random.h"


using namespace std;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Random::Instance(4);
    Random::Instance(5)->next(10);
    Random::Instance(6)->next(15);
    Random::Instance(7)->next(200);
            /*
    config::Instance()->m = 1;


    HoneycombSpinIceArray *sys;

    sys = new HoneycombSpinIceArray();
    sys->dropHoneyComb(2,2,1);
    sys->setToGroundState();
    cout<<sys->calcEnergy1()<<endl;
    sys->save("min1.dat");
    sys->PartArray::setToGroundState();
    cout<<sys->calcEnergy1()<<endl;
    sys->save("min2.dat");

    /*
    qDebug()<<"init Wang Landau Parallel";
    WangLandauParallel w(sys,1000,4,0.8,4);
    qDebug()<<"start Wang Landau DOS";
    w.dos();



    delete sys;

    cout<<"finish"<<endl;*/
    return 0;
    //return a.exec();
}
