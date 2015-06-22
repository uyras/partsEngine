/*
* File:   PartArray.cpp
* Author: uyras
*
* Created on 20 Ноябрь 2012 г., 17:22
*/

#include "PartArray.h"
#include "Part.h"
#include "config.h"
#include <cmath>
#include <iostream>
#include <fstream>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdexcept>


PartArray::PartArray() {
    this->_construct();
}

PartArray::~PartArray(){
    this->clear();
    delete this->state;
}

PartArray::PartArray(double x, double y, double z) {
    this->_construct();
    this->resize(x,y,z); //изменяем размер подложки
}

void PartArray::operator= (const PartArray& a){
    this->E1 = a.E1;
    this->E2 = a.E2;
    this->eIncrementalTemp = a.eIncrementalTemp;
    this->parts = a.parts;
    this->size = a.size;
    this->state = a.state;
    this->absSize = a.absSize;
}

PartArray* PartArray::copy(){
    PartArray *temp = new PartArray();
    temp->E1 = this->E1;
    temp->E2 = this->E2;
    temp->eIncrementalTemp = this->eIncrementalTemp;
    temp->size = this->size;
    temp->absSize = this->absSize;

    vector<Part*>::iterator iter = this->parts.begin();
    while(iter!=this->parts.end()){
        temp->insert((*iter)->copy());
        iter++;
    }

    return temp;
}

PartArray::PartArray(double x, double y, double z, double density) {
    this->_construct();

    this->resize(x,y,z); //изменяем размер подложки
    this->clear();
    this->dropRandom(density); //набрасываем в массив частицы
}

PartArray::PartArray(double x, double y, double z, int count) {
    this->_construct();

    this->resize(x,y,z); //изменяем размер подложки
    this->clear();
    this->dropRandom(count); //набрасываем в массив частицы
}

PartArray::PartArray(string file) {
    this->_construct();
    this->load(file);
}

void PartArray::resize(double x, double y, double z){
    this->size.x = x;
    this->size.y = y;
    this->size.z = z;
    this->E1 = this->E2 = 0; //обнуляем энергии системы
    this->clear(); //чистим массив частиц
}

void PartArray::dropRandom(double maxDestiny) {

    double surfVol = this->size.x * this->size.y;
    if (config::Instance()->dimensions()==3) surfVol *= this->size.z; //считаем объем (площадь) поверхности, в которую кидаем частицы
    Part* temp; //временная частица
    int partCount = this->parts.size(); //количество сброшеных частиц
    double destiny; //плотность заполнения образца

    std::vector < Part* >::iterator iterator1; // итератор для обхода массива частиц
    bool regenerate; //Флаг, нужен для проверки перекрещивания частиц
    int dropErrorCount=0; //количество ошибочных сбросов подряд

    do {

        //std::cout <<"Drop "<<partCount<<endl;

        temp = new Part();
        dropErrorCount=0;
        do {
            regenerate = false;
            //генерим координаты
            temp->pos.x = config::Instance()->rand() % ((int)(this->size.x-config::Instance()->partR*2)*100);
            temp->pos.y = config::Instance()->rand() % ((int)(this->size.y-config::Instance()->partR*2)*100);
            if (config::Instance()->dimensions()!=3) //если работаем в плоскости, то генерить третью координату не надо
                temp->pos.z = 0;
            else
                temp->pos.z = config::Instance()->rand() % ((int)(this->size.z-config::Instance()->partR*2)*100);

            temp->pos.x = temp->pos.x / 100 + config::Instance()->partR;
            temp->pos.y = temp->pos.y / 100 + config::Instance()->partR;
            temp->pos.z = temp->pos.z / 100 + config::Instance()->partR;

            //проверяем чтобы сгенная точка не пересекалась ни с какой другой (это значит что площади их сфер не пересекались)
            iterator1 = this->parts.begin();
            while (iterator1 != this->parts.end()) {
                if (temp->pos.radius((*iterator1)->pos).length()<=config::Instance()->partR*2){
                    regenerate=true;

                    //std::cout<<"Drop "<<partCount<<" particle error, repeat"<<endl;
                    break;
                }
                ++iterator1;
            }
            dropErrorCount++;
        } while (regenerate && dropErrorCount<50000);

        if (dropErrorCount>=50000) break; //если частицы уже не кидаются, брэйк

        //генерируем вектор частицы
        double longitude=(double)config::Instance()->rand()/(double)config::Instance()->rand_max*2*M_PI;
        double lattitude;
        if (config::Instance()->dimensions()==2)
            lattitude=0; // если частица 2-х мерная то угол отклонения должен быть 0
        else
            lattitude=(double)config::Instance()->rand()/(double)config::Instance()->rand_max*2-1;

        temp->m.x = config::Instance()->m * cos(longitude)*sqrt(1-lattitude*lattitude);
        temp->m.y = config::Instance()->m * sin(longitude)*sqrt(1-lattitude*lattitude);
        temp->m.z = config::Instance()->m * lattitude;

        //добавляем частицу на экземпляр
        insert(temp);
        partCount++;

        //считаем плотность заполнения экземпляра
        destiny = (config::Instance()->vol * partCount) / surfVol;
    } while (destiny < maxDestiny);
}

void PartArray::dropRandom(int count) {
    //double surfVol = this->size.x * this->size.y * this->size.z; //считаем объем (площадь) поверхности, в которую кидаем частицы
    Part* temp; //временная частица
    int partCount = this->parts.size(); //количество сброшеных частиц
    count+=partCount;

    std::vector < Part* >::iterator iterator1; // итератор для обхода массива частиц
    bool regenerate; //Флаг, нужен для проверки перекрещивания частиц
    while (partCount < count) {
        //std::cout <<"Drop "<<partCount<<endl;
        temp = new Part();
        do {
            regenerate = false;
            //генерим координаты
            temp->pos.x = config::Instance()->rand() % ((int)(this->size.x-config::Instance()->partR*2)*100);
            temp->pos.y = config::Instance()->rand() % ((int)(this->size.y-config::Instance()->partR*2)*100);
            if (config::Instance()->dimensions()==2) //если работаем в плоскости, то генерить третью координату не надо
                temp->pos.z = 0;
            else
                temp->pos.z = config::Instance()->rand() % ((int)(this->size.z-config::Instance()->partR*2)*100);

            temp->pos.x = temp->pos.x / 100 + config::Instance()->partR;
            temp->pos.y = temp->pos.y / 100 + config::Instance()->partR;
            temp->pos.z = temp->pos.z / 100 + config::Instance()->partR;

            //проверяем чтобы сгенная точка не пересекалась ни с какой другой (это значит что площади их сфер не пересекались)
            iterator1 = this->parts.begin();
            while (iterator1 != this->parts.end()) {
                if (temp->pos.radius((*iterator1)->pos).length()<=config::Instance()->partR*2){
                    regenerate = true;
                    //std::cout<<"Drop "<<partCount<<" particle error, repeat"<<endl;
                    break;
                }
                ++iterator1;
            }
        } while (regenerate);

        //генерируем вектор частицы
        double longitude = ((double)config::Instance()->rand()/(double)config::Instance()->rand_max) * 2. * M_PI;
        double lattitude;
        if (config::Instance()->dimensions()==2) lattitude=0; else lattitude=(double)config::Instance()->rand() / (double)config::Instance()->rand_max * 2. - 1.; // если частица 2-х мерная то угол отклонения должен быть 0

        temp->m.x = config::Instance()->m * cos(longitude) * sqrt(1-lattitude*lattitude);
        temp->m.y = config::Instance()->m * sin(longitude) * sqrt(1-lattitude*lattitude);
        temp->m.z = config::Instance()->m * lattitude;

        //добавляем частицу на экземпляр
        this->insert(temp);
        partCount++;
    }
}

void PartArray::dropChain(double distance){
    double rad = config::Instance()->partR;
    if (distance < 2.*rad)
        distance = 2.*rad;
    if (config::Instance()->dimensions()==2){
        short int up = 1;
        Part* temp;
        double x = rad;
        while (x <= this->size.x-rad) {
            double y = rad;
            while (y <= this->size.y-rad){
                temp = new Part();
                temp->m.y = config::Instance()->m * up;
                temp->m.x = temp->m.z = 0;

                temp->pos.x = x;
                temp->pos.y = y;
                temp->pos.z = 0;

                y+=distance;
                this->insert(temp);
            }
            up *= -1;
            x+=distance;
        }
    } else {
        short int up1=1, up2 = 1;
        Part* temp;
        double x = rad;
        while (x <= this->size.x-rad) {
            up2 = up1;
            double z = rad;
            while (z <= this->size.z-rad){
                double y = rad;
                while (y <= this->size.y-rad){
                    temp = new Part();
                    temp->m.y = config::Instance()->m * up2;
                    temp->m.x = temp->m.z = 0;

                    temp->pos.x = x;
                    temp->pos.y = y;
                    temp->pos.z = z;

                    y+=distance;
                    this->insert(temp);
                }
                up2 *= -1;
                z+=distance;
            }
            up1 *= -1;
            x+=distance;
        }
    }
}

void PartArray::dropSpinIce(double partW, double partH, double lattice)
{
    if (config::Instance()->dimensions()!=2)
        return;

    if (partW+partH>lattice) //если параметр решетки слишком маленький, увеличиваем до нужного
        lattice=partH+partW;

    double a = fmin(partW, partH); //которкая сторона овала
    double b = fmax(partW, partH); //длиная сторона овала

    double firstSpace = lattice/2.+a/2.;
    Vect center = Vect(firstSpace,firstSpace,0);

    Part *temp;
    while (center.y < this->size.y + lattice) {
        center.x = firstSpace;
        while(center.x < this->size.x + lattice){
            //добавляем горизонтальные
            if (
                    center.x + b/2. < this->size.x &&
                    center.y - lattice/2. + a/2. < this->size.y
                    ){
                temp = new Part();
                temp->shape = Part::ELLIPSE;
                temp->pos = Vect(center.x, center.y-lattice/2.,0);
                temp->m = Vect(config::Instance()->m,0,0);
                temp->w1 = b; temp->h1 = a;
                this->insert(temp);
            }

            //добавляем вертикальные
            if (
                    center.x - lattice/2. + a/2. < this->size.x &&
                    center.y + b/2. < this->size.y
                    ){
                temp = new Part();
                temp->shape = Part::ELLIPSE;
                temp->pos = Vect(center.x-lattice/2.,center.y,0);
                temp->m = Vect(0,config::Instance()->m,0);
                temp->w1 = a; temp->h1 = b;
                this->insert(temp);
            }
            center.x+=lattice;
        }
        center.y+=lattice;
    }
}

void PartArray::dropHoneyComb(int m, int n, double a, Part *tmp)
{
    double mLength=0; //магнитный момент одной частицы
    if (tmp==0){ //если шаблон частицы не был передан, делаем шаблон по умолчанию
        tmp = new Part();
        mLength = config::Instance()->m;
    } else {
        mLength = tmp->m.length();
    }

    double r=a*sqrt(3)/2;
    for(int i=0;i<m;i++)
    {
        for(int j=0;j<n;j++)
        {
            double x=2*r*i+r*(pow(-1,j)+1)/2;
            double y=sqrt(3*r*r)*j;
            vector<Part*> hexPart;

            //genHexPart start
            for(int k=0;k<6;k++)
            {
                Part* temp = tmp->copy();
                temp->pos.x = r*cos(2*M_PI*k/6)+x;
                temp->pos.y = r*sin(2*M_PI*k/6)+y;
                temp->m.x = cos(2*M_PI*k/6+M_PI/2)*mLength;
                temp->m.y = sin(2*M_PI*k/6+M_PI/2)*mLength;
                hexPart.push_back(temp);
            }
            //genHexPart end

            vector<Part*>::iterator iter = hexPart.begin();
            while (iter!=hexPart.end()){
                bool add=true;
                vector<Part*>::iterator iter2 = this->parts.begin();
                while(iter2!=this->parts.end()){
                    if (
                            this->_double_equals((*iter2)->pos.x,(*iter)->pos.x) &&
                            this->_double_equals((*iter2)->pos.y,(*iter)->pos.y)
                            )
                            add=false;
                    iter2++;
                }

                if(add)
                    this->insert(*iter);
                else
                    delete (*iter); //удаляем из памяти

                iter++;
            }
        }
    }
}

//перемешать магнитные моменты частиц M
void PartArray::shuffleM(){
    bool rotate;
    for (int i=0;i<this->count();i++){
        rotate = (double)config::Instance()->rand()/(double)config::Instance()->rand_max > 0.5;
        if (rotate)
            this->parts[i]->rotate();
    }
}

void PartArray::insert(Part * part){
    this->parts.push_back(part);
}

void PartArray::dropLattice(double distance){
    this->clear();
    Part* temp; //временная частица
    for(double x=config::Instance()->partR; x<=this->size.x-config::Instance()->partR; x+=config::Instance()->partR*2+distance){
        for(double y=config::Instance()->partR; y<=this->size.y-config::Instance()->partR; y+=config::Instance()->partR*2+distance){
            temp = new Part();
            //генерируем вектор частицы
            double longitude=(double)config::Instance()->rand()/(double)config::Instance()->rand_max*2*M_PI;
            double lattitude;
            if (config::Instance()->dimensions()==2) lattitude=0; else lattitude=(double)config::Instance()->rand()/(double)config::Instance()->rand_max*2-1; // если частица 2-х мерная то угол отклонения должен быть 0

            temp->m.x = config::Instance()->m * cos(longitude)*sqrt(1-lattitude*lattitude);
            temp->m.y = config::Instance()->m * sin(longitude)*sqrt(1-lattitude*lattitude);
            temp->m.z = config::Instance()->m * lattitude;
            temp->pos.x = x;
            temp->pos.y = y;
            temp->pos.z = 0;

            //добавляем частицу на экземпляр
            this->insert(temp);
        }
    }
}

double PartArray::destiny(bool simple){
    double surfVol = this->size.x * this->size.y;
    if (config::Instance()->dimensions()==3) surfVol *= this->size.z; //считаем объем (площадь) поверхности, в которую кидаем частицы
    if (simple)
        return (config::Instance()->vol * this->parts.size()) / surfVol;
    else{
        double destiny=0;
        std::vector < Part* >::iterator iterator1 = this->parts.begin();
        while (iterator1!=this->parts.end()){
            destiny+=(*iterator1)->volume()/surfVol;
            iterator1++;
        }
        return destiny;
    }
}

Vect PartArray::M(){
    return this->calcM1();
}

Vect PartArray::calcM1(){
    Vect temp;
    vector<Part*>::iterator iter = this->parts.begin();
    while (iter!=this->parts.end()){
        temp.x += (*iter)->m.x;
        temp.y += (*iter)->m.y;
        if (config::Instance()->dimensions()==3)
            temp.z += (*iter)->m.z;
        iter++;
    }
    return temp;
}

Vect PartArray::calcM2(){
    Vect temp;
    vector<Part*>::iterator iter = this->parts.begin();
    while (iter!=this->parts.end()){
        temp.x += fabs((*iter)->m.x);
        temp.y += fabs((*iter)->m.y);
        if (config::Instance()->dimensions()==3)
            temp.z += fabs((*iter)->m.z);
        iter++;
    }
    return temp;
}

Vect PartArray::calcM12(){
    Vect temp1, temp2;
    vector<Part*>::iterator iter = this->parts.begin();
    while (iter!=this->parts.end()){
        temp1.x += (*iter)->m.x;
        temp2.x += fabs((*iter)->m.x);

        temp1.y += (*iter)->m.y;
        temp2.y += fabs((*iter)->m.y);

        if (config::Instance()->dimensions()==3) {
            temp1.z += (*iter)->m.z;
            temp2.z += fabs((*iter)->m.z);
        }

        iter++;
    }
    temp1.x /= temp2.x;
    temp1.y /= temp2.y;
    if (config::Instance()->dimensions()==3)
        temp1.z /= temp2.z;

    return temp1;
}

void PartArray::calcH(Part* elem) {
    std::vector < Part* >::iterator iterator1;
    elem->h.x = 0;
    elem->h.y = 0;
    elem->h.z = 0;
    iterator1 = this->parts.begin();
    while (iterator1 != this->parts.end()) {
        if (elem->pos.x != (*iterator1)->pos.x || elem->pos.y != (*iterator1)->pos.y || elem->pos.z != (*iterator1)->pos.z) { //не считать взаимодействие частицы на себя
            elem->h += elem->interact(*iterator1);
        }
        ++iterator1;
    }
}

void PartArray::calcH() {
    std::vector < Part* >::iterator iterator2;
    iterator2 = this->parts.begin();
    while (iterator2 != this->parts.end()) {
        this->calcH(*iterator2);
        iterator2++;
    }
}


double PartArray::calcEnergy1(Vect& H) {
    std::vector < Part* >::iterator iterator2;
    this->E1 = 0;
    iterator2 = this->parts.begin();
    while (iterator2 != this->parts.end()) {
        this->E1 += this->calcEnergy1(*iterator2) + H.scalar((*iterator2)->m);
        iterator2++;
    }
    this->E1 *= 0.5;
    return this->E1;
}

double PartArray::calcEnergy1(){
    Vect temp = Vect(0,0,0);
    return this->calcEnergy1(temp);
}

double PartArray::calcEnergy1(Part* elem) {
    std::vector < Part* >::iterator iterator1;
    double r, r2, r5, E = 0;
    Vect rij;
    iterator1 = this->parts.begin();
    while (iterator1 != this->parts.end()) {
        if (elem->pos.x != (*iterator1)->pos.x || elem->pos.y != (*iterator1)->pos.y || elem->pos.z != (*iterator1)->pos.z) { //не считать взаимодействие частицы на себя
            rij = (*iterator1)->pos.radius(elem->pos);
            r = rij.length();
            r2 = r * r; //радиус в кубе
            r5 = r2 * r * r * r; //радиус в пятой
            E += //энергии отличаются от формулы потому что дроби внесены под общий знаменатель
                    (((*iterator1)->m.scalar(elem->m) * r2)
                     -
                     (3 * elem->m.scalar(rij) * (*iterator1)->m.scalar(rij))) / r5; //энергия считается векторным методом, так как она не нужна для каждой оси
        }
        ++iterator1;
    }
    return E;
}

void PartArray::calcEnergy1Fast(){
    std::vector < Part* >::iterator iterator1, iterator2, beginIterator, endIterator;
    double r, r2, r5, rijx, rijy;

    this->E1 = 0;
    beginIterator = iterator2 = this->parts.begin();
    endIterator = this->parts.end();

    while (iterator2 != endIterator) {
        iterator1 = beginIterator;
        while (iterator1 != endIterator) {

            if (iterator2 != iterator1) { //не считать взаимодействие частицы на себя
                rijx = (*iterator2)->pos.x - (*iterator1)->pos.x;
                rijy = (*iterator2)->pos.y - (*iterator1)->pos.y;
                r2 = rijx*rijx+rijy*rijy;
                r = sqrt(r2); //трудное место, заменить бы
                r5 = r2 * r2 * r; //радиус в пятой
                this->E1 += //энергии отличаются от формулы потому что дроби внесены под общий знаменатель
                        (( ((*iterator1)->m.x*(*iterator2)->m.x+(*iterator1)->m.y*(*iterator2)->m.y) * r2)
                         -
                         (3 * ((*iterator2)->m.x * rijx + (*iterator2)->m.y * rijy) * ((*iterator1)->m.x * rijx + (*iterator1)->m.y * rijy)  )) / r5; //энергия считается векторным методом, так как она не нужна для каждой оси

            }

            ++iterator1;
        }
        iterator2++;
    }

    this->E1 *= 0.5;
}


double PartArray::calcEnergy1FastIncremental(double initEnergy){
    vector<Part*>::iterator iter; //итератор обхода состояния

    std::vector<int> zeros; //хранит инфу на каких позициях нули
    //ищем нули
    for (unsigned int j=0;j<state->size();j++){
        if ((*this->state)[j]==0){
            zeros.push_back(j);
        }
    }


    //рассчитываем энергию
    double E=initEnergy;
    std::vector<int>::iterator iter2;
    iter = parts.begin();
    while(iter != parts.end()){
        if ( (*iter)->state == true){
            iter2 = zeros.begin();
            while (iter2!=zeros.end()){
                E -=  ( 2. * (*iter)->eArray.at(*iter2));
                iter2++;
            }
        }
        iter++;
    }

    return E;
}

double PartArray::calcEnergy1FastIncrementalFirst(){
    this->state->reset();//при первом запуске состояние системы должно быть 0
    double eIncrementalTemp = 0;

    std::vector < Part* >::iterator iterator1, iterator2, beginIterator, endIterator;
    double r, r2, r5, rijx, rijy, rijz, E;

    beginIterator = iterator2 = this->parts.begin();
    endIterator = this->parts.end();

    while (iterator2 != endIterator) {
        (*iterator2)->e = 0;
        (*iterator2)->eArray.clear();
        iterator1 = beginIterator;
        while (iterator1 != endIterator) {

            if (iterator2 != iterator1) { //не считать взаимодействие частицы на себя
                rijx = (*iterator2)->pos.x - (*iterator1)->pos.x;
                rijy = (*iterator2)->pos.y - (*iterator1)->pos.y;
                rijz = (*iterator2)->pos.z - (*iterator1)->pos.z;
                if (config::Instance()->dimensions()==2)
                    r2 = rijx*rijx+rijy*rijy;
                else
                    r2 = rijx*rijx+rijy*rijy+rijz*rijz;
                r = sqrt(r2); //трудное место, заменить бы
                r5 = r2 * r2 * r; //радиус в пятой
                if (config::Instance()->dimensions()==2)
                    E = //энергия считается векторным методом, так как она не нужна для каждой оси
                            (( ((*iterator1)->m.x*(*iterator2)->m.x+(*iterator1)->m.y*(*iterator2)->m.y) * r2)
                             -
                             (3 * ((*iterator2)->m.x * rijx + (*iterator2)->m.y * rijy) * ((*iterator1)->m.x * rijx + (*iterator1)->m.y * rijy)  )) / r5;
                else
                    E = //энергия считается векторным методом, так как она не нужна для каждой оси
                            (( ((*iterator1)->m.x*(*iterator2)->m.x+(*iterator1)->m.y*(*iterator2)->m.y+(*iterator1)->m.z*(*iterator2)->m.z) * r2)
                             -
                             (3 * ((*iterator2)->m.x * rijx + (*iterator2)->m.y * rijy + (*iterator2)->m.z * rijz) * ((*iterator1)->m.x * rijx + (*iterator1)->m.y * rijy + (*iterator1)->m.z * rijz)  )) / r5;

                (*iterator2)->e += E;//энергии отличаются от формулы потому что дроби внесены под общий знаменатель
                (*iterator2)->eArray.push_back(E);
            } else {
                (*iterator2)->eArray.push_back(0);
            }

            ++iterator1;
        }

        eIncrementalTemp += (*iterator2)->e;

        iterator2++;
    }

    return eIncrementalTemp *= 0.5; //делим на два, так как в цикле считается и E12 и E21, хотя по факту они равны
}

double PartArray::calcEnergy2() {
    std::vector < Part* >::iterator iterator2;
    this->E2 = 0;
    iterator2 = this->parts.begin();
    while (iterator2 != this->parts.end()) {
        this->E2 -= (*iterator2)->h.scalar((*iterator2)->m);
        iterator2++;
    }
    this->E2 *= 0.5;
    return this->E2;
}

void PartArray::cout() {
    std::vector < Part* >::iterator iterator1;
    std::cout << "X\tY\tZ\tMx\tMy\tMz\t\tHx\tHy\tHz\t|H|" << std::endl;
    iterator1 = this->parts.begin();
    while (iterator1 != this->parts.end()) {
        std::cout
                << (*iterator1)->pos.x << "\t"
                << (*iterator1)->pos.y << "\t"
                << (*iterator1)->pos.z << "\t"
                << (*iterator1)->m.x << "\t"
                << (*iterator1)->m.y << "\t"
                << (*iterator1)->m.z << "\t\t"/*
                           << (*iterator1)->h.x << "\t"
                           << (*iterator1)->h.y << "\t"
                           << (*iterator1)->h.z << "\t"*/
                << std::endl;
        ++iterator1;
    }
    std::cout << "E1 : " << this->E1 << "; E2 : " << this->E2 << std::endl;
}

std::vector<double> PartArray::getEVector() {
    std::vector<double> e;
    std::vector < Part* >::iterator iterator1 = this->parts.begin();

    while (iterator1 != this->parts.end()) {
        e.push_back((*iterator1)->e);
        ++iterator1;
    }
    return e;
}

std::vector<double> PartArray::getHVector() {
    std::vector<double> h;
    std::vector < Part* >::iterator iterator1 = this->parts.begin();

    while (iterator1 != this->parts.end()) {
        h.push_back((*iterator1)->h.length());
        ++iterator1;
    }
    return h;
}

std::vector<double> PartArray::getHZVector() {
    std::vector<double> h;
    std::vector < Part* >::iterator iterator1 = this->parts.begin();

    while (iterator1 != this->parts.end()) {
        h.push_back((*iterator1)->h.z);
        ++iterator1;
    }
    return h;
}

//void PartArray::setAntiferr() {
//    std::vector < Part >::iterator iterator1;
//    iterator1 = this->parts.begin();
//    while (iterator1 != this->parts.end()) {
//        if ((int) ((*iterator1).pos.x + (*iterator1).pos.z) % 2 == 1) (*iterator1).axis.z = -1.;
//        else (*iterator1).axis.z = 1;
//        (*iterator1).axis.y = 0.;
//        (*iterator1).axis.x = 0.;
//
//        ++iterator1;
//    }
//    this->calcM();
//
//    this->calcInteraction();
//    this->calcEnergy1();
//    this->calcEnergy2();
//}

//void PartArray::setLines() {
//    std::vector < Part >::iterator iterator1;
//    iterator1 = this->parts.begin();
//    while (iterator1 != this->parts.end()) {
//        if ((int) ((*iterator1).pos.x) % 2 == 1) (*iterator1).axis.z = -1.;
//        else (*iterator1).axis.z = 1;
//        (*iterator1).axis.y = 0.;
//        (*iterator1).axis.x = 0.;
//
//        ++iterator1;
//    }
//    this->calcM();
//
//    this->calcInteraction();
//    this->calcEnergy1();
//    this->calcEnergy2();
//}

void PartArray::setMAllUp() {
    std::vector < Part* >::iterator iterator1;
    iterator1 = this->parts.begin();
    while (iterator1 != this->parts.end()) {

        (*iterator1)->m.x = 0.;
        (*iterator1)->m.y = 1.;
        (*iterator1)->m.z = 0.;
        ++iterator1;
    }
}

void PartArray::setMRandom() {
    std::vector < Part* >::iterator iterator1;
    srand((unsigned int)time(NULL));
    iterator1 = this->parts.begin();
    while (iterator1 != this->parts.end()) {
        if (config::Instance()->rand() % 2 == 1) (*iterator1)->m.z = 1.;
        else (*iterator1)->m.z = -1.;
        (*iterator1)->m.y = 0.;
        (*iterator1)->m.x = 0.;
        ++iterator1;
    }
}

std::vector<double> PartArray::processStep() {
    std::vector<Part*>::iterator iter;
    std::vector<double> history;
    iter = this->parts.begin();
    this->calcH();
    history.push_back(this->calcEnergy2());
    while (iter != this->parts.end()) {
        if ((*iter)->h.length() > config::Instance()->hc && (*iter)->h.scalar((*iter)->m) < 0) {
            //std::cout << "rotate with x=" << (*iter).pos.x << "; y=" << (*iter).pos.y << std::endl;
            (*iter)->m.rotate();
            this->calcH();
            history.push_back(this->calcEnergy2());
            iter = this->parts.begin();
        } else {
            //std::cout << "normal with x=" << (*iter).pos.x << "; y=" << (*iter).pos.y << std::endl;
            iter++;
        }
        if (history.size()>10000)
            break;
    }
    return history;
}

std::vector<double> PartArray::processMaxH() {
    std::vector<double> history; //история переворота
    std::vector<Part*>::iterator iter; //итератор для перебора массива частиц
    Part* mElem = NULL; //тут будет храниться максимальная частица. по умолчанию максимальная - первая
    this->calcH();
    bool hasUnstable = true;
    while (hasUnstable) {
        hasUnstable = false;
        mElem = NULL; //чистим максимум
        iter = this->parts.begin();
        while (iter != this->parts.end()) {
            if ((*iter)->h.length() > config::Instance()->hc && (*iter)->h.scalar((*iter)->m) < 0) {
                hasUnstable = true;
                //на первом шаге цикла нужно просто присвоить максимуму значение, а на остальных уже сравнивать
                if (mElem == NULL) {
                    mElem = *iter;
                } else {
                    if (mElem->h.length() < (*iter)->h.length())
                        mElem = *iter;
                }
            }
            iter++;
        }

        //если найдена нестабильная частица - вращаем её
        if (hasUnstable) {
            mElem->m.rotate();
            this->calcH();
            //сохраняем новую энергию системы в историю
            this->calcEnergy2();
            history.push_back(this->E2);
            //std::cout << "rotate with x=" << mElem->pos.x << "; z=" << mElem->pos.z << std::endl;
        }
        if (history.size()>10000)
            break;
    }
    return history;
}

std::vector<double> PartArray::processGroupMaxH() {
    this->calcH();
    std::vector<double> history; //история переворота
    std::vector<Part*> unstable; //все нестабильные частицы здесь при просчете
    std::vector<Part*>::iterator iter; //итератор для перебора массива частиц
    std::vector<Part*>::iterator iter2; //итератор для перебора нестабильных частиц
    int maxH = 0; //максимальный модуль поля взаимодействия
    bool hasUnstable = true; //есть нестабильные частицы или нет
    while (hasUnstable) {
        int rSize = 0; //счетчик, нужен для баловства
        hasUnstable = false;
        unstable.clear(); //чистим набор нестабильных частиц

        maxH = 0;

        //step 1 - сначала находим максимальный магнитный момент системы
        iter = this->parts.begin();
        while (iter != this->parts.end()) {
            if ((*iter)->h.length() > config::Instance()->hc && (*iter)->h.scalar((*iter)->m) < 0) {
                unstable.push_back(*iter);
                hasUnstable = true;
                if ((int) (*iter)->h.length() > maxH)
                    maxH = (int) (*iter)->h.length();
            }
            iter++;
        }

        //step 2 - переворачиваем ТОЛЬКО частицы с максимальной нестабильностью
        iter2 = unstable.begin();
        while (iter2 != unstable.end()) {
            if ((int) (*iter2)->h.length() == maxH) {
                (*iter2)->m.rotate();
                rSize++;
            }
            iter2++;
        }

        //step 3 - если чтото было перевернуто - обновляем поля взаимодействия
        if (hasUnstable) {
            //std::cout << "rotate group from " << rSize << " elements" << std::endl;
            this->calcH();
            //сохраняем новую энергию системы в историю
            this->calcEnergy2();
            history.push_back(this->E2);
        }

        if (history.size()>10000)
            break;
    }

    return history;
}

std::vector<double> PartArray::processGroupStep() {
    this->calcH();
    std::vector<double> history; //история переворота
    std::vector<Part*> unstable; //все нестабильные частицы здесь при просчете
    std::vector<Part*>::iterator iter; //итератор для перебора массива частиц
    std::vector<Part*>::iterator iter2; //итератор для перебора нестабильных частиц
    bool hasUnstable = true; //есть нестабильные частицы или нет
    while (hasUnstable) {

        this->calcH();
        //сохраняем новую энергию системы в историю
        this->calcEnergy2();
        history.push_back(this->E2);
        //draw();
        //std::cout << this->E2 << endl;

        hasUnstable = false;
        unstable.clear(); //чистим набор нестабильных частиц

        //step 1 - сначала находим все неустойчивые элементы
        iter = this->parts.begin();
        while (iter != this->parts.end()) {
            if ((*iter)->h.length() > config::Instance()->hc && (*iter)->h.scalar((*iter)->m) < 0) {
                unstable.push_back(*iter);
                hasUnstable = true;
            }
            iter++;
        }

        //step 2 - переворачиваем все настабильные частицы
        iter2 = unstable.begin();
        while (iter2 != unstable.end()) {
            (*iter2)->m.rotate();
            iter2++;
        }

        if (history.size()>10000)
            break;
    }

    return history;
}


std::vector<double> PartArray::processHEffective() {
    this->calcH();
    std::vector<double> history; //история переворота
    std::vector<Part*> unstable; //частицы, подлежащие перевороту при текущей итерации
    std::vector<Part*>::iterator iter; //итератор для перебора массива частиц
    std::vector<Part*>::iterator iter2; //итератор для перебора нестабильных частиц
    double averH = 0.; //максимальный модуль поля взаимодействия
    int averCount = 0; //количество значений, положеных в среднее
    bool hasUnstable = true; //есть нестабильные частицы или нет
    while (hasUnstable) {
        int rSize = 0; //счетчик, нужен для баловства, всерьёз не воспринимать
        hasUnstable = false;
        unstable.clear(); //чистим набор нестабильных частиц

        averH = 0.;
        averCount = 0;

        //step 1 - сначала находим средний магнитный момент всех частиц
        iter = this->parts.begin();
        while (iter != this->parts.end()) {
            if ((*iter)->h.length() > config::Instance()->hc && (*iter)->h.scalar((*iter)->m) < 0) {
                averH = (averH * averCount + (*iter)->h.length()) / (double) (averCount + 1);
                averCount++;
                unstable.push_back(*iter);
                hasUnstable = true;
            }
            iter++;
        }

        //step 2 - переворачиваем ТОЛЬКО частицы у которых энергия выше или равна среднему
        iter2 = unstable.begin();
        while (iter2 != unstable.end()) {
            if ((int) (*iter2)->h.length() >= (int) averH) {
                (*iter2)->m.rotate();
                rSize++;
            }
            iter2++;
        }

        //step 3 - если чтото было перевернуто - обновляем поля взаимодействия
        if (hasUnstable) {
            //std::cout << "rotate group from " << rSize << " elements" << std::endl;
            this->calcH();
            //сохраняем новую энергию системы в историю
            this->calcEnergy2();
            history.push_back(this->E2);
        }

        if (history.size()>10000)
            break;
    }

    return history;
}

void PartArray::save(string file, bool showNotifications) {

    if (showNotifications)
        std::cout<<"save "<<file<<" file start"<<endl;
    std::ofstream f(file.c_str());

    //сначала сохраняем xyz
    f << this->size.x << endl;
    f << this->size.y << endl;
    f << this->size.z << endl;

    //строка заголовков
    f << "x\ty\tz\tMx\tMy\tMz\tr" << endl;

    //затем все магнитные моменты системы и положения точек
    vector<Part*>::iterator iter = this->parts.begin();

    string shape;
    if (iter!=this->parts.end())
        switch (((Part*)*iter)->shape) {
        case Part::CIRCLE:
            shape="CIRCLE";
            break;
        case Part::ELLIPSE:
            shape="ELLIPSE";
        case Part::SQUARE:
            shape="SQUARE";
        };

    while (iter != this->parts.end()) {
        f << (*iter)->pos.x << "\t";// << endl;
        f << (*iter)->pos.y << "\t";// << endl;
        f << (*iter)->pos.z << "\t";// << endl;
        f << (*iter)->m.x << "\t";// << endl;
        f << (*iter)->m.y << "\t";// << endl;
        f << (*iter)->m.z << "\t";// << endl;
        f << (*iter)->w1 << "\t";// << endl;
        f << (*iter)->h1 << "\t";// << endl;
        //f << (*iter)->sector << "\t";// << endl;
        f << (*iter)->r << "\t";
        f << shape << endl;
        iter++;
    }
    f.close();

    if (showNotifications)
        std::cout<<"save "<<file<<" file complete"<<endl;

}

void PartArray::savePVPython(string file, int thteta, int phi)
{
    //Vect delta(size.x/2.,size.y/2.,size.z/2.);
    Vect delta(0.,0.,0.);
    std::ofstream f(file.c_str());
    f<<"try: paraview.simple"<<endl<<
       "except: from paraview.simple import *"<<endl<<
       "paraview.simple._DisableFirstRenderCameraReset()"<<endl;
    f<<"spheres = ["<<endl;

    vector<Part*>::iterator iter = this->parts.begin();
    while (iter != this->parts.end()) {
        f << "\t";
        f<<"["<<(*iter)->pos.x-delta.x<<", "<<(*iter)->pos.y-delta.y<<", "<<(*iter)->pos.z-delta.z<<", "<<(*iter)->r<<", ";
        f<<(*iter)->m.x-delta.x<<", "<<(*iter)->m.y-delta.y<<", "<<(*iter)->m.z-delta.z<<"],"<<endl;
        iter++;
    }
    f<<"\t]"<<endl;
    f<<"i=0"<<endl;
    f<<"for [x,y,z,r,ax,ay,az] in spheres:"<<endl;
    f<<"\tprint(str(i)+\" of \"+str(len(spheres)))"<<endl;
    f<<"\tSphere( Radius=r, Center=[x, y, z], ThetaResolution="<<thteta<<", PhiResolution="<<phi<<" )"<<endl;
    f<<"\tShow()"<<endl;
    f<<"\tLine( Point1=[x, y, z], Point2=[x+ax, y+ay, z+az], Resolution=1 )"<<endl;
    f<<"\tShow()"<<endl;
    f<<"\ti+=1"<<endl;
    f<<endl;
    f<<"SetActiveSource(Box( guiName=\"Box1\", XLength="<<size.x<<", YLength="<<size.y<<", ZLength="<<size.z<<
       ", Center=["<<size.x/2.-delta.x<<", "<<size.y/2.-delta.y<<", "<<size.z/2.-delta.z<<"],  ))"<<endl;
    f<<"DataRepresentation12 = Show()"<<endl;
    f<<"DataRepresentation12.EdgeColor = [0.0, 0.0, 0.5]"<<endl;
    f<<"DataRepresentation12.SelectionPointFieldDataArrayName = 'Normals'"<<endl;
    f<<"DataRepresentation12.Representation = 'Wireframe'"<<endl;
    f<<"DataRepresentation12.ScaleFactor = 0.1"<<endl;
    f<<"DataRepresentation12.CubeAxesVisibility = 1"<<endl;
    f<<"Render()"<<endl;

    f.close();
}

void PartArray::savePVPythonAnimation(PartArray* secondSystem, string file, int thteta, int phi, int frames)
{
    if (this->count()!=secondSystem->count())
        return;
    //Vect delta(size.x/2.,size.y/2.,size.z/2.);
    Vect delta(0.,0.,0.);
    std::ofstream f(file.c_str());
    f<<"try: paraview.simple"<<endl<<
       "except: from paraview.simple import *"<<endl<<
       "paraview.simple._DisableFirstRenderCameraReset()"<<endl<<endl;

    f<<"def getCue(el,param,num,oldVal,newVal):"<<endl<<
       "\tcue = GetAnimationTrack(el.GetProperty(param),num)"<<endl<<
       "\tkeyf0 = servermanager.animation.CompositeKeyFrame()"<<endl<<
       "\tkeyf0.KeyTime = 0"<<endl<<
       "\tkeyf0.KeyValues= [oldVal]"<<endl<<
       "\tkeyf1 = servermanager.animation.CompositeKeyFrame()"<<endl<<
       "\tkeyf1.KeyTime = 1.0"<<endl<<
       "\tkeyf1.KeyValues= [newVal]"<<endl<<
       "\tcue.KeyFrames = [keyf0, keyf1]"<<endl<<
       "\treturn cue"<<endl<<endl;

    vector<Part*>::iterator iter;
    f<<"scene = GetAnimationScene()"<<endl;
    f<<"scene.EndTime = 100.0"<<endl;
    f<<"scene.NumberOfFrames = "<<frames<<endl;

    f<<"spheres = ["<<endl;
    iter = this->parts.begin();
    while (iter != this->parts.end()) {
        f << "\t";
        f<<"["<<(*iter)->pos.x-delta.x<<", "<<(*iter)->pos.y-delta.y<<", "<<(*iter)->pos.z-delta.z<<", "<<(*iter)->r<<", ";
        f<<(*iter)->m.x-delta.x<<", "<<(*iter)->m.y-delta.y<<", "<<(*iter)->m.z-delta.z<<"],"<<endl;
        iter++;
    }
    f<<"\t]"<<endl;

    f<<"newSpheres = ["<<endl;
    iter = secondSystem->parts.begin();
    while (iter != secondSystem->parts.end()) {
        f << "\t";
        f<<"["<<(*iter)->pos.x-delta.x<<", "<<(*iter)->pos.y-delta.y<<", "<<(*iter)->pos.z-delta.z<<", "<<(*iter)->r<<", ";
        f<<(*iter)->m.x-delta.x<<", "<<(*iter)->m.y-delta.y<<", "<<(*iter)->m.z-delta.z<<"],"<<endl;
        iter++;
    }
    f<<"\t]"<<endl;

    f<<"i=0"<<endl;
    f<<"for [x,y,z,r,ax,ay,az] in spheres:"<<endl;
    f<<"\t[nx,ny,nz,nr,nax,nay,naz] = newSpheres[i]"<<endl;
    f<<"\tprint(str(i)+\" of \"+str(len(spheres)))"<<endl;
    f<<"\ts = Sphere( Radius=r, Center=[x, y, z], ThetaResolution="<<thteta<<", PhiResolution="<<phi<<" )"<<endl;
    f<<"\tShow()"<<endl;
    f<<"\tl = Line( Point1=[x, y, z], Point2=[x+ax, y+ay, z+az], Resolution=1 )"<<endl;



    f<<"\tp = GetDisplayProperties( l )"<<endl;
    f<<"\tp.DiffuseColor = [0.6, 0.0, 0.0]"<<endl;
    f<<"\tp.LineWidth = 10.0"<<endl;
    f<<"\tShow()"<<endl;
    f<<"\tscene.Cues.append(getCue(l,\"Point1\",0,x,nx))"<<endl;
    f<<"\tscene.Cues.append(getCue(l,\"Point1\",1,y,ny))"<<endl;
    f<<"\tscene.Cues.append(getCue(l,\"Point1\",2,z,nz))"<<endl;
    f<<"\tscene.Cues.append(getCue(l,\"Point2\",0,x+ax,nx+nax))"<<endl;
    f<<"\tscene.Cues.append(getCue(l,\"Point2\",1,y+ay,ny+nay))"<<endl;
    f<<"\tscene.Cues.append(getCue(l,\"Point2\",2,z+az,nz+naz))"<<endl;
    f<<"\tscene.Cues.append(getCue(s,\"Center\",0,x,nx))"<<endl;
    f<<"\tscene.Cues.append(getCue(s,\"Center\",1,y,ny))"<<endl;
    f<<"\tscene.Cues.append(getCue(s,\"Center\",2,z,nz))"<<endl;
    f<<"\tscene.Cues.append(getCue(s,\"Radius\",0,r,nr))"<<endl;
    f<<"\ti+=1"<<endl;
    f<<endl;

    f<<"SetActiveSource(Box( guiName=\"Box1\", XLength="<<size.x<<", YLength="<<size.y<<", ZLength="<<size.z<<
       ", Center=["<<size.x/2.-delta.x<<", "<<size.y/2.-delta.y<<", "<<size.z/2.-delta.z<<"],  ))"<<endl;
    f<<"DataRepresentation12 = Show()"<<endl;
    f<<"DataRepresentation12.EdgeColor = [0.0, 0.0, 0.5]"<<endl;
    f<<"DataRepresentation12.SelectionPointFieldDataArrayName = 'Normals'"<<endl;
    f<<"DataRepresentation12.Representation = 'Wireframe'"<<endl;
    f<<"DataRepresentation12.ScaleFactor = 0.1"<<endl;
    f<<"DataRepresentation12.CubeAxesVisibility = 1"<<endl;
    f<<"Render()"<<endl;

    f.close();
}

void PartArray::load(string file,bool showNotifications) {
    if (showNotifications)
        std::cout<<"load "<<file<<" file start"<<endl;
    std::ifstream f(file.c_str());

    this->clear(); //удаляем все частицы
    this->E1 = this->E2 = 0; //обнуляем энергии системы

    //сначала сохраняем xyz
    f >> this->size.x;
    f >> this->size.y;
    f >> this->size.z;

    int i=0;

    //пропускаем строку с заголовками
    char c[256];
    f.getline(c,256,'\n');
    f.getline(c,256,'\n');

    //затем читаем все магнитные моменты системы и положения точек
    double radius = 0;
    Part* temp;
    string shape;
    while (!f.eof()) {
        temp = new Part();
        if (!(f >> temp->pos.x).good()) break; //если не получилось считать - значит конец файла
        f >> temp->pos.y;
        f >> temp->pos.z;
        f >> temp->m.x;
        f >> temp->m.y;
        f >> temp->m.z;
        f >> temp->w1;
        f >> temp->h1;
        //f >> temp.sector; для MPI реализации, @todo потом перегрузить
        f >> temp->r;

        f >> shape;
        if (shape.compare("CIRCLE")==0)
            temp->shape = Part::CIRCLE;
        if (shape.compare("ELLIPSE")==0)
            temp->shape = Part::ELLIPSE;


        this->insert(temp);
        radius = temp->r;
        //if (i%1000==0) std::cout<<"load "<<i<<" particle"<<std::endl;
        i++;
    }

    f.close();
    if (showNotifications)
        std::cout<<"load "<<file<<" file complete"<<endl;

    config::Instance()->partR = radius;

    if (this->size.z == 0)
        config::Instance()->set2D();
    else
        config::Instance()->set3D();

}

void PartArray::clear(){
    vector<Part*>::iterator iter = this->parts.begin();
    while (iter!=this->parts.end()){
        delete (*iter); //удаляем все что по есть по ссылкам на частицы
        iter++;
    }
    this->parts.clear();
}




//временные функции, in process, deprecated and trash

//проверяем сколько частиц стоят по полю, а сколько против поля
void PartArray::checkFM(string file, double c){
    std::ofstream f(file.c_str(), ios_base::app);

    std::vector<Part*>::iterator iter1;
    const int total = this->parts.size();
    double scalar; //Number of H greather Zero; Number of H less Zero
    int i=0;
    iter1 = this->parts.begin();

    while (iter1!= this->parts.end()) {

        Vect h;
        int Nhgz = 0, Nhlz = 0, Nhez = 0;
        std::vector<Part*>::iterator iter2 = this->parts.begin();
        //считаем количество сонаправленных и обратнонаправленных частий

        while (iter2 != this->parts.end()){
            if (iter1!=iter2){
                h = (*iter1)->interact(*iter2); //считаем поле взаимодействия двух частиц
                scalar = h.scalar((*iter1)->m)/(h.length()*(*iter1)->m.length());

                if (scalar > 0.08) Nhgz++; else
                    if (scalar < -0.08) Nhlz++; else
                        Nhez++;

            }
            iter2++;
        }

        this->calcH(*iter1);

        f
                << (double)Nhgz/(double)total << "\t"
                << (double)Nhlz/(double)total << "\t"
                << (double)Nhez/(double)total << "\t"
                << (*iter1)->h.length() << "\t"
                << c <<endl;
        if (i%1000==0 || i<1000)
            std::cout<<i<<" particle"<<std::endl;
        i++;
        iter1++;
    }
    f.close();
}


void PartArray::saveEachMagnetization(string file) {
    std::ofstream f(file.c_str());

    vector<Part*>::iterator iter = this->parts.begin();
    this->calcH();
    while (iter != this->parts.end()) {
        f <<
             (*iter)->pos.x << "\t" <<
             (*iter)->pos.y << "\t" <<
             (*iter)->m.x << "\t" <<
             (*iter)->m.y << "\t" <<
             (*iter)->h.x << "\t" <<
             (*iter)->h.y << "\t" << endl;
        iter++;
    }

    f.close();
}

int PartArray::count(){
    return this->parts.size();
}

void PartArray::rotateAllUp(){
    vector<Part*>::iterator iter;
    iter = this->parts.begin();
    while (iter!=this->parts.end())
    {
        if ((*iter)->m.y<0) (*iter)->m.y *= -1.;
        iter++;
    }
}

void PartArray::rotateAllLines(double segmentSize){
    vector<Part*>::iterator iter;
    iter = this->parts.begin();
    while (iter!=this->parts.end())
    {
        if ((int)( (*iter)->pos.x / segmentSize) % 2 == 0) { //если в четном сегменте
            if ((*iter)->m.y < 0) (*iter)->m.y*= -1.; //поворачиваем вверх
        } else {
            if ((*iter)->m.y > 0) (*iter)->m.y*= -1.; //поворачиваем вниз
        }
        iter++;
    }
}

void PartArray::setMBruteLines(double segmentSize){
    vector<Part*>::iterator iter;
    iter = this->parts.begin();
    while (iter!=this->parts.end())
    {
        (*iter)->m.x = (*iter)->m.z = 0;
        if ((int)( (*iter)->pos.x / segmentSize) % 2  == 0) { //если целая часть от деления делится на 2 без остатка, то в четном сегменте
            (*iter)->m.y = config::Instance()->m; //поворачиваем вверх
        } else {
            (*iter)->m.y = -config::Instance()->m; //поворачиваем вниз
        }
        iter++;
    }
}

void PartArray::scaleSystem(double coff){
    this->size.x *= coff;
    this->size.y *= coff;
    if (!config::Instance()->dimensions()==2)
        this->size.z *= coff;

    vector<Part*>::iterator iter;
    iter = this->parts.begin();
    while (iter!=this->parts.end()){
        (*iter)->pos.x *= coff;
        (*iter)->pos.y *= coff;
        if (!config::Instance()->dimensions()==2)
            (*iter)->pos.z *= coff;
        iter++;
    }
}

void PartArray::_construct(){
    this->state = new StateMachine(this);
}

bool PartArray::_double_equals(double a, double b)
{
    return fabs(a - b) < 1e-12;
}


double PartArray::calcJ(){
    vector<Part*>::iterator iter1,iter2;
    Vect r; //радиус-вектор
    double rL;
    double Jxx,Jxy,Jyx,Jyy,J=0;

    //для 3D случая
    double Jxz,Jyz,Jzx,Jzy,Jzz;

    iter1 = this->parts.begin();
    while (iter1!=this->parts.end()){
        iter2 = iter1;
        while (iter2!=this->parts.end()){
            if (iter2!=iter1){
                r = (*iter1)->pos.radius((*iter2)->pos);
                rL = r.length();


                if (config::Instance()->dimensions()==2){
                    Jxx = ( 1.0 / (rL*rL*rL) ) *  ( (3*r.x*r.x) /  (rL * rL) - 1.0 );
                    Jyy = ( 1.0 / (rL*rL*rL) ) *  ( (3*r.y*r.y) / (rL * rL) - 1.0 );
                    Jxy = ( 3 * r.x * r.y) / ( rL * rL * rL * rL * rL );
                    Jyx = Jxy;
                    J+= ( Jxx + Jxy + Jyx + Jyy );
                } else {
                    Jxx = ( 1.0 / (rL*rL*rL) ) *  ( (3*r.x*r.x) / (rL * rL) - 1.0 );
                    Jyy = ( 1.0 / (rL*rL*rL) ) *  ( (3*r.y*r.y) / (rL * rL) - 1.0 );
                    Jzz = ( 1.0 / (rL*rL*rL) ) *  ( (3*r.z*r.z) / (rL * rL) - 1.0 );

                    Jxy = ( 3 * r.x * r.y) / ( rL * rL * rL * rL * rL );
                    Jyx = Jxy;

                    Jxz = ( 3 * r.x * r.z) / ( rL * rL * rL * rL * rL );
                    Jzx = Jxz;

                    Jzy = ( 3 * r.z * r.y) / ( rL * rL * rL * rL * rL );
                    Jyz = Jzy;

                    J+= ( Jxx + Jxy + Jxz + Jyx + Jyy + Jyz + Jzx + Jzy + Jzz);
                }

            }
            iter2++;
        }

        iter1++;
    }

    J /= this->count();
    return J;
}


double PartArray::calcJ2(){
    vector<Part*>::iterator iter1,iter2;
    Vect r; //радиус-вектор
    double rL;
    double Jxx,Jxy,Jyx,Jyy,J=0;

    //для 3D случая
    double Jxz,Jyz,Jzx,Jzy,Jzz;

    iter1 = this->parts.begin();
    while (iter1!=this->parts.end()){
        iter2 = iter1;
        while (iter2!=this->parts.end()){
            if (iter2!=iter1){
                r = (*iter1)->pos.radius((*iter2)->pos);
                rL = r.length();


                if (config::Instance()->dimensions()==2){
                    Jxx = ( 1.0 / (rL*rL*rL) ) *  ( (3*r.x*r.x) /  (rL * rL) - 1.0 );
                    Jyy = ( 1.0 / (rL*rL*rL) ) *  ( (3*r.y*r.y) / (rL * rL) - 1.0 );
                    Jxy = ( 3 * r.x * r.y) / ( rL * rL * rL * rL * rL );
                    Jyx = Jxy;
                    J+= (
                                fabs(Jxx) + fabs(Jxy) +
                                fabs(Jyx) + fabs(Jyy)
                                );
                } else {
                    Jxx = ( 1.0 / (rL*rL*rL) ) *  ( (3*r.x*r.x) / (rL * rL) - 1.0 );
                    Jyy = ( 1.0 / (rL*rL*rL) ) *  ( (3*r.y*r.y) / (rL * rL) - 1.0 );
                    Jzz = ( 1.0 / (rL*rL*rL) ) *  ( (3*r.z*r.z) / (rL * rL) - 1.0 );

                    Jxy = ( 3 * r.x * r.y) / ( rL * rL * rL * rL * rL );
                    Jyx = Jxy;

                    Jxz = ( 3 * r.x * r.z) / ( rL * rL * rL * rL * rL );
                    Jzx = Jxz;

                    Jzy = ( 3 * r.z * r.y) / ( rL * rL * rL * rL * rL );
                    Jyz = Jzy;

                    J+= (
                                fabs(Jxx) + fabs(Jxy) + fabs(Jxz) +
                                fabs(Jyx) + fabs(Jyy) + fabs(Jyz) +
                                fabs(Jzx) + fabs(Jzy) + fabs(Jzz)
                                );
                }

            }
            iter2++;
        }

        iter1++;
    }

    J /= this->count();
    return J;
}

double PartArray::calcJ12(){
    vector<Part*>::iterator iter1,iter2;
    Vect r; //радиус-вектор
    double rL;
    double Jxx,Jxy,Jyx,Jyy,J1=0,J2=0;

    //для 3D случая
    double Jxz,Jyz,Jzx,Jzy,Jzz;

    iter1 = this->parts.begin();
    while (iter1!=this->parts.end()){
        iter2 = iter1;
        while (iter2!=this->parts.end()){
            if (iter2!=iter1){
                r = (*iter1)->pos.radius((*iter2)->pos);
                rL = r.length();


                if (config::Instance()->dimensions()==2){
                    Jxx = ( 1.0 / (rL*rL*rL) ) *  ( (3*r.x*r.x) / (rL * rL) - 1.0 );
                    Jyy = ( 1.0 / (rL*rL*rL) ) *  ( (3*r.y*r.y) / (rL * rL) - 1.0 );
                    Jxy = ( 3 * r.x * r.y) / ( rL * rL * rL * rL * rL );
                    Jyx = Jxy;
                    J1+= ( Jxx + Jxy + Jyx + Jyy );
                    J2+= (
                                fabs(Jxx) + fabs(Jxy) +
                                fabs(Jyx) + fabs(Jyy)
                                );
                } else {
                    Jxx = ( 1.0 / (rL*rL*rL) ) *  ( (3*r.x*r.x) / (rL * rL) - 1.0 );
                    Jyy = ( 1.0 / (rL*rL*rL) ) *  ( (3*r.y*r.y) / (rL * rL) - 1.0 );
                    Jzz = ( 1.0 / (rL*rL*rL) ) *  ( (3*r.z*r.z) / (rL * rL) - 1.0 );

                    Jxy = ( 3 * r.x * r.y) / ( rL * rL * rL * rL * rL );
                    Jyx = Jxy;

                    Jxz = ( 3 * r.x * r.z) / ( rL * rL * rL * rL * rL );
                    Jzx = Jxz;

                    Jzy = ( 3 * r.z * r.y) / ( rL * rL * rL * rL * rL );
                    Jyz = Jzy;

                    J1+= ( Jxx + Jxy + Jxz + Jyx + Jyy + Jyz + Jzx + Jzy + Jzz);
                    J2+= (
                                fabs(Jxx) + fabs(Jxy) + fabs(Jxz) +
                                fabs(Jyx) + fabs(Jyy) + fabs(Jyz) +
                                fabs(Jzx) + fabs(Jzy) + fabs(Jzz)
                                );
                }

            }
            iter2++;
        }

        iter1++;
    }

    //J1 *= (1./this->count());
    //J2 *= (1./this->count());
    return J1/J2;
}

bool PartArray::setToGroundState(){
    StateMachineFree minstate;
    this->state->reset(); //сбрасываем в начальное состояние, так как полный перебор предполагает все состояния

    double initE = this->calcEnergy1FastIncrementalFirst();

    double eMin, eTemp;

    bool first = true;
    do {
        eTemp = this->calcEnergy1FastIncremental(initE);
        if (first){
            eMin = eTemp;
            minstate = *this->state;
            first = false;
        }
        else
            if (eTemp < eMin) {
                eMin = eTemp;
                minstate = *this->state;
            }
    } while (this->state->halfNext());

    *(this->state) = minstate;

    return true;
}

bool PartArray::setToMaximalState(){
    StateMachineFree maxstate;
    this->state->reset(); //сбрасываем в начальное состояние, так как полный перебор предполагает все состояния

    double initE = this->calcEnergy1FastIncrementalFirst();

    double eMax, eTemp;

    bool first = true;
    do {
        eTemp = this->calcEnergy1FastIncremental(initE);
        if (first){
            eMax = eTemp;
            maxstate = *this->state;
            first = false;
        }
        else
            if (eMax < eTemp) {
                eMax = eTemp;
                maxstate = *this->state;
            }
    } while (this->state->halfNext());

    *(this->state) = maxstate;

    return true;
}


bool PartArray::setToMonteCarloGroundState(const double t, int steps){
    int tryingCount=0; //количество попыток переворота

    do {
        if (this->processMonteCarloStep(t))
            tryingCount = 0;
        else
            tryingCount++;
    } while (tryingCount<steps);
    return true;
}

bool PartArray::processMonteCarloStep(const double t){
    const double Eold = this->calcEnergy1();
    const int num = this->state->randomize();

    //если переворот привел к увеличению энергии, либо повлияла температура, возвращаем всё обратно
    const double
            de = Eold-this->calcEnergy1(),
            p = exp(de/t), //вероятность оставления системы
            randNum = (double)config::Instance()->rand()/(double)config::Instance()->rand_max; //случайное число от 0 до 1
    //если de>0 значит, оставляем переворот (новое E меньше старого), или
    //если выпавшее число меньше вероятности, тоже оставляем
    if (de>0 || (t>0 && randNum <= p)){
        return true;
    } else {
        this->parts[num]->rotate();
        this->E1 = Eold;
        return false;
    }
}

bool PartArray::setToMonteCarloGroundState2(){
    //if (this->count() > 32)
    //return false;


    int num,rotatedCount,tryingCount=0;
    const int count = this->count(), maxTryigCount = count*count*count;
    double Eold,Enew;
    vector<int> rot(count);
    do {
        Eold = this->calcEnergy1();
        rotatedCount = ( config::Instance()->rand()%(count-2) ) + 1; //решаем сколько частиц подворачивать
        for (int i=0;i<rotatedCount;i++){
            num = config::Instance()->rand()%count; //решаем какую частицу подворачивать
            this->parts.at(num)->rotate();
            rot[i] = num; //запоминаем подвернутую частицу
        }
        Enew = this->calcEnergy1();
        //если переворот привел к увеличению энергии, возвращаем всё обратно
        if (Eold<=Enew){
            for (int i=0;i<rotatedCount;i++){
                num = rot[i]; //решаем какую частицу подворачивать
                this->parts.at(num)->rotate();
            }
            tryingCount++;
        } else tryingCount=0;
    } while (tryingCount<maxTryigCount);


    return true;
}

bool PartArray::setToPTGroundState(int replicas, int totalSteps, double tMin, double tMax){
    //создали набор-шаблон частиц
    vector<PartArray*> systems;

    //создали набор температур
    vector <double> t;

    PartArray *temp;
    double tempInterval = 0;

    if (replicas>1)
        tempInterval = (tMax-tMin)/double(replicas-1);

    //плодим системы - копии оригинала и раздаём температуры
    for (int i=0;i<replicas;i++){
        systems.push_back(this->copy());

        //раздаём температуры
        t.push_back(tMin+tempInterval*i);
    }


    //magic
    bool mcResult, //результат работы монте-карло шага (применены ли изменения или нет)
            solved=false; //флаг, решено ли уравнение
    int i,
            eRepeatNum=0, //количество повторений энергии
            steps=0; //количество шагов

    //данные о минимуме
    StateMachineFree minState;
    double eMin=9999;


    vector<PartArray*>::iterator iter, iter2;

    do{
        //монтекарлим 1 раз все системы
        iter=systems.begin(); i=0;
        while (iter!=systems.end()){
            temp = *iter;
            mcResult = temp->processMonteCarloStep(t[i]);
            if (i==0){
                if (!mcResult)
                    eRepeatNum++;
                else
                    eRepeatNum = 0;
            }
            iter++; i++;
        }



        //производим обмен системы, если выпал случай
        iter=iter2=systems.begin(); i=0;
        iter2++;
        while (iter2!=systems.end()){
            double randNum = (double)config::Instance()->rand()/(double)config::Instance()->rand_max;
            double de = ((*iter)->E1) - ((*iter2)->E1);
            double p = std::min(1.,exp(de/(0.75)));

            if (randNum<p){
                temp = (*iter);
                (*iter) = (*iter2);
                (*iter2) = temp;

                eRepeatNum = 0;
            }
            iter++; iter2++; i++;
        }

        //случай когда найден минимум
        if (systems[0]->E1<=eMin){
            minState = *(systems[0]->state);
            eMin = systems[0]->E1;
            //std::cout << "E=" << minSystem->E1 <<endl;
        }

        steps++;
        if (steps>=totalSteps){
            //минимум достигнут
            solved = true;
        }

    } while(!solved);



    //под конец чистим память
    iter=systems.begin();
    while (iter!=systems.end()){
        temp = *iter;
        delete temp;
        iter++;
    }
    systems.clear();
    t.clear();

    *this->state = minState;

    return true;
}



double PartArray::calcEnergy1FastIncrementalTemp(unsigned long long int state){
    unsigned long long int power = 1; //степень двойки для выуживания битов из состояния


    //рассчитываем энергию
    power = 1;
    double E=this->eIncrementalTemp;
    vector<Part*>::iterator iter;

    iter = this->parts.begin();
    while(iter!= this->parts.end()){
        if ( (power & state) != 0){
            for (int i=0;i<this->count();i++){
                E -=  ( 2. * (*iter)->eArray.at(i));
            }
        }
        power<<=1;
        iter++;
    }

    return E;
}

void PartArray::dropAdaptive(int count){
    //double surfVol = this->size.x * this->size.y * this->size.z; //считаем объем (площадь) поверхности, в которую кидаем частицы
    Part* temp; //временная частица
    int partCount = this->parts.size(); //количество сброшеных частиц

    std::vector < Part* >::iterator iterator1; // итератор для обхода массива частиц
    bool regenerate; //Флаг, нужен для проверки перекрещивания частиц
    while (partCount < count) {
        //std::cout <<"Drop "<<partCount<<endl;
        temp = new Part();
        do {
            regenerate = false;
            //генерим координаты
            temp->pos.x = config::Instance()->rand() % ((int)(this->size.x-config::Instance()->partR*2)*100);
            temp->pos.y = config::Instance()->rand() % ((int)(this->size.y-config::Instance()->partR*2)*100);
            if (config::Instance()->dimensions()==2) //если работаем в плоскости, то генерить третью координату не надо
                temp->pos.z = 0;
            else
                temp->pos.z = config::Instance()->rand() % ((int)(this->size.z-config::Instance()->partR*2)*100);

            temp->pos.x = temp->pos.x / 100 + config::Instance()->partR;
            temp->pos.y = temp->pos.y / 100 + config::Instance()->partR;
            temp->pos.z = temp->pos.z / 100 + config::Instance()->partR;

            //проверяем чтобы сгенная точка не пересекалась ни с какой другой (это значит что площади их сфер не пересекались)
            iterator1 = this->parts.begin();
            while (iterator1 != this->parts.end()) {
                if (temp->pos.radius((*iterator1)->pos).length()<=config::Instance()->partR*2){
                    regenerate = true;
                    //std::cout<<"Drop "<<partCount<<" particle error, repeat"<<endl;
                    break;
                }
                ++iterator1;
            }
        } while (regenerate);

        //генерируем вектор частицы
        double longitude = ((double)config::Instance()->rand()/(double)config::Instance()->rand_max) * 2. * M_PI;
        double lattitude;
        if (config::Instance()->dimensions()==2) lattitude=0; else lattitude=(double)config::Instance()->rand() / (double)config::Instance()->rand_max * 2. - 1.; // если частица 2-х мерная то угол отклонения должен быть 0

        if (partCount>0){
            this->calcH(temp);
            temp->m = temp->h;
            temp->m.setUnitary();
        } else {
            temp->m.x = config::Instance()->m * cos(longitude)*sqrt(1-lattitude*lattitude);
            temp->m.y = config::Instance()->m * sin(longitude)*sqrt(1-lattitude*lattitude);
            temp->m.z = config::Instance()->m * lattitude;
        }

        //добавляем частицу на экземпляр
        this->insert(temp);
        partCount++;
    }
}
void PartArray::turnUp(){
    this->turnToDirection(new Vect(0,1,0));
}

void PartArray::turnRight(){
    this->turnToDirection(new Vect(1,0,0));
}

void PartArray::turnToDirection(Vect *v){
    vector<Part*>::iterator iter = this->parts.begin();
    iter = this->parts.begin();
    while(iter!=this->parts.end()){
        if ((*iter)->m.scalar(*v)<0){
            (*iter)->m.rotate();
        }
        iter++;
    }
}

void PartArray::movePosRandomly(double d){
    Vect dir; //направление, в которое двигать частицу.
    for(int i=0;i<this->count();i++){
        double longitude = ((double)config::Instance()->rand()/(double)config::Instance()->rand_max) * 2. * M_PI;
        double lattitude=0;
        switch (config::Instance()->dimensions()) {
        case 2:
            lattitude=0; // если частица 2-х мерная то угол отклонения должен быть 0
            break;
        case 3:
            lattitude=(double)config::Instance()->rand()/(double)config::Instance()->rand_max*2.-1.;
            break;
        }
        dir.x = d * cos(longitude) * sqrt(1-lattitude*lattitude);
        dir.y = d * sin(longitude) * sqrt(1-lattitude*lattitude);
        dir.z = 0 * lattitude;
        this->parts[i]->pos += dir;
    }
}

void PartArray::moveMRandomly(double fi){
    if (config::Instance()->dimensions()==2){
        double side = 1.;
        double oldFi;

        for(int i=0;i<this->count();i++){
            double oldLen = this->parts[i]->m.length();
            if ((double)config::Instance()->rand()/(double)config::Instance()->rand_max>0.5)
                side = -1.;
            else
                side = 1.;
            oldFi = this->parts[i]->m.angle();
            double longitude = oldFi+(fi*side);
            this->parts[i]->m.x = oldLen * cos(longitude);
            this->parts[i]->m.y = oldLen * sin(longitude);
            this->parts[i]->m.z = 0;
        }
    } else {
        for(int i=0;i<this->count();i++){
            Part* temp = this->parts[i];
            Vect ox,oy,oz,newV;
            //1. нормализуем вектор частицы, считаем его длину
            oz = temp->m.normalize();

            //2. генерируем ортонормированный базис, где oz-магнитный момент частицы
            oy = Vect::crossProduct(oz,Vect(0.5,0,0));
            ox = Vect::normal(oy,oz);
            oy = Vect::normal(ox,oz);

            //3. генерируем направление сдвига вектора
            double longitude = ((double)config::Instance()->rand()/(double)config::Instance()->rand_max) * 2. * M_PI; //[0;2pi]

            //4. получаем положение вдоль оси z
            //double lattitude=(double)config::Instance()->rand()/(double)config::Instance()->rand_max*(1-cos(fi))+cos(fi); //[cos(fi);1]
            double lattitude=cos(fi); //задано параметром функции, cos(fi)

            //5. получаем сдвинутый вектор
            newV = oz*lattitude + (ox*cos(longitude) + oy*sin(longitude)) * sqrt(1-lattitude*lattitude);
            newV *= temp->m.length();
            temp->m = newV;
        }
    }
}
