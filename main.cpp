#include <QCoreApplication>
#include <boost/mpi.hpp>
#include "config.h"
#include "partarrayboost.h"
#include "PartArray.h"
#include "PartArrayMPI.h"

namespace mpi=boost::mpi;
using namespace std;
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    mpi::environment env;
    mpi::communicator world;
    config::Instance()->srand(time(NULL)+world.rank());


    if (world.rank()==0){
        PartArrayMPI aaa(10.,10.,10.);
        aaa.dropRandom(3);
        aaa.calcEnergy1FastIncrementalFirst();
        aaa.state->randomize(1);
        aaa.cout();
        aaa.state->draw();
        cout<<endl;
        world.send(1,1,aaa);
    }
    if (world.rank()==1){
        PartArrayMPI bbb(10.,10.,10.);
        world.recv(0,1,bbb);
        //cout<<parts.size();

        //bbb.insert(&p);
        bbb.cout();
        bbb.state->draw();
        cout<<endl;
    }

    return a.exec();
}
