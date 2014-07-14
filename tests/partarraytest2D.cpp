#include "partarraytest2D.h"

void PartArrayTest2D::initTestCase()
{
    config::Instance()->set2D();
    config::Instance()->srand(time(NULL));
}

void PartArrayTest2D::dropRandom()
{
    PartArray *a = new PartArray(10,10,10);
    a->dropRandom(10);
    QCOMPARE(a->count(),10);
    a->dropRandom(1);
    QCOMPARE(a->count(),11);
}
