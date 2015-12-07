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
};

#endif // PARTARRAYTEST3D_H
