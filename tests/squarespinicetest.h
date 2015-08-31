#ifndef SQUARESPINICETEST
#define SQUARESPINICETEST

#include <QObject>
#include <QTest>
#include <QDebug>
#include "squarespinicearray.h"
#include "config.h"
#include "ctime"

class SquareSpinIceTest : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase(){
        config::Instance()->set2D();
        config::Instance()->srand(time(NULL));
        config::Instance()->partR = 0.5;
        config::Instance()->m = 1;
    }

    void cellTypes(){
        SquareSpinIceArray *sys1;
        sys1 = new SquareSpinIceArray();
        sys1->dropSpinIce(2,2);
        QCOMPARE((int)sys1->cells.size(),4);
        sys1->clear();
        sys1->dropSpinIce(1,1);
        QCOMPARE((int)sys1->cells.size(),1);

        sys1->setToMaximalState();
        QCOMPARE(sys1->cells[0]->type(),3);

        sys1->state->next();
        QCOMPARE(sys1->cells[0]->type(),2);

        sys1->setToGroundState();
        QCOMPARE(sys1->cells[0]->type(),0);
    }

    void copy(){
        SquareSpinIceArray *sys1,*sys2;
        sys1 = new SquareSpinIceArray();
        sys1->dropSpinIce(3,3);
        sys2 = (SquareSpinIceArray*)sys1->copy();
        QCOMPARE(sys2->count(),sys1->count());
        QCOMPARE(sys1->calcEnergy1(),sys2->calcEnergy1());

        sys1->setToGroundState(); sys2->setToGroundState();
        QCOMPARE(sys1->calcEnergy1(),sys2->calcEnergy1());

        sys1->setToMaximalState(); sys2->setToMaximalState();
        QCOMPARE(sys1->calcEnergy1(),sys2->calcEnergy1());
    }
};


#endif // SQUARESPINICETEST

