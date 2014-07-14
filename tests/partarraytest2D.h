#ifndef PARTARRAYTEST2D_H
#define PARTARRAYTEST2D_H

#include <QObject>
#include <QTest>
#include <QDebug>
#include "PartArray.h"
#include "config.h"
#include "ctime"

class PartArrayTest2D : public QObject
{
    Q_OBJECT

signals:

private slots:
    void initTestCase();
    void dropRandomI();
    void dropRandomD();
};

#endif // PARTARRAYTEST2D_H
