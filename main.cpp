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

    config::Instance()->srand(time(NULL));

    config::Instance()->m = 1;

    PartArray sys;
    sys.dropHoneyComb(2,3,10);
    cout<<sys.count()<<endl;
    sys.setToGroundState();
    sys.save("sys.dat");
    cout<<"finish"<<endl;
    return a.exec();
}
