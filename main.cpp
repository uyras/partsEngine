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
#include "dos.h"

#include "random.h"
#include <cmath>
#include <QElapsedTimer>


using namespace std;


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    config::Instance()->set2D();

    SquareSpinIceArray sys;
    sys.dropSpinIce(40,40);
    sys.setMinstate(sys.groundState());
    sys.setMaxstate(sys.maximalState());
    sys.save("SSI_40x40.mfsys");


    cout<<"finish"<<endl;
    return 0;
    //return a.exec();
}
