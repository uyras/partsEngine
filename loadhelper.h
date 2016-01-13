#ifndef LOADHELPER_H
#define LOADHELPER_H

#include <QString>
#include <QFile>
#include <QTextStream>
#include <QDebug>

class PartArray;
class LoadHelper
{
    friend class PartArray;
public:
    LoadHelper(QString file);
    virtual ~LoadHelper();

    bool validate();
    bool go(QString section); //переместить курсор к определенной секции
    bool go(unsigned int num); //переместить курсо к секции num
    void close();

    //функции чтения
    QString line();
    bool end();
    LoadHelper & operator >>(double & num);
    LoadHelper & operator >>(int & num);
    LoadHelper & operator >>(unsigned int & num);
    LoadHelper & operator >>(QString & num);
    LoadHelper & operator >>(bool & num);

    void readHeader(PartArray* sys);

    static int version(QString file);

private:
    void readHeader(PartArray *sys, bool readAnyWay);
    QMap<QString,QString> dumpFileContent();
    QFile f;
    QTextStream s;
};

#endif // LOADHELPER_H
