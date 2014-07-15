#include <QCoreApplication>

#include "VectTest2D.h"
#include "PartArrayTest2D.h"
#include "PartArrayTest3D.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QTest::qExec(new VectTest2D, argc, argv);
    QTest::qExec(new PartArrayTest2D, argc, argv);
    QTest::qExec(new PartArrayTest3D, argc, argv);
    return a.exec();
}
