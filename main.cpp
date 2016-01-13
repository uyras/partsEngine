#include <QCoreApplication>
#include <ctime>

#include "config.h"
#include "StateMachine.h"
#include "StateMachineFree.h"
#include "PartArray.h"
#include "squarespinicearray.h"
#include "honeycombspinicearray.h"

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

    HoneycombSpinIceArray sys;
    sys.dropHoneyComb(3,3,1);
    sys.setToGroundState();
    sys.setToMaximalState();
    sys.state.randomize(10);
    sys.save("10.mfsys");
    sys.load("10.mfsys");
    sys.save("11.mfsys");

    PartArray sys2;
    sys2.load("11.mfsys");
    sys2.save("12.mfsys");
    sys2.state.randomize(10);
    sys2.save("13.mfsys");

    cout<<"finish"<<endl;
    return 0;
    //return a.exec();
}
