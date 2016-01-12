#include "sysloader.h"

SysLoader::regmap SysLoader::_registered = regInit();

SysLoader::SysLoader()
{

}

PartArray *SysLoader::load(QString filename)
{
    int version = SysLoader::checkVersion(filename);
    switch(version){
    case 1:
        return load_v1(filename);
        break;
    case 2:
        return load_v2(filename);
    default:
        return NULL;
    }
}


PartArray *SysLoader::load_v2(QString filename)
{
    //читаем параметры
    map<QString,QString> params;
    QStringList tempList;
    QFile f(filename);
    f.open(QFile::ReadOnly);

    QTextStream infile(&f);
    QString s;
    s = infile.readLine(); //skip header
    s = infile.readLine(); //read first data line
    while(s!="[parts]" && !infile.atEnd()){
        tempList =s.split('=');
        if (tempList.length()!=2)
            qWarning()<<"Skip param"<<s<<"in file"<<filename;
        else {
            params[tempList[0]]=tempList[1];
        }
        s = infile.readLine();
    }
    f.close();

    switch(params["dimensions"].toInt()){
    case 2:
        config::Instance()->set2D();
        break;
    case 3:
        config::Instance()->set3D();
        break;
    default:
        qFatal("Dimensions in file %s are setted as %s, which is wrong!",qUtf8Printable(filename),qUtf8Printable(params["dimensions"]));
    }

    PartArray *sys = SysLoader::create(params["type"]);
    if (sys==0)
        sys = new PartArray();
    sys->load(filename);
    //qFatal("System type %s is not supported",qUtf8Printable(params["type"]));
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
    return sys;
}

PartArray *SysLoader::load_v1(QString filename)
{
    PartArray* sys = new PartArray();
    sys->load_v1(filename.toStdString());
    return sys;
}

int SysLoader::checkVersion(QString filename)
{
    QFile f(filename);

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
            } else return 0;
        }
    } else {
        qFatal("file %s not found", qUtf8Printable(filename));
    }
}

PartArray *SysLoader::create(const QString &name)
{
    typename regmap::iterator it = _registered.find(name);
    if (it != _registered.end())
        return it->second->create();
    return 0;
}
