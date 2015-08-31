#include "honeycombspinicearray.h"

HoneycombSpinIceArray::HoneycombSpinIceArray()
{

}

HoneycombSpinIceArray::~HoneycombSpinIceArray()
{

}

void HoneycombSpinIceArray::dropHoneyComb(int m, int n, double a, Part *tmp)
{
    this->clear();

    this->m = m; this->n = n;
    double mLength=0; //магнитный момент одной частицы
    if (tmp==0){ //если шаблон частицы не был передан, делаем шаблон по умолчанию
        tmp = new Part();
        mLength = config::Instance()->m;
    } else {
        mLength = tmp->m.length();
    }

    double r=a*sqrt(3)/2;
    for(int j=0;j<n;j++)
    {
        for(int i=0;i<m;i++)
        {
            double x=2*r*i+r*(pow(-1,j)+1)/2;
            double y=sqrt(3*r*r)*j;

            HoneycombSpinIceCell *tempCell = new HoneycombSpinIceCell();
            tempCell->pos = Vect(x,y,1);

            vector<Part*> hexPart;

            //genHexPart start
            for(int k=0;k<6;k++)
            {
                Part* temp = tmp->copy();
                temp->pos.x = r*cos(2*M_PI*k/6)+x;
                temp->pos.y = r*sin(2*M_PI*k/6)+y;
                temp->m.x = cos(2*M_PI*k/6+M_PI/2)*mLength;
                temp->m.y = sin(2*M_PI*k/6+M_PI/2)*mLength;
                hexPart.push_back(temp);
            }
            //genHexPart end

            vector<Part*>::iterator iter = hexPart.begin();
            while (iter!=hexPart.end()){
                bool add=true;
                vector<Part*>::iterator iter2 = this->parts.begin();
                while(iter2!=this->parts.end()){
                    if (
                            this->_double_equals((*iter2)->pos.x,(*iter)->pos.x) &&
                            this->_double_equals((*iter2)->pos.y,(*iter)->pos.y)
                            )
                    {
                        add=false;
                        delete (*iter);//удаляем из памяти
                        (*iter) = (*iter2);
                        break;
                    }
                    iter2++;
                }

                if(add)
                    this->insert(*iter);

                tempCell->parts.push_back(*iter);

                iter++;
            }
            this->cells.push_back(tempCell);
        }
    }
}

bool HoneycombSpinIceArray::setToGroundState()
{
    HoneycombSpinIceCell* temp;
    for (int i=0;i<n;i++)
        for (int j=0;j<m;j++){
            temp = cells[i*m+j];
            switch ((j+(i%2))%3){
            case 0:
                temp->rotateClockWise();
                break;
            case 1:
                temp->rotateAntiClockWise();
                break;
            case 2:
                temp->rotateChaotic();
                break;
            }
        }
    return true;
}

bool HoneycombSpinIceArray::setToMaximalState()
{
    for (int i=0;i<m*n;i++){
        cells[i]->rotateChaotic();
    }
    return true;
}

void HoneycombSpinIceArray::clear()
{
    this->clearCells();

    PartArray::clear();
}

PartArray *HoneycombSpinIceArray::beforeCopy()
{
    return (PartArray*) new HoneycombSpinIceArray();
}

void HoneycombSpinIceArray::afterCopy(PartArray *temp2)
{
    HoneycombSpinIceArray* temp = (HoneycombSpinIceArray*) temp2;
    //копируем содержание ячеек
    vector<HoneycombSpinIceCell*>::iterator iter = this->cells.begin();
    HoneycombSpinIceCell *tempCell, *oldCell;
    while(iter!=this->cells.end()){
        tempCell = new HoneycombSpinIceCell();
        oldCell = *iter;
        for (int j=0;j<6;j++)
            for (int i=0;i<this->count();i++){
                if (oldCell->parts[j]==this->parts[i]){
                    tempCell->parts.push_back(temp->parts[i]);
                    break;
                }
            }

        tempCell->pos = oldCell->pos;
        temp->cells.push_back(tempCell);
        iter++;
    }
    temp->m = this->m;
    temp->n = this->n;
}

void HoneycombSpinIceArray::clearCells()
{
    vector<HoneycombSpinIceCell*>::iterator iter = this->cells.begin();
    while (iter!=this->cells.end()){
        delete (*iter); //удаляем из памяти все ячейки
        iter++;
    }
    this->cells.clear();
}

