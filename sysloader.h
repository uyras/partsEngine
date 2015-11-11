#ifndef SYSLOADER_H
#define SYSLOADER_H

#include <string>
#include <map>
#include <QFile>
#include <QTextStream>
#include <QtDebug>
#include "PartArray.h"
#include "squarespinicearray.h"
#include "honeycombspinicearray.h"

using namespace std;

class SysLoader
{
public:
    SysLoader();

    static PartArray* load(QString filename);

private:
    static PartArray* load_v2(QString filename);
    static PartArray* load_v1(QString filename);

    /**
     * @brief checkVersion Возвращает версию файла.
     * @param filename
     * @return 0, если версия не опознана, либо версию формата
     */
    static int checkVersion(QString filename);
};

#endif // SYSLOADER_H
