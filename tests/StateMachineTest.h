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

    void constructor1(){
        StateMachine s1;
        QVERIFY(s1.size()==0);
        QVERIFY(!s1.connected());
    }

    void constructor2(){
        PartArray sys1(10,10,1,10);

        StateMachine s1(&sys1);

        QCOMPARE((int)s1.size(),10);
        QCOMPARE(s1[5],false);

        QEXPECT_FAIL("", "Will fix in the next release", Continue);
        QCOMPARE(&s1,sys1.state);
    }

    void constructor3(){
        PartArray sys1(10,10,1,10);

        StateMachine *s1 = new StateMachine(sys1);
       QCOMPARE((int)s1->size(),10);
        QCOMPARE((*s1)[5],false);

        QEXPECT_FAIL("", "Will fix in the next release", Continue);
        QCOMPARE(s1,sys1.state);
        delete s1;
    }

    void operatorEqual(){
        StateMachineFree s1(3);
        s1+=5;

        PartArray sys1(10,10,1,3);
        (*sys1.state)=s1;
        QVERIFY((*sys1.state).size()==3);
        QVERIFY(sys1.parts[2]->state);
    }

    void reset(){
        PartArray sys1(10,10,1,3);
        (*sys1.state)+=5;
        sys1.state->reset();
        QCOMPARE(sys1.state->isFirst(),true);
    }

    void setLast(){
        PartArray sys1(10,10,1,2);
        sys1.state->setLast();

        QVERIFY((*sys1.state)[0]);
        QVERIFY((*sys1.state)[1]);
    }

    void randonize(){
        PartArray sys1(100,100,1,100);

        QVERIFY(sys1.state->randomize()<100);
        QVERIFY(!sys1.state->isFirst());

        sys1.state->reset();

        QCOMPARE(sys1.state->randomize(2),2);
        QVERIFY(!sys1.state->isFirst());
    }

    void isFirst(){
        PartArray sys1(100,100,1,100);
        (*sys1.state)+=10;
        QVERIFY(!sys1.state->isFirst());
        sys1.state->reset();
        QVERIFY(sys1.state->isFirst());
    }

    void isLast(){
        PartArray sys1(100,100,1,10);
        QVERIFY(!sys1.state->isLast());
        (*sys1.state)+=100;
        QVERIFY(!sys1.state->isLast());
        (*sys1.state)+=(1024-101);
        QVERIFY(sys1.state->isLast());
    }

    void isHalfLast(){
        PartArray sys1(100,100,1,10);
        QVERIFY(!sys1.state->isHalfLast());
        (*sys1.state)+=100;
        QVERIFY(!sys1.state->isHalfLast());
        (*sys1.state)+=(512-101);
        QVERIFY(sys1.state->isHalfLast());
    }

    void next(){
        PartArray sys1(100,100,1,3);

        QCOMPARE(sys1.state->next(),true);
        QCOMPARE((*sys1.state)[0],true);
        QCOMPARE((*sys1.state)[1],false);
        QCOMPARE((*sys1.state)[2],false);


        QCOMPARE((*sys1.state).next(),true);
        QCOMPARE((*sys1.state)[0],false);
        QCOMPARE((*sys1.state)[1],true);
        QCOMPARE((*sys1.state)[2],false);


        int i=2;
        while ((*sys1.state).next()) i++;

        QCOMPARE(i,7);
        QVERIFY((*sys1.state).isFirst());

        PartArray sys2;
        QVERIFY(!sys2.state->next());
    }

    void halfNext(){
        PartArray sys1(100,100,1,3);

        QCOMPARE(sys1.state->halfNext(),true);
        QCOMPARE((*sys1.state)[0],true);
        QCOMPARE((*sys1.state)[1],false);
        QCOMPARE((*sys1.state)[2],false);


        QCOMPARE((*sys1.state).halfNext(),true);
        QCOMPARE((*sys1.state)[0],false);
        QCOMPARE((*sys1.state)[1],true);
        QCOMPARE((*sys1.state)[2],false);


        int i=2;
        while ((*sys1.state).halfNext()) i++;

        QCOMPARE(i,3);
        QVERIFY((*sys1.state).isFirst());
    }

    void prev(){
        PartArray sys1(100,100,1,3);

        QVERIFY(!sys1.state->prev());
        QCOMPARE((*sys1.state)[0],true);
        QCOMPARE((*sys1.state)[1],true);
        QCOMPARE((*sys1.state)[2],true);

        QVERIFY(sys1.state->prev());
        QCOMPARE((*sys1.state)[0],false);
        QCOMPARE((*sys1.state)[1],true);
        QCOMPARE((*sys1.state)[2],true);

        int i=2;
        while (sys1.state->prev()) i++;
        QCOMPARE(i,8);
        QVERIFY(sys1.state->isLast());

        PartArray sys2;
        QVERIFY(!sys2.state->prev());
    }

    void halfPrev(){
        PartArray sys1(100,100,1,3);

        QVERIFY(!sys1.state->halfPrev());
        QCOMPARE((*sys1.state)[0],true);
        QCOMPARE((*sys1.state)[1],true);
        QCOMPARE((*sys1.state)[2],false);

        QVERIFY((*sys1.state).halfPrev());
        QCOMPARE((*sys1.state)[0],false);
        QCOMPARE((*sys1.state)[1],true);
        QCOMPARE((*sys1.state)[2],false);

        int i=2;
        while ((*sys1.state).halfPrev()) i++;
        QCOMPARE(i,4);
        QVERIFY((*sys1.state).isHalfLast());
    }

    void plusplus(){
        PartArray sys1(10,10,1,3),sys2(10,10,1,3);
        StateMachine s1(sys1),s2(sys2);
        s1.next();s2++;
        s1.next();s2++;
        s1.next();s2++;
        QVERIFY(s1==s2);

        int i=3;
        while (s2++) i++;
        QCOMPARE(i,7);
        QVERIFY(s2.isFirst());
    }

    void minusminus(){
        PartArray sys1(10,10,1,3),sys2(10,10,1,3);
        StateMachine s1(sys1),s2(sys2);
        s1.prev();s2--;
        s1.prev();s2--;
        s1.prev();s2--;
        QVERIFY(s1==s2);

        int i=3;
        while (s2--) i++;
        QCOMPARE(i,8);
        QVERIFY(s2.isLast());
    }

    void toString(){
        PartArray sys1(10,10,1,4);
        StateMachine s1(sys1);
        s1.halfPrev();
        QVERIFY(s1.toString().compare("1110")==0);
        s1.halfPrev();
        s1.halfPrev();
        QVERIFY(s1.toString().compare("1010")==0);
    }

    void fromString(){
        PartArray sys1(10,10,1,10),sys2(10,10,1,10);
        StateMachine s1(sys1),s2(sys2);
        s1+=1022;
        s2.fromString(s1.toString());
        QVERIFY(s2==s1);
    }

    void getelement(){
        PartArray sys1(10,10,1,10);
        StateMachine s1(sys1);
        s1+=1022;
        QVERIFY(s1[7]);
    }

    void size(){
        PartArray sys1(10,10,1,10);
        StateMachine s1;
        QCOMPARE((int)s1.size(),0);
        s1.connect(&sys1);
        QCOMPARE((int)s1.size(),10);
    }

    void equals(){
        PartArray sys1(10,10,1,5),sys2(10,10,1,5);
        StateMachine s1(sys1),s2(sys2);
        s1+=10; s2+=5;
        QVERIFY(!(s1==s2));
        s2+=5;
        QVERIFY(s1==s2);
        PartArray sys3(10,10,1,6);
        StateMachine s3(sys3);
        s3+=10;
        QVERIFY(!(s1==s3));
    }

    void connectdisconnect(){
        PartArray sys1(10,10,1,5);
        StateMachine s1;

        s1.connect(&sys1);
        QVERIFY(s1.connected());
        QCOMPARE((int)s1.size(),5);

        s1.disconnect();
        QVERIFY(!s1.connected());
        QCOMPARE((int)s1.size(),0);

        s1.connect(&sys1);
        QVERIFY(s1.connected());
        QCOMPARE((int)s1.size(),5);

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
        QCOMPARE(a->state->randomize(2),2);
        a->state->reset();
        int reallyTurned = a->state->randomize(1);

        int num=0, turned=-1;
        for (int i=0;i<10;i++){
            if (a->parts.at(i)->state){
                num++;
                turned = i;
            }
        }
        QCOMPARE(num,1);
        QCOMPARE(reallyTurned,turned);
    }
};

#endif // STATEMACHINETEST_H
