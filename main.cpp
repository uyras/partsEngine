#include <QCoreApplication>
#include <ctime>

#include "config.h"
#include "PartArray.h"
#include "squarespinicearray.h"
#include "honeycombspinicearray.h"

#include "StateMachine.h"
#include "StateMachineFree.h"
#include "squarespinicearray.h"
#include "squarelattice.h"
#include "sysloader.h"

#include "random.h"
#include <cmath>
#include <QElapsedTimer>


using namespace std;


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    config::Instance()->set2D();

    SquareSpinIceArray sys;
    //sys.setInteractionRange(0.9);
    sys.dropSpinIce(2,2);
    sys.save("honeycomb.sys");

    SquareSpinIceArray sys2,sys3,sys4;
    sys2.load("honeycomb.sys");
    sys3.load("honeycomb.sys");
    sys4.load("honeycomb.sys");
    std::vector < Part* >::const_iterator iter;

    double e1I, e2I, e3I, e1,e2,e3,e4,e5;

    QElapsedTimer timer;

    timer.start();
    e1I = sys.calcEnergy1FastIncrementalFirst();
    do {
        sys.calcEnergy1FastIncremental(e1I);
    } while (sys.state->halfNext());
    cout << timer.nsecsElapsed() << "\tnanoseconds"<<endl;

    timer.start();
    e2I = sys2.calcEnergy1FastIncrementalFirst2();
    do {
        sys2.calcEnergy1FastIncremental2(e2I);
    } while (sys2.state->halfNext());
    cout << timer.nsecsElapsed() << "\tnanoseconds"<<endl;

    timer.start();
    e3I = sys3.calcEnergy1FastIncrementalFirst3();
    do {
        sys3.calcEnergy1FastIncremental3(e3I);
    } while (sys3.state->halfNext());
    cout << timer.nsecsElapsed() << "\tnanoseconds"<<endl;

    timer.start();
    do{
        iter = sys4.parts.begin();
        while (iter != sys4.parts.end()) {
            sys4.EComplete(*iter);
            iter++;
        }
        sys4.state->next();
    } while (sys4.state->halfNext());
    cout << timer.nsecsElapsed() << "\tnanoseconds"<<endl;

    timer.start();
    do {
        iter = sys4.parts.begin();
        while (iter != sys4.parts.end()) {
            sys4.ECompleteOld(*iter);
            iter++;
        }
        sys3.state->next();
    } while (sys3.state->halfNext());
    cout << timer.nsecsElapsed() << "\tnanoseconds"<<endl;


    e1I = sys.calcEnergy1FastIncrementalFirst();
    e2I = sys2.calcEnergy1FastIncrementalFirst2();
    e3I = sys3.calcEnergy1FastIncrementalFirst3();

    for (int i=0;i<10;i++){
        e3 = sys3.calcEnergy1FastIncremental3(e3I);
        e2 = sys2.calcEnergy1FastIncremental2(e2I);
        e1 = sys.calcEnergy1FastIncremental(e1I);

        e4=e5=0;
        iter = sys4.parts.begin();
        while (iter != sys4.parts.end()) {
            e4 += sys4.EComplete(*iter);
            e5 += sys4.ECompleteOld(*iter);
            iter++;
        }
        e4*=0.5; e5*=0.5;

        cout<<e1<<"\t"<<e2<<"\t"<<e3<<"\t"<<e4<<"\t"<<e5<<endl;

        sys.state->next();
        sys2.state->next();
        sys3.state->next();
        sys4.state->next();
    }

    /*
    std::vector < Part* >::const_iterator iter;
    double E1, E2;

    QElapsedTimer timer;

    timer.start();
    for (int i=0;i<10000;i++){
        E1=0;
        iter = sys.parts.begin();
        while (iter != sys.parts.end()) {
            E1 += sys.EComplete(*iter);
            iter++;
        }
        E1 *= 0.5;
    }
    qDebug() << timer.nsecsElapsed() << "nanoseconds";

    timer.start();
    for (int i=0;i<10000;i++){
        E2=0;
        iter = sys.parts.begin();
        while (iter != sys.parts.end()) {
            E2 += sys.ECompleteOld(*iter);
            iter++;
        }
        E2 *= 0.5;
    }
    qDebug() << timer.nsecsElapsed() << "nanoseconds";

    cout<<E1<<"\t"<<E2<<endl;*/

    /*SquareSpinIceArray *sys = new SquareSpinIceArray();
    sys->dropSpinIce(10,5,1);
    sys->E();

    PartArray *sys2 = SysLoader::load("out.dat");
    sys2->state->next();
    sys2->save("out2.dat");

    cout<<sys2->type().toStdString();

    PartArray *sys3 = SysLoader::load("out2.dat");
    sys3->state->next();
    sys3->save("out3.dat");

    cout<<"finish"<<endl;*/
    return 0;
    //return a.exec();
}
