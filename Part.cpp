/*
 * File:   Part.cpp
 * Author: uyras
 *
 * Created on 20 Ноябрь 2012 г., 15:50
 */

#include <vector>
#include "Part.h"
#include "config.h"

Part::Part() :
    state(false)
{
    //по умолчанию на частицу ничего не действует
    this->h.x = this->h.y = this->h.z = 0;
    sector = 0; //по умолчанию, все частицы из сектора 0
    this->r = config::Instance()->partR; //по умолчанию радиус задается в конфигах
    this->e = 0;
    this->w1 = this->h1 = 0;
}
Part::~Part(){
    this->eArray.clear();
}

Vect Part::interact(Part* elem){
    Vect h;
    double r, r2, r5, part, dx, dy, dz;

    Vect rvect = elem->pos.radius(this->pos);
    r = rvect.length();
    r2 = r * r; //радиус в квадрате
    r5 = r2 * r * r * r; //радиус в пятой
    dx = this->pos.x - elem->pos.x;
    dy = this->pos.y - elem->pos.y;
    dz = this->pos.z - elem->pos.z;
    part = elem->m.x * dx + elem->m.y * dy + elem->m.z * dz;
    //степени отличаются от формулы потому что обе дроби внесены под общий знаменатель
    h.x = ((3 * part * dx) - elem->m.x * r2) / r5;
    h.y = ((3 * part * dy) - elem->m.y * r2) / r5;
    h.z = ((3 * part * dz) - elem->m.z * r2) / r5;

    //h.absK = config::Instance()->ergGauss/pow(config::Instance()->santiMeter,3.);

    return h;
}

void Part::rotate(float angle){
    angle = angle;
    this->m.rotate();
    this->state = !this->state;
}

Part* Part::copy(){
    Part* temp = new Part();

    temp->pos = this->pos;
    temp->m = this->m;
    temp->h = this->h;
    temp->e = this->e;
    temp->eArray=this->eArray;
    temp->_vol = this->_vol;
    temp->sector = this->sector;
    temp->state = this->state;

    return temp;
}

double Part::volume()
{
    if (config::Instance()->dimensions()==2){
        return M_PI * this->r * this->r;
    } else {
        return (4.0/3.0) * M_PI * this->r * this->r * this->r;
    }
}
