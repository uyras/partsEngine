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
    iter = this->_state.end();
    while (iter!=this->_state.begin()){
        iter--;
        if ((*iter)->state==0){
            std::cout<<"0";
        } else {
            std::cout<<"1";
        }
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

std::vector<Part*>::iterator StateMachine::begin(){
    return this->_state.begin();
}

std::vector<Part*>::iterator StateMachine::end(){
    return this->_state.end();
}

int StateMachine::randomize(int count){
    int randnum, parts = this->_state.size();
    for (int i=0;i<count;i++){
        randnum = config::Instance()->rand()%parts;
        (*(this->_state[randnum])).rotate();
    }
    if (count==1)
        return randnum;
    else
        return -1;
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

    vector<bool>::const_iterator iter1 = one._state.begin();
    vector<Part*>::iterator iter2 = this->_state.begin();
    while(iter2!=this->_state.end()){
        if ((*iter2)->state != *iter1)
            return false;
        iter1++; iter2++;
    }

    return true;
}

bool StateMachine::operator==(const StateMachine &one)
{
    if (one._state.size()!=this->_state.size()) return false;

    vector<Part*>::const_iterator iter1 = one._state.begin();
    vector<Part*>::iterator iter2 = this->_state.begin();
    while(iter2!=this->_state.end()){
        if ((*iter2)->state != (*iter1)->state)
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
