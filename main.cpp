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

    Dos dos(0,5,5);
    dos.reg(0,1);
    dos.reg(0,2);
    dos.reg(5);
    dos.reg(4.99999);

    cout<<dos.toString();


    cout<<"finish"<<endl;
    return 0;
    //return a.exec();
}
