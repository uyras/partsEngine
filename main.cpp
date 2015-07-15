#include <QCoreApplication>
#include <ctime>

#include "config.h"
#include "PartArray.h"
#include "squarespinicearray.h"

#include "StateMachine.h"
#include "StateMachineFree.h"
#include "wanglandauparallel.h"

#include "random.h"


using namespace std;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    config::Instance()->srand(time(NULL));
    config::Instance()->m = 1;


    SquareSpinIceArray *sys;

    sys = new SquareSpinIceArray();

    sys->dropSpinIce(3,3);
    cout<<"Parts: "<<sys->count()<<endl;


    qDebug()<<"init Wang Landau Parallel";
    WangLandauParallel w(sys,1000,6,0.75,3);
    qDebug()<<"start Wang Landau DOS";
    w.dos();


    delete sys;

    cout<<"finish"<<endl;
    return 0;
    //return a.exec();
}
