#include <QCoreApplication>
#include "partarraytest2D.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QTest::qExec(new PartArrayTest2D, argc, argv);
    return a.exec();
}
