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
    sys.dropSpinIce(2,2,3);
    sys.save("square.dat");
/*
    do {
        double e1 = sys.E();
        double e2 = sys.EComplete();
        if (floor(e1*10)!=floor(e2*10))
            cout<<"err "<<e1<<"\t"<<e2<<"\t";
        cout<<"err "<<floor(e1*10)<<"\t"<<floor(e2*10)<<endl;
    } while (sys.state->next());
*/

    cout<<"finish"<<endl;
    return 0;
    //return a.exec();
}
