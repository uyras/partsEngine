#ifndef DOS2TEST_H
#define DOS2TEST_H

#include <QObject>
#include <QTest>
#include <QDebug>
#include "dos2.h"

class Dos2Test : public QObject
{
    Q_OBJECT

private slots:
    void regTest(){
        Dos2 dos(-4.,4.,4);

        dos.reg(-5.);
        QCOMPARE(dos.at(0),1.);

        dos.reg(-3.9999999999);
        QCOMPARE(dos.at(0),2.);

        dos.reg(-2.9999999999);
        QCOMPARE(dos.at(0),3.);
        QCOMPARE(dos.at(1),0.);

        dos.reg(-2.00000000001);
        QCOMPARE(dos.at(0),4.);
        QCOMPARE(dos.at(1),0.);

        dos.reg(-2.0);
        QCOMPARE(dos.at(0),4.);
        QCOMPARE(dos.at(1),1.);

        dos.reg(-1.9999999999);
        QCOMPARE(dos.at(0),4.);
        QCOMPARE(dos.at(1),2.);

        dos.reg(-0.9999999999);
        QCOMPARE(dos.at(0),4.);
        QCOMPARE(dos.at(1),3.);

        dos.reg(-0.00000000001);
        QCOMPARE(dos.at(1),4.);
        QCOMPARE(dos.at(2),0.);

        dos.reg(-0.0);
        QCOMPARE(dos.at(1),4.);
        QCOMPARE(dos.at(2),1.);

        dos.reg(0.0);
        QCOMPARE(dos.at(1),4.);
        QCOMPARE(dos.at(2),2.);

        dos.reg(0.00000000001);
        QCOMPARE(dos.at(1),4.);
        QCOMPARE(dos.at(2),3.);

        dos.reg(1.00000000001);
        QCOMPARE(dos.at(1),4.);
        QCOMPARE(dos.at(2),4.);

        dos.reg(1.9999999999);
        QCOMPARE(dos.at(2),5.);
        QCOMPARE(dos.at(3),0.);

        dos.reg(2.0);
        QCOMPARE(dos.at(2),5.);
        QCOMPARE(dos.at(3),1.);

        dos.reg(2.00000000001);
        QCOMPARE(dos.at(2),5.);
        QCOMPARE(dos.at(3),2.);

        dos.reg(3.00000000001);
        QCOMPARE(dos.at(2),5.);
        QCOMPARE(dos.at(3),3.);

        dos.reg(5.0);
        QCOMPARE(dos.at(2),5.);
        QCOMPARE(dos.at(3),4.);

        QCOMPARE(dos.at(0),4.);
        QCOMPARE(dos.at(1),4.);
        QCOMPARE(dos.at(2),5.);
        QCOMPARE(dos.at(3),4.);
    }

    void valTest(){
        Dos2 dos(-4.,4.,4);

        dos.reg(-5.);
        dos.reg(-4.5);
        dos.reg(-4.4);
        dos.reg(-4.3);
        dos.reg(-3.9999999999);
        dos.reg(-2.9999999999);
        dos.reg(-2.00000000001);
        dos.reg(-2.0);
        dos.reg(-1.2);
        dos.reg(-1.1);
        dos.reg(-1.9999999999);
        dos.reg(-0.9999999999);
        dos.reg(-0.00000000001);
        dos.reg(-0.0);
        dos.reg(0.0);
        dos.reg(0.00000000001);
        dos.reg(1.00000000001);
        dos.reg(1.9999999999);
        dos.reg(2.0);
        dos.reg(2.00000000001);
        dos.reg(3.00000000001);
        dos.reg(5.0);

        QCOMPARE(dos[-5.],7.);
        QCOMPARE(dos[-4.5],7.);
        QCOMPARE(dos[-4.4],7.);
        QCOMPARE(dos[-4.3],7.);
        QCOMPARE(dos[-3.9999999999],7.);
        QCOMPARE(dos[-2.9999999999],7.);
        QCOMPARE(dos[-2.00000000001],7.);
        QCOMPARE(dos[-2.0],6.);
        QCOMPARE(dos[-1.2],6.);
        QCOMPARE(dos[-1.1],6.);
        QCOMPARE(dos[-1.9999999999],6.);
        QCOMPARE(dos[-0.9999999999],6.);
        QCOMPARE(dos[-0.00000000001],6.);
        QCOMPARE(dos[-0.0],5.);
        QCOMPARE(dos[0.0],5.);
        QCOMPARE(dos[0.00000000001],5.);
        QCOMPARE(dos[1.00000000001],5.);
        QCOMPARE(dos[1.9999999999],5.);
        QCOMPARE(dos[2.0],4.);
        QCOMPARE(dos[2.00000000001],4.);
        QCOMPARE(dos[3.00000000001],4.);
        QCOMPARE(dos[5.0],4.);
    }

};

#endif // DOS2TEST_H
