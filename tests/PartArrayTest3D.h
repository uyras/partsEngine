#ifndef PARTARRAYTEST3D_H
#define PARTARRAYTEST3D_H

#include <QObject>
#include <QTest>
#include <QDebug>
#include "PartArray.h"
#include "config.h"
#include "ctime"

class PartArrayTest3D : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase(){
        config::Instance()->set3D();
        config::Instance()->srand(time(NULL));
        config::Instance()->partR = 0.5;
        config::Instance()->m = 1;
    }
    void dropRandomI()
    {
        PartArray *a = new PartArray(10,10,10);
        a->dropRandom(10);
        QCOMPARE(a->count(),10);
        a->dropRandom(1);
        QCOMPARE(a->count(),11);
        delete a;
    }
    void dropRandomD(){
        PartArray *a = new PartArray(10,10,10);
        a->dropRandom(0.1);
        QVERIFY(a->destiny(true)>0.09&&a->destiny(true)<0.11);
        a->dropRandom(0.2);
        QVERIFY(a->destiny(true)>0.19&&a->destiny(true)<0.21);
        delete a;
    }
};

#endif // PARTARRAYTEST3D_H
