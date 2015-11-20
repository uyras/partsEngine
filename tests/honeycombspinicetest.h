#ifndef HONEYCOMBSPINICETEST_H
#define HONEYCOMBSPINICETEST_H

#include <QObject>
#include <QTest>
#include <QDebug>
#include "honeycombspinicearray.h"
#include "config.h"
#include "ctime"

class HoneycombSpinIceTest : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase(){
        config::Instance()->set2D();
        config::Instance()->srand(time(NULL));
        config::Instance()->partR = 0.5;
        config::Instance()->m = 1;
    }

    void dropHoneyCombTest(){
        HoneycombSpinIceArray *sys1 = new HoneycombSpinIceArray();
        sys1->dropHoneyComb(2,2,1);
        QCOMPARE(sys1->count(),19);
        QCOMPARE((int)sys1->cells.size(),4);

        sys1->dropHoneyComb(2,2,2);
        QCOMPARE(sys1->count(),19);
        QCOMPARE((int)sys1->cells.size(),4);
    }

    void copy(){
        HoneycombSpinIceArray *sys1,*sys2;
        sys1 = new HoneycombSpinIceArray();
        sys1->dropHoneyComb(2,2,1);
        sys2 = (HoneycombSpinIceArray*)sys1->copy();
        QCOMPARE(sys2->count(),sys1->count());
        QCOMPARE(sys2->cells.size(),sys1->cells.size());
        QCOMPARE(sys1->EComplete(),sys2->EComplete());

        sys1->setToGroundState(); sys2->setToGroundState();
        QCOMPARE(sys1->EComplete(),sys2->EComplete());

        sys1->setToMaximalState(); sys2->setToMaximalState();
        QCOMPARE(sys1->EComplete(),sys2->EComplete());
    }
};

#endif // HONEYCOMBSPINICETEST_H

