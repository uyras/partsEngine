#include "StateMachine.h"

StateMachine::StateMachine()
{
    this->_system = NULL;
}

StateMachine::StateMachine(PartArray* system){
    this->setSystem(system);
}

bool StateMachine::operator++(int){
    return this->next();
}

bool StateMachine::operator++(){
    return this->next();
}

StateMachine & StateMachine::operator =(StateMachine const & two){
    //если принимающее состояние не связано с системой, то можно переписывать значения
    if (!this->_system && this->_state.size()!=two._state.size()){
        this->resize(two._state.size());
    }

    if (this->_state.size()==two._state.size()){
        //если состояние связано с системой, то попутно придется переворачивать частицы
        if (this->_system){
            std::vector<bool>::iterator iter1;
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
    }
    return *this;
}

StateMachine* StateMachine::copy(PartArray* system){
    StateMachine* temp = new StateMachine();
    temp->_state = this->_state;
    temp->_system = system;
    return temp;
}

bool StateMachine::next(){
    //версия для типа bool
    if (this->_system){
        std::vector<bool>::iterator iter;
        std::vector<Part>::iterator part;

        iter = this->_state.begin();
        part = this->_system->parts.begin();

        while (iter!=this->_state.end()){
            if (*iter==0){
                *iter=1;
                part->m.rotate();
                return true;
                break;
            } else {
                *iter=0;
                part->m.rotate();
            }
            iter++;
            part++;
        }

        return false;
    } else {
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
}

void StateMachine::setSystem(PartArray* system){
    system->state = this;
    this->_system = system;
    this->_state.clear();
    this->resize();
}

void StateMachine::resize(int newSize){
    if (newSize==-1)
        newSize = this->_system->count();
    this->_state.resize(newSize,false);
}

void StateMachine::draw(){
    std::vector<bool>::iterator iter;
    iter = this->_state.end();
    do{
        iter--;
        if (*iter==0){
            std::cout<<"0";
        } else {
            std::cout<<"1";
        }
    } while (iter!=this->_state.begin());
    std::cout<<std::endl;
}

void StateMachine::reset(){
    std::vector<bool>::iterator iter;
    std::vector<Part>::iterator part;
    iter = this->_state.begin();
    part = this->_system->parts.begin();
    while(iter!=this->_state.end()){
        if (*iter==1){
            part->m.rotate();
            *iter=0;
        }
        iter++; part++;
    }
}

void StateMachine::rotate(int num){
    this->_state.at(num)= !this->_state.at(num);
    if (this->_system)
        this->_system->parts.at(num).m.rotate();
}

std::vector<bool>::iterator StateMachine::begin(){
    return this->_state.begin();
}

std::vector<bool>::iterator StateMachine::end(){
    return this->_state.end();
}

int StateMachine::randomize(int count){
    int randnum, parts = this->_state.size();
    for (int i=0;i<count;i++){
        randnum = config::Instance()->rand()%parts;
        this->_state.at(randnum)= !this->_state.at(randnum);
        if (this->_system)
            this->_system->parts.at(randnum).m.rotate();
    }
    if (count==1)
        return randnum;
    else
        return -1;
}

