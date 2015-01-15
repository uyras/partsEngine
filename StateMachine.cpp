#include "StateMachine.h"

StateMachine::StateMachine(PartArray* system){
    this->setSystem(system);
}

bool StateMachine::operator++(){
    return this->next();
}

bool StateMachine::next(){

    //версия для типа bool
    std::vector<Part*>::iterator iter;

    iter = this->_state.begin();

    while (iter!=(this->_state.end())){
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

    //версия для типа bool
    std::vector<Part*>::iterator iter;

    iter = this->_state.begin();

    while (iter!=(this->_state.end()-1)){// -1 тут чтобы перебирать только половину состояний
        (*iter)->rotate();
        if ((*iter)->state==1){
            return true;
            break;
        }
        iter++;
    }

    return false;
}

void StateMachine::setSystem(PartArray* system){
    std::vector<Part*>::iterator iter = system->parts.begin();

    while (iter!=system->parts.end()){
        this->_state.push_back(*iter);
        iter++;
    }
}

void StateMachine::draw(){
    std::vector<Part*>::iterator iter;
    iter = this->_state.begin();
    while (iter!=this->_state.end()){
        if ((*iter)->state==0){
            std::cout<<"0";
        } else {
            std::cout<<"1";
        }
        iter++;
    }
    std::cout<<std::endl;
}

void StateMachine::reset(){
    std::vector<Part*>::iterator iter;
    iter = this->_state.begin();
    while(iter!=this->_state.end()){
        if ((*iter)->state==1){
            (*iter)->rotate();
        }
        iter++;
    }
}

void StateMachine::hardReset(){
    std::vector<Part*>::iterator iter;
    iter = this->_state.begin();
    while(iter!=this->_state.end()){
        (*iter)->state=0;
        iter++;
    }
}

std::vector<Part*>::iterator StateMachine::begin(){
    return this->_state.begin();
}

std::vector<Part*>::iterator StateMachine::end(){
    return this->_state.end();
}

int StateMachine::randomize(int count){
    int randnum,
            rm = config::Instance()->rand_max,
            parts = this->_state.size(),
            rotated = 0;

    if (count>parts)
        return -1;

    Part* temp;
    while (rotated<count){
        randnum = config::Instance()->rand();
        randnum = floor(double(randnum)/double(rm)*double(parts-1));
        temp = (this->_state.at(randnum));

        if (!temp->state){
            temp->rotate();
            rotated++;
        }
    }

    if (count==1)
        return randnum;
    else
        return count;
}

StateMachine & StateMachine::operator= (const StateMachineFree & one){
    if (one._state.size()>=this->_state.size()) {
        vector<bool>::const_iterator iter1 = one._state.begin();
        vector<Part*>::iterator iter2 = this->_state.begin();
        while(iter2!=this->_state.end()){
            if ((*iter2)->state != *iter1)
                (*iter2)->rotate();
            iter1++; iter2++;
        }
    }


    return *this;
}

bool StateMachine::operator==(const StateMachineFree &one)
{
    if (one._state.size()!=this->_state.size()) return false;
    if (one._state.size()==this->_state.size() && (one._state.size()==0 || one._state.size()==1)) return true;

    vector<bool>::const_iterator iter1 = one._state.begin();
    vector<Part*>::iterator iter2 = this->_state.begin();

    //вычисляем, прямое или обратное совпадение считать
    bool compEquals=true;
    if ((*iter2)->state != (*iter1))
        compEquals = false;

    while(iter2!=this->_state.end()){
        if ( ((*iter2)->state == *iter1) != compEquals)
            return false;
        iter1++; iter2++;
    }

    return true;
}

bool StateMachine::operator==(const StateMachine &one)
{
    if (one._state.size()!=this->_state.size()) return false;
    if (one._state.size()==this->_state.size() && (one._state.size()==0 || one._state.size()==1)) return true;

    vector<Part*>::const_iterator iter1 = one._state.begin();
    vector<Part*>::iterator iter2 = this->_state.begin();

    //вычисляем, прямое или обратное совпадение считать
    bool compEquals=true;
    if ((*iter2)->state != (*iter1)->state)
        compEquals = false;

    while(iter2!=this->_state.end()){
        if ( ((*iter2)->state == (*iter1)->state) != compEquals)
            return false;
        iter1++; iter2++;
    }

    return true;
}

bool StateMachine::isInitial(){
    vector<Part*>::iterator iter = this->_state.begin();
    while (iter!=this->_state.end()){
        if ((*iter)->state==true)
            return false;
        iter++;
    }
    return true;
}
