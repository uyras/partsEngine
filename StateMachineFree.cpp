#include "StateMachineFree.h"

StateMachineFree::StateMachineFree()
{
}

StateMachineFree::StateMachineFree(StateMachine* state){
    *this = *state;
}

void StateMachineFree::draw(){
    std::vector<bool>::iterator iter;
    iter = this->_state.begin();
    while (iter!=this->_state.end()){
        if (*iter==0){
            std::cout<<"0";
        } else {
            std::cout<<"1";
        }
        iter++;
    }
    std::cout<<std::endl;
}

bool StateMachineFree::next(){
    //версия для типа bool
    std::vector<bool>::iterator iter;
    iter = this->_state.begin();

    while (iter!=(this->_state.end()-1)){
        if (*iter==0){
            *iter=1;
            return true;
            break;
        } else {
            *iter=0;
        }
        iter++;
    }

    return false;
}

StateMachineFree & StateMachineFree::operator= (const StateMachine & one){
    this->_state.clear();

    vector<Part*>::const_iterator iter = one._state.begin();
    while(iter!=one._state.end()){
        this->_state.push_back((*iter)->state);
        iter++;
    }

    return *this;
}

bool StateMachineFree::operator==(const StateMachineFree &one)
{
    if (one._state.size()!=this->_state.size()) return false;

    vector<bool>::const_iterator iter1 = one._state.begin();
    vector<bool>::iterator iter2 = this->_state.begin();
    while(iter2!=this->_state.end()){
        if ((*iter2) != (*iter1))
            return false;
        iter1++; iter2++;
    }

    return true;
}

bool StateMachineFree::operator==(const StateMachine &one)
{
    if (one._state.size()!=this->_state.size()) return false;

    vector<Part*>::const_iterator iter1 = one._state.begin();
    vector<bool>::iterator iter2 = this->_state.begin();
    while(iter2!=this->_state.end()){
        if ((*iter1)->state != *iter2)
            return false;
        iter1++; iter2++;
    }

    return true;
}

void StateMachineFree::reset(){
    vector<bool>::iterator iter = this->_state.begin();
    while(iter!=this->_state.end()){
        *iter = false;
        iter++;
    }
}

int StateMachineFree::randomize(int count){
    int randnum, parts = this->_state.size();
    vector<bool>::iterator iter;
    for (int i=0;i<count;i++){
        randnum = config::Instance()->rand()%parts;
        iter = this->_state.begin()+randnum;
        *iter = !*iter;
    }
    if (count==1)
        return randnum;
    else
        return -1;
}

bool StateMachineFree::isInitial(){
    vector<bool>::iterator iter = this->_state.begin();
    while (iter!=this->_state.end()){
        if (*iter==true)
            return false;
        iter++;
    }
    return true;
}

std::vector<bool>::iterator StateMachineFree::begin(){
    return this->_state.begin();
}

std::vector<bool>::iterator StateMachineFree::end(){
    return this->_state.end();
}


