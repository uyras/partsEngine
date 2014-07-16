#include <QCoreApplication>

#include "VectTest2D.h"
#include "PartArrayTest2D.h"
#include "PartArrayTest3D.h"
#include "StateMachineTest.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QTest::qExec(new VectTest2D, argc, argv);
    QTest::qExec(new PartArrayTest2D, argc, argv);
    QTest::qExec(new PartArrayTest3D, argc, argv);
    QTest::qExec(new StateMachineTest, argc, argv);
    return a.exec();
}
