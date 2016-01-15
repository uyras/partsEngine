#ifndef PARTARRAYTEST_H
#define PARTARRAYTEST_H

#include <QObject>
#include <QTest>
#include <QDebug>
#include "PartArray.h"
#include "config.h"
#include "random.h"
#include "ctime"

class PartArrayTest : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase(){
        config::Instance()->set2D();
        Random::Instance();
    }

    void energy(){
        //тест вычисления энергии после создания системы
        PartArray *sys = new PartArray();
        sys->setInteractionRange(0.);
        for (int i=0;i<4;i++){
            for (int j=0;j<4;j++){
                Part temp;
                temp.pos.setXYZ(i,j,0);
                temp.m.setXYZ(0,1,0);
                try{
                    sys->insert(temp);
                } catch(const string c){
                    cout<<c<<endl;
                }
            }
        }

        QCOMPARE(sys->E(), sys->EComplete());

        //после встряхивания состояния
        sys->state.randomize(10);
        QCOMPARE(sys->E(), sys->EComplete());

        //после обнуления конфигурации
        sys->state.reset();
        sys->state.randomize(10);
        QCOMPARE(sys->E(), sys->EComplete());

        //после полного обнуления конфигурации
        sys->state.hardReset();
        QCOMPARE(sys->E(), sys->EComplete());
        delete sys;
    }


    void energy2(){
        int nums[100] = {
            -820, -537, -188, -706, -537,
            -354, -706, -1323, -649, 34,
            -88, -205, -416, 166, -656,
            -873, -278, -66, 753, 165,
            -66, 46, 165, -523, -907,
            -295, 53, -135, -746, -234,
            -585, -873, -649, -416, -88,
            -656, 34, 166, -205, -873,
            -578, 55, -88, -255, 55,
            587, -255, -523, -907, -746,
            53, -585, -295, -234, -135,
            -873, -1637, -1075, -747, -985,
            -1075, -613, -985, -1323, -537,
            -204, 109, -359, -263, -30,
            -417, -985, 34, 767, 609,
            542, 258, 890, 33, -135,
            -66, 196, 979, 441, 137,
            299, 383, -255, -295, 367,
            680, 542, -142, 420, 33,
            -205, -416, -134, 159, -359
        };

        PartArray sys;
        sys.setInteractionRange(0.);
        for (int i=0;i<3;i++)
            for (int j=0; j<3; j++){
                Part temp;
                temp.pos.setXYZ(i,j,0);
                temp.m.setXYZ(0,1,0);
                sys.insert(temp);
            }

        for (int i=0;i<100;i++){
            QCOMPARE(int(floor(sys.E()*100)),nums[i]);
            sys.state.next();
        }
    }

    void energy3(){
        PartArray sys;
        Part p1, p2, p3;
        p1.m = p3.m = Vect(0,1,0);
        p2.m = Vect(0,-1,0);
        p1.pos.setXYZ(0,0,0);
        p2.pos.setXYZ(1,0,0);
        p3.pos.setXYZ(2,0,0);
        sys.insert(p1);
        sys.insert(p2);
        sys.insert(p3);
        QCOMPARE(int(floor(sys.E()*1000)),-1875);
        int eOld = int(floor(sys.E()*1000));
        sys.parts[0]->rotate();
        QVERIFY(int(floor(sys.E()*1000)) != eOld);
    }

    void constructor1(){
        for (int i=0;i<1000000;i++){
            PartArray sys1;
            sys1.insert(Part());
            sys1[0]->pos.setXYZ(1,2,3);
        }
    }

    void constructor2(){
        PartArray sys1;
        sys1.insert(Part());
        sys1[0]->pos.setXYZ(1,2,3);
        PartArray sys2 = PartArray(sys1);
        QCOMPARE(sys1.count(), sys2.count());
        QVERIFY(sys1[0]!=sys2[0]);
        QCOMPARE(sys1[0]->pos.x, sys2[0]->pos.x);
    }

    void testEquals(){
        PartArray sys1;
        sys1.insert(Part());
        sys1[0]->pos.setXYZ(1,2,3);
        sys1[0]->m.setXYZ(1,0,0);
        PartArray sys2 = PartArray(sys1);
        QVERIFY(sys1 == sys2);
        QCOMPARE(sys1[0]->pos.x, sys2[0]->pos.x);

        sys2.state.next();
        QVERIFY(sys1 == sys2);

        sys2.state.prev();
        sys1.E();
        QVERIFY(sys1 == sys2);

        sys2.clear();
        QVERIFY(!(sys1 == sys2));

        sys1.clear();
        QVERIFY(sys1 == sys2);
    }

    void testNonEquals(){
        PartArray sys1;
        sys1.insert(Part());
        sys1[0]->pos.setXYZ(1,2,3);
        sys1[0]->m.setXYZ(1,0,0);
        PartArray sys2 = PartArray(sys1);
        QVERIFY(!(sys1 != sys2));
        QCOMPARE(sys1[0]->pos.x, sys2[0]->pos.x);

        sys2.state.next();
        QVERIFY(!(sys1 != sys2));

        sys1.E();
        QVERIFY(!(sys1 != sys2));

        sys2.clear();
        QVERIFY(sys1 != sys2);

        sys1.clear();
        QVERIFY(!(sys1 != sys2));
    }

    void copy(){
        PartArray sys1;
        for (int i=0;i<3;i++)
            for (int j=0;j<3;j++){
                Part temp;
                temp.pos.setXYZ(i,j,0);
                temp.m.setXYZ(0,1,0);
                sys1.insert(temp);
            }
        sys1.setToGroundState();
        sys1.setToMaximalState();
        sys1.state.next();
        sys1.E();

        PartArray sys2 = PartArray(sys1);
        QVERIFY(sys1==sys2);
        QVERIFY(sys1.state == sys2.state);
        QCOMPARE(sys1.E(),sys2.E());
    }

};

#endif // PARTARRAYTEST_H
