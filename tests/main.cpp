#include <QCoreApplication>

#include "VectTest2D.h"
#include "PartArrayTest2D.h"
#include "PartArrayTest3D.h"
#include "StateMachineTest.h"
#include "statemachinefreetest.h"
#include "squarespinicetest.h"
#include "randomtest.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QTest::qExec(new VectTest2D, argc, argv);
    QTest::qExec(new PartArrayTest2D, argc, argv);
    QTest::qExec(new PartArrayTest3D, argc, argv);
    QTest::qExec(new SquareSpinIceTest, argc, argv);
    QTest::qExec(new StateMachineTest, argc, argv);
    QTest::qExec(new StateMachineFreeTest, argc, argv);
    QTest::qExec(new RandomTest, argc, argv);
    return a.exec();
}
