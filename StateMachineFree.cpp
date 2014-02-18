#include "StateMachineFree.h"

StateMachineFree::StateMachineFree()
{
}

void StateMachineFree::draw(){
    std::vector<bool>::iterator iter;
    iter = this->_state.end();
    while (iter!=this->_state.begin()){
        iter--;
        if (*iter==0){
            std::cout<<"0";
        } else {
            std::cout<<"1";
        }
    }
    std::cout<<std::endl;
}

bool StateMachineFree::next(){
    //версия для типа bool
    std::vector<bool>::iterator iter;
    iter = this->_state.begin();

    while (iter!=this->_state.end()){
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
