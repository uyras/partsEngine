#include "squarespinicecell.h"

SquareSpinIceCell::SquareSpinIceCell()
{
    this->makeTypes();
}

SquareSpinIceCell::~SquareSpinIceCell()
{
    this->types.clear();
}

int SquareSpinIceCell::type()
{
    vector<oneCell>::iterator iter = this->types.begin();
    while (iter!=this->types.end()){
        if (
                this->top->m.scalar((*iter).top)>=0 &&
                this->right->m.scalar((*iter).right)>=0 &&
                this->bottom->m.scalar((*iter).bottom)>=0 &&
                this->left->m.scalar((*iter).left)>=0
                ) {
            return (*iter).type;
        }
        iter++;
    }
    return -1;
}

void SquareSpinIceCell::makeTypes()
{
    int configs[][5] =
    {   //mx_top, my_right, mx_bottom, my_left, type
        {1,-1,-1,1,0},
        {-1,1,-1,1,1},
        {1,1,1,1,1},
        {-1,-1,-1,1,2},
        {1,1,-1,1,2},
        {1,-1,1,1,2},
        {1,-1,-1,-1,2},
        {-1,-1,1,1,3}
    };

    this->types.clear();

    //создаем в памяти конфигурации и заполняем их
    for (int i=0;i<8;i++){
        oneCell cell;
        cell.top = Vect(configs[i][0],0,0);
        cell.right = Vect(0,configs[i][1],0);
        cell.bottom = Vect(configs[i][2],0,0);
        cell.left = Vect(0,configs[i][3],0);
        cell.type = configs[i][4];
        this->types.push_back(cell);

        //Зеркальная конфигурация
        cell = oneCell();
        cell.top= Vect(configs[i][0] * -1,0,0);
        cell.right = Vect(0,configs[i][1] * -1,0);
        cell.bottom = Vect(configs[i][2] * -1,0,0);
        cell.left = Vect(0,configs[i][3] * -1,0);
        cell.type = configs[i][4];
        this->types.push_back(cell);
    }
}
