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
    void parseHeader(); //спарсить заголовок в свою внутреннюю структуру
    bool go(QString section); //переместить курсор к определенной секции
    bool go(unsigned int num); //переместить курсо к секции num
    void close();

    //функции чтения
    QString line();
    bool end();
    LoadHelper & operator >>(double & num);
    LoadHelper & operator >>(int & num);
    LoadHelper & operator >>(long & num);
    LoadHelper & operator >>(unsigned int & num);
    LoadHelper & operator >>(QString & num);
    LoadHelper & operator >>(bool & num);

    void applyHeader(PartArray* sys);

    static int version(QString file);

    std::map<QString,QString> params;

private:
    void applyHeader(PartArray *sys, bool readAnyWay);
    QMap<QString,QString> dumpFileContent();
    QFile f;
    QTextStream s;
};

#endif // LOADHELPER_H
