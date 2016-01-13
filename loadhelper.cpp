#include "loadhelper.h"
#include "PartArray.h"

LoadHelper::LoadHelper(QString file):
    f(file)
{
    if (!f.open(QFile::ReadOnly))
        qFatal("file %s not found", qUtf8Printable(file));
    s.setDevice(&f);
}

LoadHelper::~LoadHelper()
{
    this->close();
}

bool LoadHelper::validate()
{
    //@todo дописать validate
    return LoadHelper::version(f.fileName())>1;
}

bool LoadHelper::go(QString section)
{
    s.seek(0);
    QString str;
    section.push_front('[');
    section.push_back(']');
    while (!s.atEnd() && str != section){
        str = s.readLine();
    }
    return str==section;
}

bool LoadHelper::go(unsigned int num)
{
    s.seek(0);
    for (int i=0;i<(int)num;i++)
        while(!line().isNull()){}
    return s.atEnd();
}

void LoadHelper::close()
{
    if (f.isOpen())
        f.close();
}

QString LoadHelper::line()
{
    if (end())
        return QString();
    else
        return s.readLine();
}

bool LoadHelper::end()
{
    qint64 p = s.pos();
    QString str = s.readLine();
    s.seek(p);
    if (str.isNull())
        return true;
    if (str[0]=='[' && str[str.length()-1]==']')
        return true;
    return false;
}

LoadHelper &LoadHelper::operator >>(double &num)
{
    s>>num;
    return *this;
}

LoadHelper &LoadHelper::operator >>(int &num)
{
    s>>num;
    return *this;
}

LoadHelper &LoadHelper::operator >>(unsigned int &num)
{
    s>>num;
    return *this;
}

LoadHelper &LoadHelper::operator >>(QString &num)
{
    s>>num;
    return *this;
}

LoadHelper &LoadHelper::operator >>(bool &num)
{
    int n;
    s>>n;
    num = (bool)n;
    return *this;
}

void LoadHelper::readHeader(PartArray *sys)
{
    this->readHeader(sys,false);
}

int LoadHelper::version(QString file)
{
    QFile f(file);

    if (f.open(QFile::ReadOnly)) {
        QTextStream infile(&f);
        QString s = infile.readLine();
        if (s=="[header]"){
            f.close();
            return 2;
        } else {
            s = infile.readLine(); //read 2 line
            s = infile.readLine(); //read 3 line
            s = infile.readLine(); //read 4 line
            if (s=="x\ty\tz\tMx\tMy\tMz\tr"){
                f.close();
                return 1;
            } else {
                f.close();
                return 0;
            }
        }
        f.close();
    } else {
        qFatal("file %s not found", qUtf8Printable(file));
    }
}

void LoadHelper::readHeader(PartArray *sys, bool readAnyWay)
{
    //читаем параметры
        map<QString,QString> params;
        QStringList tempList;

        go("header");

        QString str;
        while(!(str = this->line()).isNull()){
            tempList =str.split('=');
            if (tempList.length()!=2)
                qWarning()<<"Skip param"<<str<<"in file"<<f.fileName();
            else {
                params[tempList[0]]=tempList[1];
            }
        }

        if (readAnyWay)
            sys->setType(params["type"]);

        if (sys->type()!=params["type"])
            return;

        switch(params["dimensions"].toInt()){
        case 2:
            config::Instance()->set2D();
            break;
        case 3:
            config::Instance()->set3D();
            break;
        default:
            qFatal("Dimensions in file %s are setted as %s, which is wrong!",qUtf8Printable(f.fileName()),qUtf8Printable(params["dimensions"]));
        }

        sys->eMin = params["emin"].toDouble();
        sys->eMax = params["emax"].toDouble();
        sys->minstate.fromString(qUtf8Printable(params["minstate"]));
        sys->maxstate.fromString(qUtf8Printable(params["maxstate"]));

        //check the state of the system
        if (QString::fromStdString(sys->state.toString()) != params["state"]){
            qFatal("Something gonna worng while reading system: system state and state in the header are not the same");
        }

        //check the system size
        if (sys->count()!=params["size"].toInt()){
            qFatal("Something gonna worng while reading system: system size and size in the header are not the same");
        }
}

QMap<QString, QString> LoadHelper::dumpFileContent()
{
    QMap<QString,QString> strList;
    s.seek(0);
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
    return strList;
}
