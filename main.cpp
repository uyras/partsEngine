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
    sys.setInteractionRange(0.8);
    sys.dropSpinIce(2,2);
    sys.state.randomize(100);
    vector< vector<Part*> > clusters = sys.clusters();

    int i=1;
    for (vector<Part*> cluster : clusters){
        ofstream f(QString("cluster%1.dat").arg(i).toStdString().c_str());
        for (Part* temp : cluster){
            f<<temp->pos.x-temp->m.x*0.5<<"\t"<<temp->pos.y-temp->m.y*0.5<<"\t";
            f<<temp->m.x<<"\t"<<temp->m.y<<endl;
        }
        f.close();
        i++;
    }


    cout<<"finish"<<endl;
    return 0;
    //return a.exec();
}
