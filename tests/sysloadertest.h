#ifndef SYSLOADERTEST
#define SYSLOADERTEST

#include <QObject>
#include <QTest>
#include <QDebug>

#include "PartArray.h"
#include "squarespinicearray.h"
#include "honeycombspinicearray.h"
#include "sysloader.h"

class SysLoaderTest: public QObject
{
    Q_OBJECT

private slots:
    void loadStandart(){
        PartArray sys;
        Part *temp, *temp2;
        for (int i=0;i<10;i++){
            temp= new Part();
            temp->m = Vect(i+1,i*2+1,i*3+1);
            temp->pos = Vect(i, i/2., i/3.);
            if (i%2)
                temp->rotate();
            sys.insert(temp);
        }
        sys.state.randomize(5);
        sys.save("sys.sys");

        PartArray *sys2 = SysLoader::load("sys.sys");
        QCOMPARE(sys2->type(),QString("standart"));
        QCOMPARE(sys2->count(),sys.count());
        for (int i=0;i<10;i++){
            temp2 = sys2->parts[i];
            temp = sys.getById(temp2->Id());
            QCOMPARE(temp->m, temp2->m);
            QCOMPARE(temp->pos, temp2->pos);
            QCOMPARE(temp->Id(), temp2->Id());
        }

        QCOMPARE(QString(sys.state.toString().c_str()), QString(sys2->state.toString().c_str()));
        delete sys2;
    }

    void loadSquare(){
        SquareSpinIceArray sys;
        Part *temp, *temp2;
        sys.dropSpinIce(3,3);
        sys.state.randomize(5);
        sys.save("sys.sys");

        PartArray *sys2 = SysLoader::load("sys.sys");
        QCOMPARE(sys2->type(),QString("squarespinice"));
        QCOMPARE(sys2->count(),sys.count());
        for (int i=0;i<10;i++){
            temp2 = sys2->parts[i];
            temp = sys.getById(temp2->Id());
            QCOMPARE(temp->m, temp2->m);
            QCOMPARE(temp->pos, temp2->pos);
            QCOMPARE(temp->Id(), temp2->Id());
        }

        QCOMPARE(QString(sys.state.toString().c_str()), QString(sys2->state.toString().c_str()));
        delete sys2;
    }

    void loadHoneycomb(){
        HoneycombSpinIceArray sys;
        Part *temp, *temp2;
        sys.dropHoneyComb(3,3,1);
        sys.state.randomize(5);
        sys.save("sys.sys");

        PartArray *sys2 = SysLoader::load("sys.sys");
        QCOMPARE(sys2->type(),QString("honeycomb"));
        QCOMPARE(sys2->count(),sys.count());
        for (int i=0;i<10;i++){
            temp2 = sys2->parts[i];
            temp = sys.getById(temp2->Id());
            QCOMPARE(round(temp->m.x*1e5), round(temp2->m.x*1e5));
            QCOMPARE(round(temp->m.y*1e5), round(temp2->m.y*1e5));
            QCOMPARE(round(temp->m.z*1e5), round(temp2->m.z*1e5));
            QCOMPARE(round(temp->pos.x*1e5), round(temp2->pos.x*1e5));
            QCOMPARE(round(temp->pos.y*1e5), round(temp2->pos.y*1e5));
            QCOMPARE(round(temp->pos.z*1e5), round(temp2->pos.z*1e5));
            QCOMPARE(temp->Id(), temp2->Id());
        }

        QCOMPARE(QString(sys.state.toString().c_str()), QString(sys2->state.toString().c_str()));
        delete sys2;
    }
};

#endif // SYSLOADERTEST

