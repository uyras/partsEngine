#ifndef PARTARRAYTEST2D_H
#define PARTARRAYTEST2D_H

#include <QObject>
#include <QTest>
#include <QDebug>
#include "PartArray.h"
#include "config.h"
#include "ctime"

class PartArrayTest2D : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase(){
        config::Instance()->set2D();
        config::Instance()->srand(time(NULL));
        config::Instance()->partR = 0.5;
        config::Instance()->m = 1;
    }
    void dropRandomI()
    {
        PartArray *a = new PartArray(10,10,1);
        a->dropRandom(10);
        QCOMPARE(a->count(),10);
        a->dropRandom(1);
        QCOMPARE(a->count(),11);
        delete a;
    }
    void dropRandomD(){
        PartArray *a = new PartArray(10,10,1);
        a->dropRandom(0.3);
        QVERIFY(a->destiny(true)>0.25&&a->destiny(true)<0.35);
        a->dropRandom(0.4);
        QVERIFY(a->destiny(true)>0.35&&a->destiny(true)<0.45);
        delete a;
    }
};

#endif // PARTARRAYTEST2D_H
