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

    PartArray sys1(10,10,1,3),sys2(10,10,1,3),sys3(10,10,1,4);
    StateMachine s1(&sys2),s2(&sys2);
    s1 = StateMachine(&sys2);

    s2 = StateMachine(&sys3);
    !(*sys2.state==*sys3.state);
    !(*sys2.state==s2);
    !(s1==s2);

    /*SquareSpinIceArray *sys;

    sys = new SquareSpinIceArray();
    sys->dropSpinIce(2,2);

    //sys->dropHoneyComb(2,2,1);
    cout<<"Parts: "<<sys->count()<<endl;
    sys->setToMaximalState();
    sys->save("max");



    //qDebug()<<"init Wang Landau Parallel";
    //WangLandauParallel w(sys,1000,4,0.8,4);
    //qDebug()<<"start Wang Landau DOS";
    //w.dos();


    delete sys;*/

    cout<<"finish"<<endl;
    return 0;
    //return a.exec();
}
