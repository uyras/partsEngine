#include <QCoreApplication>
#include <boost/mpi.hpp>
#include "config.h"
#include "partarrayboost.h"
#include "PartArray.h"
#include "PartArrayMPI.h"
#include <boost/multiprecision/cpp_int.hpp>

#include "StateMachine.h"
#include "StateMachineFree.h"

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
    sys.state->next();
    StateMachineFree s;
    s = sys.state;
    sys.state->next();
    cout<<s.toString()<<endl;
    s.prev();
    cout<<s.toString()<<endl;
    s.next();
    s.fromString(s.toString());
    cout<<s.toString()<<endl;
    cout<<"finish"<<endl;
    return 0;
    //return a.exec();
}
