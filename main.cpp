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


    PartArray *s = new PartArray(), *s1;
    Part* p;

    p = new Part();
    p->m.x = p->m.y = 1;
    s->insert(p);
    p = new Part();
    p->m.x = p->m.y = 2;
    s->insert(p);
    p = new Part();
    p->m.x = p->m.y = 3;
    s->insert(p);


    s->state->draw();
    s1 = s->copy();
    for (;;){
        s1 = s->copy();
        delete s1;
        //s1->clear();
    }

    /*
    config::Instance()->srand(time(NULL));
    PartArray *sys;
    int trueCount = 0;
    for (int i=0;i<100;i++){
        sys = new PartArray(10,10,1);

        sys->dropChain(2);
        sys->shuffleM();

        sys->setToPTGroundState(5,1000);

        cout<<sys->calcEnergy1()<<endl;
        if (fabs(sys->calcEnergy1()+6.67773)<0.00001)
            trueCount++;
        delete sys;
    }
    cout<<"result accuracy: "<<(double)trueCount/100.*100.<<"%"<<endl;
    */

    cout<<"finish";
    return 0;
}
