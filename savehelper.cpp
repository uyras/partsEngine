#include "savehelper.h"
#include "PartArray.h"

SaveHelper::SaveHelper(QString file, bool overwritePreviousData):
f(file)
{
    if (!overwritePreviousData){
        if (!f.open(QFile::ReadOnly))
            qFatal("file %s is unwritable or not found", qUtf8Printable(file));
        QTextStream s;
        s.setDevice(&f);
        QString temp, section;
        while (!s.atEnd()) {
            temp = s.readLine();
            if (temp[0]=='[' && temp[temp.length()-1]==']'){
                temp.chop(1);temp.remove(0,1);
                section = temp;
                strList[section].clear();
            } else{
                if (!strList[section].isEmpty())
                    strList[section].append('\n');
                strList[section].append(temp);
            }
        }
        f.close();
    }
    if (!f.open(QFile::ReadWrite | QFile::Truncate))
        qFatal("file %s is unwritable or not found", qUtf8Printable(file));
}

SaveHelper::~SaveHelper()
{
    this->close();
}

void SaveHelper::go(QString section)
{
    this->section = section;
    strList[section].clear();
}

void SaveHelper::close()
{
    if (f.isOpen()){
        if (strList.contains("header") && strList.contains("parts")){
            QTextStream s;
            s.setDevice(&f);

            //пишем Header
            s<<"[header]"<<endl;
            s<<strList["header"].trimmed();

            //пишем parts
            s<<endl<<"[parts]"<<endl;
            s<<strList["parts"].trimmed();

            QMap<QString, QString>::iterator iter = strList.begin();
            while (iter!=strList.end()){
                if (iter.key()!="header" && iter.key()!="parts"){
                    s<<endl<<"["<<iter.key()<<"]"<<endl;
                    s<<iter.value().trimmed();
                }
                iter++;
            }
        }
        f.close();
    }
}

SaveHelper &SaveHelper::operator <<(const double num)
{
    write(QString::number(num));
    return *this;
}

SaveHelper &SaveHelper::operator <<(const int num)
{
    write(QString::number(num));
    return *this;
}

SaveHelper &SaveHelper::operator <<(const unsigned int num)
{
    write(QString::number(num));
    return *this;
}

SaveHelper &SaveHelper::operator <<(const long int num)
{
    write(QString::number(num));
    return *this;
}

SaveHelper &SaveHelper::operator <<(const QString num)
{
    write(num);
    return *this;
}

SaveHelper &SaveHelper::operator <<(const bool num)
{
    write(QString::number((int)num));
    return *this;
}

void SaveHelper::write(QString str)
{
    if (section.isEmpty())
        qFatal("You can not write to the empty section");
    strList[section].append(str).append("\t");
}

void SaveHelper::line(QString str)
{
    if (section.isEmpty())
        qFatal("You can not write to the empty section");
    strList[section].append(str).append("\n");
}

void SaveHelper::writeHeader(PartArray *sys)
{
    go("header");
    line("version=2");
    line(QString("dimensions=%1").arg(config::Instance()->dimensions()));
    line(QString("type=%1").arg(sys->type()));
    line(QString("size=%1").arg(sys->count()));
    line(QString("emin=%1").arg(sys->eMin));
    line(QString("emax=%1").arg(sys->eMax));
    line(QString("state=%1").arg(QString::fromStdString(sys->state.toString())));
    line(QString("minstate=%1").arg(QString::fromStdString(sys->minstate.toString())));
    line(QString("maxstate=%1").arg(QString::fromStdString(sys->maxstate.toString())));
    line(QString("sizescale=1"));
    line(QString("magnetizationscale=1"));
}

void SaveHelper::writeDumped(QMap<QString, QString> dumped)
{
    QMap<QString, QString>::iterator iter;
    iter = dumped.begin();
    while (iter!= dumped.end()){
        if (!strList.contains(iter.key())){
            strList[iter.key()] = iter.value();
        }
        iter++;
    }
}
