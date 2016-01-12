#include "squarelattice.h"

SquareLattice::SquareLattice():
    m(0),
    n(0),
    l(1)
{
    _type = "squarelattice";
    SysLoader::reg<SquareLattice>(type());
}

void SquareLattice::dropSquareLattice(int m, int n, double l, double len, Part *example)
{
    this->clear(); //очищаем систему

    //сохраняем параметры решетки
    this->m = m;
    this->n = n;
    this->l = l;

    if (example==0){ //если образец не задан, делаем его пустым по умолчанию
        example = new Part();
    }

    Part* temp; //временная частица
    for (int i=0; i<m; i++){
        for (int j=0; j<n; j++){
            temp = new Part(*example); //создаем копию объекта
            temp->m = Vect(0,len,0);
            temp->pos = Vect(i*l, j*l, 0);
            this->insert(temp);
        }
    }
}

StateMachineFree SquareLattice::groundState()
{
    StateMachineFree oldState = state, newState; //сохраняем текущее состояние

    Part* temp; //временная частица

    vector<Part*>::iterator iter = parts.begin();
    int i=0, j=0;
    while (iter != parts.end()){ //обходим все частицы
        temp = *iter;
        if ((temp->m.scalar(Vect(0,1,0))>0) ^ (i%2==1))
            temp->rotate();

        if ((++j) == n){
            j=0; i++;
        }
        iter++;
    }

    newState = state; //сохраняем состояние минимума
    state = oldState; //возвращаем начальное состояние

    return newState;
}

StateMachineFree SquareLattice::maximalState()
{
    StateMachineFree oldState = state, newState;
    Part* temp; //временная частица

    vector<Part*>::iterator iter = parts.begin();
    int i=0, j=0;
    while (iter != parts.end()){
        temp = *iter;
        if ((temp->m.scalar(Vect(0,1,0))>0) ^ (j%2==1))
            temp->rotate();

        if ((++j) == n){
            j=0; i++;
        }
        iter++;
    }

    newState = state;
    state = oldState;

    return newState;
}

void SquareLattice::load(QString file)
{
    this->clear(); //чистим систему

    //грузим основной набор частиц
    PartArray::load(file);

    //читаем файл
    QFile infile(file);
    infile.open(QFile::ReadOnly);
    QTextStream f(&infile);

    //пропускаем до своей секции
    QString s;
    while (s!="[square]"){
        s = f.readLine();
    }

    //читаем три своих поля
    f>>this->m;
    f>>this->n;
    f>>this->l;

    //закрываем файл
    infile.close();
}

void SquareLattice::save(QString file)
{
    //Сохраняем основную часть файла
    PartArray::save(file);

    //Открываем в режиме добавления
    ofstream f(file.toStdString().c_str(), ios_base::app | ios_base::out);

    //пишем секцию
    f<<"[square]"<<endl;

    //пишем свои данные
    f<<this->m<<endl;
    f<<this->n<<endl;
    f<<this->l<<endl;

    //закрываем файл
    f.close();
}

