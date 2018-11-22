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

    /**
     * @brief SaveHelper Конструктор сохранения. Открывает файл для записи
     * @param file Путь до сохраняемого файла
     * @param overwritePreviousData Перезаписать предыдущий файл.
     * Если true, удаляет все данные,
     * если false дописывает в существующий файл.
     * Однако функция go(QString) перезаписывает секцию в любом случае
     */
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
