#ifndef PARTARRAYTEST2D_H
#define PARTARRAYTEST2D_H

#include <QObject>
#include <QTest>
#include <QDebug>
#include "PartArray.h"
#include "config.h"
#include "random.h"
#include "ctime"

class PartArrayTest2D : public QObject
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
        for (int i=0;i<4;i++){
            for (int j=0;j<4;j++){
            Part temp;
            temp.pos.setXYZ(i,j,0);
            temp.m.setXYZ(0,1,0);
            sys->insert(temp);
            }
        }

        QCOMPARE(sys->E(), sys->EComplete());

        //после встряхивания состояния
        sys->state->randomize(10);
        QCOMPARE(sys->E(), sys->EComplete());

        //после обнуления конфигурации
        sys->state->reset();
        sys->state->randomize(10);
        QCOMPARE(sys->E(), sys->EComplete());

        //после полного обнуления конфигурации
        sys->state->hardReset();
        QCOMPARE(sys->E(), sys->EComplete());

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
        for (int i=0;i<3;i++)
            for (int j=0; j<3; j++){
                Part temp;
                temp.pos.setXYZ(i,j,0);
                temp.m.setXYZ(0,1,0);
                sys.insert(temp);
            }

        for (int i=0;i<100;i++){
            QCOMPARE(int(floor(sys.E()*100)),nums[i]);
            sys.state->next();
        }
    }

};

#endif // PARTARRAYTEST2D_H
