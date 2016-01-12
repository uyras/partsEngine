#ifndef SYSLOADER_H
#define SYSLOADER_H

#include <string>
#include <map>
#include <QFile>
#include <QTextStream>
#include <QtDebug>
#include <QString>
#include "PartArray.h"

using namespace std;

//pattern http://habrahabr.ru/post/129202/
class abstractPartArrayCreator
{
public:
    abstractPartArrayCreator() {}
    virtual PartArray * create() const = 0;
};

template <class C>
class PartArrayCreator : public abstractPartArrayCreator
{
public:
    PartArrayCreator() {}
    virtual PartArray * create() const { return new C(); }
};

class SysLoader
{
public:

    static PartArray* load(QString filename);

    template <class C>
    static void reg(const QString & name){
        typename regmap::iterator it = _registered.find(name);
        if (it == _registered.end()){
            _registered[name] = new PartArrayCreator<C>();
        }
    }

private:
    SysLoader();
    static PartArray* load_v2(QString filename);
    static PartArray* load_v1(QString filename);

    /**
     * @brief checkVersion Возвращает версию файла.
     * @param filename
     * @return 0, если версия не опознана, либо версию формата
     */
    static int checkVersion(QString filename);


    static PartArray *create(const QString & name);
    typedef map<QString, abstractPartArrayCreator*> regmap;
    static regmap _registered;
    static regmap regInit(){
        regmap some_map;
        some_map["standart"] = new PartArrayCreator<PartArray>();
        return some_map;
    }


};

#endif // SYSLOADER_H
