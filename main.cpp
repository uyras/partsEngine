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

    config::Instance()->set3D();

    config::Instance()->m = 1;


    PartArray *sys = new PartArray();


    sys->dropTetrahedron(1,1,1);
    sys->savePVPython("sys.py");



    delete sys;

    cout<<"finish"<<endl;
    return 0;
    //return a.exec();
}
