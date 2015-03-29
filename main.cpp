#include <QCoreApplication>
#include <QSettings>
#include <QString>
#include <iostream>
#include <sstream>
#include "config.h"
#include "partarrayboost.h"
#include "PartArray.h"
#include <boost/mpi/environment.hpp>
#include <boost/mpi/communicator.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

//namespace mpi=boost::mpi;
using namespace std;
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    config::Instance()->srand(time(NULL));

    PartArray aaa(10.,10.,10.);
    aaa.dropRandom(2);
    aaa.calcEnergy1FastIncrementalFirst();
    aaa.state->randomize(1);
    aaa.cout();
    aaa.state->draw();
    cout<<endl;

    std::ofstream ofs("data.text");
    std::ifstream ifs("data.text");
    boost::archive::text_oarchive oa(ofs);
    oa<<(aaa);
    ofs.close();
    cout<<"ready"<<endl;

    boost::archive::text_iarchive ia(ifs);
    PartArray v2;
    ia>>v2;
    ifs.close();
    aaa.state->randomize();
    v2.cout();
    v2.state->draw();
    cout<<endl;
    aaa.cout();
    aaa.state->draw();

    return a.exec();
}
