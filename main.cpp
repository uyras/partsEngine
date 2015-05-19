#include <QCoreApplication>
#include <boost/mpi.hpp>
#include "config.h"
#include "partarrayboost.h"
#include "PartArray.h"
#include "PartArrayMPI.h"
#include <boost/multiprecision/cpp_int.hpp>

namespace mpi=boost::mpi;
using namespace std;
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    mpi::environment env;
    mpi::communicator world;

    config::Instance()->srand(time(NULL));

    config::Instance()->m = 1;

    PartArray sys;
    sys.dropHoneyComb(3,1,1);
    sys.state->next();
    sys.state->hardReset();
    PartArrayMPI sys2;
    sys2.dropHoneyComb(3,1,1);
    sys2.state->next();
    sys2.state->hardReset();
    double eMin,eMax;
    sys2.getMinMaxEnergy(eMin,eMax);
    sys.setToGroundState();

    if (world.rank()==0){
        cout<<sys.calcEnergy1()<<endl;
        cout<<eMin<<endl;
    }

    cout<<"finish"<<endl;
    return a.exec();
}
