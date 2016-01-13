#ifndef SAVEHELPER_H
#define SAVEHELPER_H

#include <QString>
#include <QFile>
#include <QTextStream>
#include <QDebug>

class PartArray;
class SaveHelper
{
    friend class PartArray;
public:
    SaveHelper(QString file, bool overwritePreviousData = false);
    virtual ~SaveHelper();

    void go(QString section);
    void close();

    SaveHelper & operator <<(const double num);
    SaveHelper & operator <<(const int num);
    SaveHelper & operator <<(const unsigned int num);
    SaveHelper & operator <<(const long int num);
    SaveHelper & operator <<(const QString num);
    SaveHelper & operator <<(const bool num);
    void write(QString str);

    void line(QString str = QString());

    void writeHeader(PartArray* sys);

private:
    void writeDumped(QMap<QString,QString> dumped);
    QFile f;
    QMap<QString, QString> strList;
    QString section;
};

#endif // SAVEHELPER_H
