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

PartArray::PartArray(char* file) {
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
    if (!config::Instance()->U2D) surfVol *= this->size.z; //считаем объем (площадь) поверхности, в которую кидаем частицы
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
            if (config::Instance()->U2D) //если работаем в плоскости, то генерить третью координату не надо
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
        if (config::Instance()->U2D)
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
    double surfVol = this->size.x * this->size.y * this->size.z; //считаем объем (площадь) поверхности, в которую кидаем частицы
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
            if (config::Instance()->U2D) //если работаем в плоскости, то генерить третью координату не надо
                temp->pos.z = 0;
            else
                temp->pos.z = config::Instance()->rand() % ((int)(this->size.z-config::Instance()->partR*2)*100);

            temp->pos.x = temp->pos.x / 100 + config::Instance()->partR;
            temp->pos.y = temp->pos.y / 100 + config::Instance()->partR;
            if (config::Instance()->U2D)
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
        if (config::Instance()->U2D) lattitude=0; else lattitude=(double)config::Instance()->rand() / (double)config::Instance()->rand_max * 2. - 1.; // если частица 2-х мерная то угол отклонения должен быть 0

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
    if (distance==-1)
        distance = 2.*rad;
    double x = rad;
    short int up = 1;
    Part* temp;
    while (x < this->size.x-rad) {
        double y = rad;
        while (y < this->size.y-rad){
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
    this->state->_state.push_back(part);
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
            if (config::Instance()->U2D) lattitude=0; else lattitude=(double)config::Instance()->rand()/(double)config::Instance()->rand_max*2-1; // если частица 2-х мерная то угол отклонения должен быть 0

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
    if (!config::Instance()->U2D) surfVol *= this->size.z; //считаем объем (площадь) поверхности, в которую кидаем частицы
    if (simple)
        return (config::Instance()->vol * this->parts.size()) / surfVol;
    else{
        double destiny=0;
        std::vector < Part* >::iterator iterator1 = this->parts.begin();
        while (iterator1!=this->parts.end()){
            destiny+=(*iterator1)->volume/surfVol;
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
        if (!config::Instance()->U2D)
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
        if (!config::Instance()->U2D)
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

        if (!config::Instance()->U2D) {
            temp1.z += (*iter)->m.z;
            temp2.z += fabs((*iter)->m.z);
        }

        iter++;
    }
    temp1.x /= temp2.x;
    temp1.y /= temp2.y;
    if (!config::Instance()->U2D)
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
    iter = state->begin();
    int i=0;
    while(iter != state->end()){
        if ( (*iter)->state == 0){
            zeros.push_back(i);
        }
        iter++; i++;
    }


    //рассчитываем энергию
    double E=initEnergy;
    std::vector<int>::iterator iter2;
    iter = state->begin();
    while(iter != state->end()){
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
    double r, r2, r5, rijx, rijy, E;

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
                r2 = rijx*rijx+rijy*rijy;
                r = sqrt(r2); //трудное место, заменить бы
                r5 = r2 * r2 * r; //радиус в пятой
                E = //энергия считается векторным методом, так как она не нужна для каждой оси
                        (( ((*iterator1)->m.x*(*iterator2)->m.x+(*iterator1)->m.y*(*iterator2)->m.y) * r2)
                         -
                         (3 * ((*iterator2)->m.x * rijx + (*iterator2)->m.y * rijy) * ((*iterator1)->m.x * rijx + (*iterator1)->m.y * rijy)  )) / r5;

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
                << (*iterator1)->m.z << "\t\t"
                << (*iterator1)->h.x << "\t"
                << (*iterator1)->h.y << "\t"
                << (*iterator1)->h.z << "\t";
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

void PartArray::save(char* file, bool showNotifications) {

    if (showNotifications)
        std::cout<<"save "<<file<<" file start"<<endl;
    std::ofstream f(file);

    //сначала сохраняем xyz
    f << this->size.x << endl;
    f << this->size.y << endl;
    f << this->size.z << endl;

    //строка заголовков
    f << "x\ty\tz\tMx\tMy\tMz\tr" << endl;

    //затем все магнитные моменты системы и положения точек
    vector<Part*>::iterator iter = this->parts.begin();
    while (iter != this->parts.end()) {
        f << (*iter)->pos.x << "\t";// << endl;
        f << (*iter)->pos.y << "\t";// << endl;
        f << (*iter)->pos.z << "\t";// << endl;
        f << (*iter)->m.x << "\t";// << endl;
        f << (*iter)->m.y << "\t";// << endl;
        f << (*iter)->m.z << "\t";// << endl;
        //f << (*iter)->sector << "\t";// << endl;
        f << (*iter)->volume << endl;
        iter++;
    }
    f.close();

    if (showNotifications)
        std::cout<<"save "<<file<<" file complete"<<endl;

}

void PartArray::load(char* file,bool showNotifications) {
    if (showNotifications)
        std::cout<<"load "<<file<<" file start"<<endl;
    std::ifstream f(file);

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
    while (!f.eof()) {
        temp = new Part();
        if (!(f >> temp->pos.x).good()) break; //если не получилось считать - значит конец файла
        f >> temp->pos.y;
        f >> temp->pos.z;
        f >> temp->m.x;
        f >> temp->m.y;
        f >> temp->m.z;
        //f >> temp.sector; для MPI реализации, @todo потом перегрузить
        f >> temp->volume;
        this->insert(temp);
        radius = temp->volume;
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
    this->state->_state.clear();
    this->parts.clear();
}




//временные функции, in process, deprecated and trash

//проверяем сколько частиц стоят по полю, а сколько против поля
void PartArray::checkFM(char* file, double c){
    std::ofstream f(file, ios_base::app);

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


void PartArray::saveEachMagnetization(char* file) {
    std::ofstream f(file);

    vector<Part*>::iterator iter = this->parts.begin();
    vector<Part*>::iterator iter2 = this->parts.begin();
    Vect tempv;
    int i=1;
    while (iter != this->parts.end()) {
        f <<
             (*iter)->pos.x << "\t" <<
             (*iter)->pos.y << "\t" <<
             (*iter)->m.x << "\t" <<
             (*iter)->m.y << "\t";

        iter2 = this->parts.begin();
        while (iter2!=this->parts.end()){
            if (iter!=iter2){
                tempv = (*iter)->interact(*iter2);
                f << tempv.x << "\t" << tempv.y << "\t";
            } else
                f << 0 << "\t" << 0 << "\t";
            iter2++;
        }
        f<<i << endl;
        iter++;
        i++;
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
    if (!config::Instance()->U2D)
        this->size.z *= coff;

    vector<Part*>::iterator iter;
    iter = this->parts.begin();
    while (iter!=this->parts.end()){
        (*iter)->pos.x *= coff;
        (*iter)->pos.y *= coff;
        if (!config::Instance()->U2D)
            (*iter)->pos.z *= coff;
        iter++;
    }
}

void PartArray::_construct(){
    this->state = new StateMachine(this);
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


                if (config::Instance()->U2D){
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


                if (config::Instance()->U2D){
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


                if (config::Instance()->U2D){
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
    } while (this->state->next());

    *(this->state) = minstate;

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
    double surfVol = this->size.x * this->size.y * this->size.z; //считаем объем (площадь) поверхности, в которую кидаем частицы
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
            if (config::Instance()->U2D) //если работаем в плоскости, то генерить третью координату не надо
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
        if (config::Instance()->U2D) lattitude=0; else lattitude=(double)config::Instance()->rand() / (double)config::Instance()->rand_max * 2. - 1.; // если частица 2-х мерная то угол отклонения должен быть 0

        if (partCount>0){
            this->calcH(temp);
            temp->m = temp->h;
            temp->m.setUnit();
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
