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
        sys1->dropSpinIce(1,1);
        QCOMPARE((int)sys1->cells.size(),1);
        sys1->clear();

        sys1->dropSpinIce(2,2);
        QCOMPARE((int)sys1->cells.size(),4);

        sys1->setToMaximalState();
        for (int i=0;i<4;i++)
            QCOMPARE(sys1->cells[i]->type(),3);

        sys1->cells[0]->top->rotate();
        QCOMPARE(sys1->cells[0]->type(),2);


        sys1->setToGroundState();
        for (int i=0;i<4;i++)
            QCOMPARE(sys1->cells[i]->type(),0);
    }

    void copy(){
        SquareSpinIceArray *sys1,*sys2;
        sys1 = new SquareSpinIceArray();
        sys1->dropSpinIce(3,3);
        sys2 = (SquareSpinIceArray*)sys1->copy();
        QCOMPARE(sys2->count(),sys1->count());
        QCOMPARE(sys1->EComplete(),sys2->EComplete());

        sys1->setToGroundState(); sys2->setToGroundState();
        QCOMPARE(sys1->EComplete(),sys2->EComplete());

        sys1->setToMaximalState(); sys2->setToMaximalState();
        QCOMPARE(sys1->EComplete(),sys2->EComplete());
    }

    void dropSpinIce(){
        SquareSpinIceArray a;
        a.dropSpinIce(2,2,3);

        QCOMPARE(a.parts[0]->pos.x,1.5);
        QCOMPARE(a.parts[0]->pos.y,3.);

        QCOMPARE(a.parts[1]->pos.x,1.5);
        QCOMPARE(a.parts[1]->pos.y,0.);

        QCOMPARE(a.parts[2]->pos.x,0.);
        QCOMPARE(a.parts[2]->pos.y,1.5);

        QCOMPARE(a.parts[3]->pos.x,3.);
        QCOMPARE(a.parts[3]->pos.y,1.5);

        QCOMPARE(a.count(),12);
    }
};


#endif // SQUARESPINICETEST

