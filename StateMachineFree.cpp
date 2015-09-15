#include "StateMachineFree.h"

StateMachineFree::StateMachineFree()
{
    this->_state.clear();
}

StateMachineFree::StateMachineFree(const unsigned long size)
{
    this->_state.resize(size,false);
}

StateMachineFree::StateMachineFree(const StateMachine &state)
{
    this->_construct(&state);
}

StateMachineFree::StateMachineFree(const StateMachine *state)
{
    this->_construct(state);
}

/*
StateMachineFree::StateMachineFree(StateMachine* state){
    this->_state.clear();
    for (int i=0;i<state->size();i++){
        this->_state.push_back(state[i]);
    }
}
*/

void StateMachineFree::reset(){
    vector<bool>::iterator iter = this->_state.begin();
    while(iter!=this->_state.end()){
        *iter = false;
        iter++;
    }
}

void StateMachineFree::setLast(){
    vector<bool>::iterator iter = this->_state.begin();
    while(iter!=this->_state.end()){
        *iter = true;
        iter++;
    }
}

int StateMachineFree::randomize(int count){
    int randnum=0, parts = this->_state.size();
    vector<bool>::iterator iter;
    for (int i=0;i<count;i++){
        randnum = config::Instance()->rand()%parts;
        iter = this->_state.begin()+randnum;
        *iter = !*iter;
    }
    if (count==1)
        return randnum;
    else
        return count;
}

bool StateMachineFree::isFirst(){
    vector<bool>::iterator iter = this->_state.begin();
    while (iter!=this->_state.end()){
        if (*iter==true)
            return false;
        iter++;
    }
    return true;
}

bool StateMachineFree::isLast(){
    vector<bool>::iterator iter = this->_state.begin();
    while (iter!=this->_state.end()){
        if (*iter==false)
            return false;
        iter++;
    }
    return true;
}

bool StateMachineFree::isHalfLast(){
    vector<bool>::iterator iter = this->_state.begin();
    while (iter!=this->_state.end()-1){
        if (*iter==false)
            return false;
        iter++;
    }
    return true;
}

bool StateMachineFree::next(){
    std::vector<bool>::iterator iter;
    iter = this->_state.begin();

    while (iter != this->_state.end()){
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

bool StateMachineFree::halfNext(){
    std::vector<bool>::iterator iter;
    iter = this->_state.begin();

    while (iter != (this->_state.end()-1)){
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

bool StateMachineFree::prev(){
    std::vector<bool>::iterator iter;
    iter = this->_state.begin();

    while (iter != this->_state.end()){
        if (*iter==1){
            *iter=0;
            return true;
            break;
        } else {
            *iter=1;
        }
        iter++;
    }

    return false;
}

bool StateMachineFree::halfPrev(){
    std::vector<bool>::iterator iter;
    iter = this->_state.begin();

    while (iter != (this->_state.end()-1)){
        if (*iter==1){
            *iter=0;
            return true;
            break;
        } else {
            *iter=1;
        }
        iter++;
    }

    return false;
}

bool StateMachineFree::operator++()
{
    return this->next();
}

bool StateMachineFree::operator--()
{
    return this->prev();
}

bool StateMachineFree::operator++(int)
{
    return this->next();
}

bool StateMachineFree::operator--(int)
{
    return this->prev();
}

std::string StateMachineFree::toString() const{
    std::vector<bool>::const_iterator iter;
    std::string s="";
    iter = this->_state.begin();
    while (iter!=this->_state.end()){
        if (*iter==0){
            s+="0";
        } else {
            s+="1";
        }
        iter++;
    }
    return s;
}

bool StateMachineFree::fromString(const std::string &s)
{
    this->_state.clear();
    for (unsigned int i=0;i<s.size();i++){
        this->_state.push_back(s[i]!='0');
    }
    return true;
}

bool StateMachineFree::operator [](const unsigned long int num) const{
    return this->_state[num];
}

bool StateMachineFree::at(const unsigned long num) const
{
    return this->operator [](num);
}

unsigned long int StateMachineFree::size() const{
    return this->_state.size();
}

StateMachineFree & StateMachineFree::operator= (const StateMachineFree & one){
    this->_state.clear();

    for (unsigned int i=0;i<one.size();i++)
        this->_state.push_back(one[i]);

    return *this;
}

bool StateMachineFree::operator==(const StateMachineFree &one)
{
    if (one._state.size()!=this->_state.size()) return false;
    if (one._state.size()==this->_state.size() && (one._state.size()==0 || one._state.size()==1)) return true;

    vector<bool>::const_iterator iter1 = one._state.begin();
    vector<bool>::iterator iter2 = this->_state.begin();

    //вычисляем, прямое или обратное совпадение считать
    bool compEquals=true;
    if ((*iter2) != (*iter1))
        compEquals = false;

    while(iter2!=this->_state.end()){
        if ( ((*iter2) == (*iter1)) != compEquals)
            return false;
        iter1++; iter2++;
    }

    return true;
}

StateMachineFree &StateMachineFree::operator+=(int val)
{
    for (int i=0;i<val;i++){
        this->next();
    }
    return *this;
}

void StateMachineFree::resize(const unsigned long size)
{
    this->_state.resize(size);
}

void StateMachineFree::_construct(const StateMachine *state)
{
    this->_state.clear();
    for (unsigned int i=0;i<state->size();i++){
        this->_state.push_back(state->operator[](i));
    }
}


