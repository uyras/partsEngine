#include "StateMachine.h"

StateMachine::StateMachine(PartArray* system){
    this->setSystem(system);
}

bool StateMachine::operator++(){
    return this->next();
}

StateMachine & StateMachine::operator =(StateMachine const & two){

    /*
    if (this->_state.size()==two._state.size()){
        //если состояние связано с системой, то попутно придется переворачивать частицы
        if (this->_system){
            std::vector<Part*>::iterator iter1;
            std::vector<bool>::const_iterator iter2;
            std::vector<Part>::iterator part;
            iter1 = this->_state.begin();
            iter2 = two._state.begin();

            part = this->_system->parts.begin();
            while (iter1!=this->_state.end()){
                if ((*iter1)^(*iter2)){
                    part->m.rotate();
                }
                iter1++; iter2++;
                part++;
            }
        }
        this->_state = two._state;
    }*/
    return *this;
}

bool StateMachine::next(){

    //версия для типа bool
    std::vector<Part*>::iterator iter;

    iter = this->_state.begin();

    while (iter!=this->_state.end()){
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

void StateMachine::rotate(int num){/*
    this->_state.at(num)= !this->_state.at(num);
    if (this->_system)
        this->_system->parts.at(num).m.rotate();*/
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
        while(iter1!=one._state.end()){
            (*iter2)->state = *iter1;
            iter1++; iter2++;
        }
    }


    return *this;
}

