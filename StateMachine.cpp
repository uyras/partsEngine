#include "StateMachine.h"
#include "StateMachineFree.h"
#include "PartArray.h"

StateMachine::StateMachine():_system(0)
{
}

StateMachine::StateMachine(PartArray *system)
{
    this->connect(system);
}

StateMachine::StateMachine(PartArray &system)
{
    this->connect(&system);
}

StateMachine& StateMachine::operator=(const StateMachineFree &state)
{
    long unsigned int s;
    if (this->size() < state.size())
        s = this->size();
    else
        s=state.size();

    for (unsigned int i=0;i<s;i++){
        if (this->operator [](i) != state[i]){
            this->_system->parts[i]->rotate();
        }
    }
    _system->changeState();
    return *this;
}

StateMachine &StateMachine::operator=(const StateMachine &state)
{
    long unsigned int s;
    if (this->size() < state.size())
        s = this->size();
    else
        s=state.size();

    for (unsigned int i=0;i<s;i++){
        if (this->operator[](i) != state[i]){
            this->_system->parts[i]->rotate();
        }
    }
    _system->changeState();
    return *this;
}

void StateMachine::connect(PartArray *system)
{
    this->_system=system;
    //delete system->state;
    //system->state = this;
}

void StateMachine::disconnect()
{
    this->_system = 0;
}

bool StateMachine::connected() const
{
    return !(this->_system==0);
}

void StateMachine::reset(){
    std::vector<Part*>::iterator iter;
    iter = this->_system->parts.begin();
    while(iter!=this->_system->parts.end()){
        if ((*iter)->state==1){
            (*iter)->rotate();
        }
        iter++;
    }
    _system->changeState();
}

void StateMachine::hardReset(){
    std::vector<Part*>::iterator iter;
    iter = this->_system->parts.begin();
    while(iter!=this->_system->parts.end()){
        (*iter)->state=0;
        iter++;
    }
    this->_system->changeSystem();
}

void StateMachine::setLast(){
    std::vector<Part*>::iterator iter;
    iter = this->_system->parts.begin();
    while(iter!=this->_system->parts.end()){
        if ((*iter)->state==0){
            (*iter)->rotate();
        }
        iter++;
    }

    _system->changeState();
}

int StateMachine::randomize(int count){
    int randnum=0,
            rm = Random::Instance()->max(),
            parts = this->_system->parts.size(),
            rotated = 0;

    _system->changeState();
    Part* temp;
    while (rotated<count){
        randnum = config::Instance()->rand();
        randnum = floor(double(randnum)/double(rm)*double(parts-1));
        temp = (this->_system->parts.at(randnum));

        //if (!temp->state){
            temp->rotate();
            rotated++;
        //}
    }

    if (count==1)
        return randnum;
    else
        return count;
}

bool StateMachine::isFirst(){
    vector<Part*>::iterator iter = this->_system->parts.begin();
    while (iter!=this->_system->parts.end()){
        if ((*iter)->state==true)
            return false;
        iter++;
    }
    return true;
}

bool StateMachine::isLast(){
    vector<Part*>::iterator iter = this->_system->parts.begin();
    while (iter!=this->_system->parts.end()){
        if ((*iter)->state==false)
            return false;
        iter++;
    }
    return true;
}

bool StateMachine::isHalfLast(){
    vector<Part*>::iterator iter = this->_system->parts.begin();
    while (iter!=this->_system->parts.end()-1){
        if ((*iter)->state==false)
            return false;
        iter++;
    }
    if ((*iter)->state==true)
        return false;

    return true;
}

bool StateMachine::next(){

    //версия для типа bool
    std::vector<Part*>::iterator iter;

    _system->changeState();

    iter = this->_system->parts.begin();

    while (iter!=(this->_system->parts.end())){
        (*iter)->rotate();
        if ((*iter)->state==1){
            return true;
            break;
        }
        iter++;
    }

    return false;
}

bool StateMachine::halfNext(){

    _system->changeState();

    //версия для типа bool
    std::vector<Part*>::iterator iter;

    iter = this->_system->parts.begin();

    while (iter!=(this->_system->parts.end()-1)){// -1 тут чтобы перебирать только половину состояний
        (*iter)->rotate();
        if ((*iter)->state==1){
            return true;
            break;
        }
        iter++;
    }

    return false;
}

bool StateMachine::prev(){

    _system->changeState();

    //версия для типа bool
    std::vector<Part*>::iterator iter;

    iter = this->_system->parts.begin();

    while (iter!=(this->_system->parts.end())){
        (*iter)->rotate();
        if ((*iter)->state==0){
            return true;
            break;
        }
        iter++;
    }

    return false;
}

bool StateMachine::halfPrev(){

    _system->changeState();

    //версия для типа bool
    std::vector<Part*>::iterator iter;

    iter = this->_system->parts.begin();

    while (iter!=(this->_system->parts.end()-1)){// -1 тут чтобы перебирать только половину состояний
        (*iter)->rotate();
        if ((*iter)->state==0){
            return true;
            break;
        }
        iter++;
    }

    return false;
}

bool StateMachine::operator++(){
    return this->next();
}

bool StateMachine::operator--(){
    return this->prev();
}

bool StateMachine::operator++(int){
    return this->next();
}

bool StateMachine::operator--(int){
    return this->prev();
}

StateMachine &StateMachine::operator+=(const unsigned long int  val)
{
    for (unsigned int i=0;i<val;i++){
        this->next();
    }
    return *this;
}

std::string StateMachine::toString() const{
    std::vector<Part*>::const_iterator iter;
    std::string s="";
    iter = this->_system->parts.begin();
    while (iter!=this->_system->parts.end()){
        if ((*iter)->state==0){
            s+="0";
        } else {
            s+="1";
        }
        iter++;
    }
    return s;
}

bool StateMachine::fromString(const std::string & s)
{
    if (this->_system->parts.size()<s.size())
        return false;

    _system->changeState();

    for (unsigned int i=0;i<s.size();i++){
        if (this->_system->parts[i]->state != (s[i]!='0'))
            this->_system->parts[i]->rotate();
    }
    return true;
}

bool StateMachine::operator[](const unsigned long int num) const{
    return this->_system->parts[num]->state;
}

unsigned long StateMachine::size() const{
    if (this->connected())
        return this->_system->parts.size();
    else
        return 0;
}

bool StateMachine::operator==(const StateMachine &one) const
{
    //если системы разных размеров, априори считаем их разными
    if (one._system->parts.size()!=this->_system->parts.size()) return false;

    //если система из 0 иди 1 частицы, считать их одинаковыми
    if (
            one._system->parts.size()==this->_system->parts.size() &&
            (one._system->parts.size()==0 || one._system->parts.size()==1)
            ) return true;

    vector<Part*>::const_iterator iter1 = one._system->parts.begin();
    vector<Part*>::iterator iter2 = this->_system->parts.begin();

    //вычисляем, прямое или обратное совпадение считать
    //то есть системы могут быть равными или обратными
    bool compEquals = (*iter2)->state == (*iter1)->state;

    while(iter2 != this->_system->parts.end()){
        if ( ((*iter2)->state == (*iter1)->state) != compEquals)
            return false;
        iter1++; iter2++;
    }

    return true;
}
