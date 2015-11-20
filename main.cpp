#include <QCoreApplication>
#include <ctime>

#include "config.h"
#include "PartArray.h"
#include "squarespinicearray.h"
#include "honeycombspinicearray.h"

#include "StateMachine.h"
#include "StateMachineFree.h"
#include "squarespinicearray.h"
#include "sysloader.h"

#include "random.h"
#include <cmath>


using namespace std;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    config::Instance()->set2D();

    config::Instance()->m = 1;

    HoneycombSpinIceArray sys;
    sys.dropHoneyComb(3,3,1);
    sys.save("norm.dat");
    sys.setToGroundState();
    sys.save("gs.dat");
    sys.setToMaximalState();
    sys.save("max.dat");

    PartArray* sys2 = SysLoader::load("max.dat");
    sys2->setToGroundState();
    int i=0;
    sys2->save("new.dat");


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
