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
    config::Instance()->set3D();
    Vect a(1.1,0.3,0.5), b(1,0,0), c;
    a = b*=2;
    a=a*2;
    a.draw();
    b.draw();
    c.draw();

    /*config::Instance()->srand(time(NULL));
    PartArray *sys1;
    sys1 = new PartArray(3,3,3);
    sys1->dropChain(2);
    sys1->savePVPython("2.py");
    //sys1->setMAllUp();
    //sys1->savePVPython("2.py");*/

    cout<<"finish";
    return 0;
}
