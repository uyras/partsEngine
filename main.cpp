#include <iostream>
#include <fstream>
#include <cstdlib>
#include "config.h"
#include <ctime>

#include "StateMachine.h"
#include "StateMachineFree.h"
#include "PartArray.h"
#include "Part.h"

using namespace std;

int main(){

    config::Instance()->srand(time(NULL));
    PartArray *sys1,*sys2;
    sys1 = new PartArray(10,10,1);
    sys1->dropChain(2);
    StateMachineFree *state2 = new StateMachineFree(sys1->state);
    StateMachineFree *state3 = new StateMachineFree(sys1->state);
    //state2->randomize();
    cout<<(*state3==*state2);


    cout<<"finish";
    return 0;
}
