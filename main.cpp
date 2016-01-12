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

    PartArray *backupSys = SysLoader::load("1.dat");

    backupSys->state.next();

    backupSys->save("4.dat");

    delete backupSys;

    cout<<"finish"<<endl;
    return 0;
    //return a.exec();
}
