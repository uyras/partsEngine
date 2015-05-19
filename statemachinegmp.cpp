#include "StateMachine.h"

#include <boost/multiprecision/cpp_int.hpp>
#include <iostream>

using namespace boost::multiprecision;

void StateMachine::add(const cpp_int & b)
{
    std::vector<Part*>::iterator iter = this->_state.begin();

    unsigned i=0;
    unsigned short int m=0;
    while (iter!=this->_state.end()){
        m=(int)bit_test(b,i)+(int)(*iter)->state+m;
        switch (m){
        case 1:
            if ((*iter)->state!=1)
                (*iter)->rotate();
            m=0;
            break;
        case 2:
            if ((*iter)->state==1)
                (*iter)->rotate();
            m=1;
            break;
        case 3:
            if ((*iter)->state!=1)
                (*iter)->rotate();
            m=1;
            break;
        }
        iter++; i++;
    }
    return;
}
