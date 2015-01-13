#ifndef STATEMACHINETEST_H
#define STATEMACHINETEST_H

#include <QObject>
#include <QTest>
#include <QDebug>
#include "PartArray.h"
#include "StateMachine.h"
#include "config.h"
#include "ctime"

class StateMachineTest : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase(){
        config::Instance()->srand(time(NULL));
        config::Instance()->set2D();
    }

    void testEquals(){
        PartArray sys1(10,10,1,3),sys2(10,10,1,3),sys3(10,10,1,4);
        StateMachine s1(&sys2),s2(&sys2);
        QVERIFY(*sys1.state==*sys2.state);
        s1 = StateMachine(&sys2);
        QVERIFY(*sys1.state==s1);

        sys1.parts[0]->rotate();
        QVERIFY(!(*sys1.state==*sys2.state));
        QVERIFY(!(*sys1.state==s1));

        s2 = StateMachine(&sys3);
        QVERIFY(!(*sys2.state==*sys3.state));
        QVERIFY(!(*sys2.state==s2));
        QVERIFY(!(s1==s2));

        sys2.parts[1]->rotate();
        sys2.parts[2]->rotate();
        QVERIFY(*sys1.state==*sys2.state);
        s2 = StateMachine(&sys2);
        QVERIFY(*sys1.state==s2);
        s1 = StateMachine(&sys1);
        QVERIFY(s1==s2);

    }

    void emptyTest(){
        PartArray sys1(10,10,1),sys2(10,10,1);
        StateMachine s1(&sys2),s2(&sys2);
        QVERIFY(*sys1.state==*sys2.state);
        QVERIFY(*sys1.state==s2);
        QVERIFY(s1==*sys2.state);
        QVERIFY(s1==s2);

        sys1.dropRandom(1);
        s1 = StateMachine(&sys1);
        QVERIFY(!(*sys1.state==*sys2.state));
        QVERIFY(!(*sys1.state==s2));
        QVERIFY(!(s1==*sys2.state));
        QVERIFY(!(s1==s2));


        sys2.dropRandom(1);
        s2 = StateMachine(&sys2);
        QVERIFY(*sys1.state==*sys2.state);
        QVERIFY(*sys1.state==s2);
        QVERIFY(s1==*sys2.state);
        QVERIFY(s1==s2);


        sys2.dropRandom(2);
        s2 = StateMachine(&sys2);
        QVERIFY(!(*sys1.state==*sys2.state));
        QVERIFY(!(*sys1.state==s2));
        QVERIFY(!(s1==*sys2.state));
        QVERIFY(!(s1==s2));
    }


    void randomize(){
        config::Instance()->set3D();
        PartArray *a = new PartArray(10,10,10);
        a->dropRandom(10);
        QVERIFY(a->state->randomize()>=0);
        a->state->reset();
        QCOMPARE(a->state->randomize(11),-1);
        a->state->reset();
        QCOMPARE(a->state->randomize(5),5);

        int num=0;
        for (int i=0;i<10;i++){
            if (a->parts.at(i)->state)
                num++;
        }
        QCOMPARE(num,5);
    }
};

#endif // STATEMACHINETEST_H
