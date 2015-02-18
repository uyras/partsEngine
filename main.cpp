#include <QCoreApplication>
#include <QString>
#include <cmath>
#include <fstream>
#include <vector>
#include <deque>
#include <algorithm>
#include "config.h"
#include "PartArray.h"
#include "Part.h"

using namespace std;

int d=400; //параметр решетки

class SuperPart
{
public:
    SuperPart(Part* o){
        this->original = o;
    }
    ~SuperPart(){
        delete this->original;
    }

    bool isNeighbour(SuperPart* p2) {
        return
                this->original->pos.radius(p2->original->pos).length()<d &&
                this->original->m.scalar(p2->original->m)==0 &&
                this!=p2;
    }
    bool isConnected(SuperPart* p2){
        return this->original->interact(p2->original).scalar(this->original->m)>0;
    }

    vector<SuperPart*> neighbours; //массив соседей для частицы
    bool walked; //флаг,учавствует (или учавствовала) ли частица в кластере
    Part* original;
};

class SuperPartArray
{
public:
    /**
     * @brief SuperPartArray В конструкторе супер-класса заполняем массив супер-частиц
     * @param sys
     */
    double l;
    SuperPartArray(PartArray* sys){
        this->l = l;
        vector<Part*>::iterator iter = sys->parts.begin();
        Part* temp1;
        SuperPart* temp2;
        while (iter!=sys->parts.end()){
            temp1 = *iter;
            temp2 = new SuperPart(temp1);
            this->parts.push_back(temp2);
            iter++;
        }
        this->oldArray = sys;
        this->findNeighbours();
        this->clearWalked();
    }
    ~SuperPartArray(){
        this->parts.clear();
        delete this->oldArray;
    }

    void clearWalked(){
        vector<SuperPart*>::iterator iter = this->parts.begin();
        while (iter!=this->parts.end()){
            (*iter)->walked = false;
            iter++;
        }
    }

    SuperPart* getUnwalked(){
        vector<SuperPart*>::iterator iter = this->parts.begin();
        while (iter!=this->parts.end()){
            if (!(*iter)->walked)
                return *iter;
            iter++;
        }
        return 0;
    }


    vector<SuperPart*> parts;
    PartArray* oldArray;

    /**
     * @brief findNeighbours Ищем соседей для каждой частицы и прописываем их в свойствах
     */
    void findNeighbours(){
        vector<SuperPart*>::iterator iter1 = this->parts.begin();
        vector<SuperPart*>::iterator iter2;
        SuperPart* temp,*temp2;
        while (iter1!=this->parts.end()){
            temp=*iter1;
            iter2=this->parts.begin();
            while (iter2!=this->parts.end()){
                temp2 = *iter2;
                if (temp->isNeighbour(temp2))
                    temp->neighbours.push_back(temp2);
                iter2++;
            }
            iter1++;
        }
    }

    vector<SuperPart*> getMaxCluster(){
        vector<SuperPart*> maxCluster, currentCluster;
        SuperPart* temp;

        //сначала делаем все частицы некластеризованными
        this->clearWalked();
        while ((temp = this->getUnwalked())!=0){
            currentCluster.clear();
            currentCluster.push_back(temp);
            temp->walked = true;

            walkNeighbours(temp,currentCluster);
            //запоминаем кластер с максимальным
            if (maxCluster.size()<=currentCluster.size()){
                maxCluster=currentCluster;
            }
        }
        return maxCluster;

    }

    void walkNeighbours(
            SuperPart* part,
            vector<SuperPart*> & currentCluster
            ){
        vector<SuperPart*>::iterator iter = part->neighbours.begin();
        SuperPart* temp;
        while (iter!=part->neighbours.end()){
            temp = *iter;
            if (!temp->walked && part->isConnected(temp)){
                temp->walked = true;
                currentCluster.push_back(temp);
                this->walkNeighbours(temp,currentCluster);
            }
            iter++;
        }
    }
};

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    config::Instance()->set2D();
    config::Instance()->randmode_standart();
    config::Instance()->srand(time(NULL));

    int w=80,h=220;//высота и ширина частицы
    int cell=5; //количество ячеек
    int l = (cell*d)+w+1; //высота и ширина решетки (4 ячейки всего)
    PartArray *sys= new PartArray(l,l,1);
    sys->dropSpinIce(w,h,d);
    sys->state->randomize(30);
    sys->save("one.dat");
    cout<<"n="<<sys->count()<<"; d="<<d<<endl;

    SuperPartArray* ssys = new SuperPartArray(sys);
    vector<SuperPart*> v = ssys->getMaxCluster();
    cout<<v.size()<<endl;

    vector<SuperPart*>::iterator iter = v.begin();
    PartArray* sys2 = new PartArray(l,l,1);
    while (iter!=v.end()){
        sys2->insert((*iter)->original);
        iter++;
    }
    sys2->save("two.dat");

    cout<<"finish!"<<endl;

    return a.exec();
}
