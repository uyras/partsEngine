#include "squarespinicearray.h"

SquareSpinIceArray::SquareSpinIceArray()
{
    this->_type = "squarespinice";
}

SquareSpinIceArray::~SquareSpinIceArray()
{
    this->clearCells();
}

void SquareSpinIceArray::dropSpinIce(int hCells, int vCells, double l)
{
    if (config::Instance()->dimensions()!=2)
        qFatal("Square spin ice is possible only in 2 dimensionals model");

    this->clear();

    double l_2 = l/2.; //полурешетка
    Vect center(0,0,0);

    Part *temp;
    Vect partPos(0,0,0);
    center.y = l_2;
    for (int i=0;i<vCells;i++){
        center.x = l_2;
        for (int j=0;j<hCells;j++){
            //создаем ячейку
            SquareSpinIceCell *cell = new SquareSpinIceCell();
            cell->pos = center;

            //добавляем верхнюю
            partPos.x = center.x;
            partPos.y = center.y+l_2;

            if ((temp = this->findByPosition(partPos)) == 0){
                temp = new Part();
                temp->shape = Part::ELLIPSE;
                temp->pos = partPos;
                temp->m = Vect(config::Instance()->m,0,0);
                this->insert(temp);
            }
            cell->top = temp;

            //добавляем нижнюю
            partPos.x = center.x;
            partPos.y = center.y-l_2;

            if ((temp = this->findByPosition(partPos)) == 0){
                temp = new Part();
                temp->shape = Part::ELLIPSE;
                temp->pos = partPos;
                temp->m = Vect(config::Instance()->m,0,0);
                this->insert(temp);
            }
            cell->bottom = temp;

            //добавляем левую
            partPos.x = center.x-l_2;
            partPos.y = center.y;

            if ((temp = this->findByPosition(partPos)) == 0){
                temp = new Part();
                temp->shape = Part::ELLIPSE;
                temp->pos = partPos;
                temp->m = Vect(0,config::Instance()->m,0);
                this->insert(temp);
            }
            cell->left = temp;

            //добавляем правую
            partPos.x = center.x+l_2;
            partPos.y = center.y;

            if ((temp = this->findByPosition(partPos)) == 0){
                temp = new Part();
                temp->shape = Part::ELLIPSE;
                temp->pos = partPos;
                temp->m = Vect(0,config::Instance()->m,0);
                this->insert(temp);
            }
            cell->right = temp;
            cell->row = i;
            cell->column = j;

            this->cells.push_back(cell);

            center.x+=l;
        }
        center.y+=l;
    }
}

Part *SquareSpinIceArray::findByPosition(const Vect &pos, double epsilon)
{
    vector<Part*>::reverse_iterator iter = this->parts.rbegin();
    Part* temp;
    while (iter!=this->parts.rend()){
        temp = *iter;
        if (fabs(temp->pos.x-pos.x)<epsilon && fabs(temp->pos.y-pos.y)<epsilon){
            return temp;
        }
        iter++;
    }
    return 0;
}

double SquareSpinIceArray::setToGroundState()
{
    if (this->minstate->size()==0){
        vector<SquareSpinIceCell*>::iterator iter = this->cells.begin();
        SquareSpinIceCell *temp;
        while (iter!=this->cells.end()){
            temp = *iter;
            int direction = (temp->row+temp->column)%2*2-1; //либо +1 либо -1, в шахматном порядке

            if (temp->top->m.scalar(Vect(direction,0,0))<0)
                temp->top->rotate();
            if (temp->bottom->m.scalar(Vect(direction*-1.,0,0))<0)
                temp->bottom->rotate();
            if (temp->left->m.scalar(Vect(0,direction,0))<0)
                temp->left->rotate();
            if (temp->right->m.scalar(Vect(0,direction*-1.,0))<0)
                temp->right->rotate();

            iter++;
        }
        *minstate = *state;
        this->changeState();
    } else
        *state = *minstate;
    return eMin=E();
}

double SquareSpinIceArray::setToMaximalState()
{
    if (this->maxstate->size()==0){
        vector<SquareSpinIceCell*>::iterator iter = this->cells.begin();
        SquareSpinIceCell *temp;
        while (iter!=this->cells.end()){
            temp = *iter;
            int direction = (temp->row+temp->column)%2*2-1;

            if (temp->top->m.scalar(Vect(direction,0,0))<0)
                temp->top->rotate();
            if (temp->bottom->m.scalar(Vect(direction*-1.,0,0))<0)
                temp->bottom->rotate();
            if (temp->left->m.scalar(Vect(0,direction*-1.,0))<0)
                temp->left->rotate();
            if (temp->right->m.scalar(Vect(0,direction,0))<0)
                temp->right->rotate();

            iter++;
        }
        *maxstate = *state;
        this->changeState();
    } else
        *state = *maxstate;
    return eMax=E();
}

PartArray *SquareSpinIceArray::beforeCopy()
{
    return new SquareSpinIceArray();
}

void SquareSpinIceArray::afterCopy(PartArray *temp2)
{
    SquareSpinIceArray* temp = (SquareSpinIceArray*) temp2;
    //копируем содержание ячеек
    vector<SquareSpinIceCell*>::iterator iter = this->cells.begin();
    SquareSpinIceCell *tempCell, *oldCell;
    while(iter!=this->cells.end()){
        tempCell = new SquareSpinIceCell();
        oldCell = *iter;
        for (int i=0;i<this->count();i++){
            if (oldCell->top==this->parts[i])
                tempCell->top=temp->parts[i];

            if (oldCell->right==this->parts[i])
                tempCell->right=temp->parts[i];

            if (oldCell->bottom==this->parts[i])
                tempCell->bottom=temp->parts[i];

            if (oldCell->left==this->parts[i])
                tempCell->left=temp->parts[i];
        }

        tempCell->column = oldCell->column;
        tempCell->row = oldCell->row;
        tempCell->pos = oldCell->pos;
        temp->cells.push_back(tempCell);
        iter++;
    }
}

void SquareSpinIceArray::clear()
{
    this->clearCells();

    PartArray::clear();
}

void SquareSpinIceArray::load(QString file)
{
    this->clear();
    //load base part of file
    PartArray::load(file);

    //open file
    QFile infile(file);
    infile.open(QFile::ReadOnly);
    QTextStream f(&infile);

    //skip to cells section
    QString s;
    while (s!="[cells]"){
        s = f.readLine();
    }


    //read cells data
    s=f.readLine();

    QStringList params;
    SquareSpinIceCell *tempCell;
    while (! (
               (s[0]=='[' && s[s.length()-1]==']') ||
               (s.isEmpty())
               )){ //read due to the next section or end of file
        params = s.split('\t');
        tempCell = new SquareSpinIceCell();
        tempCell->pos = Vect(
                    params[0].toDouble(),
                    params[1].toDouble(),
                    1
                    );
        tempCell->row = params[2].toInt();
        tempCell->column = params[3].toInt();
        tempCell->top = this->getById(params[4].toInt());
        tempCell->bottom = this->getById(params[5].toInt());
        tempCell->left = this->getById(params[6].toInt());
        tempCell->right = this->getById(params[7].toInt());

        this->cells.push_back(tempCell);

        s=f.readLine();
    }

    //close file
    infile.close();
}

void SquareSpinIceArray::save(QString file)
{
    //save base part of file
    PartArray::save(file);

    //open file in append mode
    QFile outfile(file);
    outfile.open(QFile::WriteOnly | QFile::Append);
    QTextStream f(&outfile);

    //write header
    f<<"[cells]"<<endl;

    //write particles
    vector<SquareSpinIceCell*>::iterator iter = this->cells.begin();
    while (iter != this->cells.end()) {
        f<<(*iter)->pos.x<<"\t";
        f<<(*iter)->pos.y<<"\t";
        f<<(*iter)->row<<"\t";
        f<<(*iter)->column<<"\t";
        f<<(*iter)->top->Id()<<"\t";
        f<<(*iter)->bottom->Id()<<"\t";
        f<<(*iter)->left->Id()<<"\t";
        f<<(*iter)->right->Id();

        f << endl;
        iter++;
    }

    //close file
    outfile.close();
}

void SquareSpinIceArray::clearCells()
{
    vector<SquareSpinIceCell*>::iterator iter = this->cells.begin();
    while (iter!=this->cells.end()){
        delete (*iter); //удаляем из памяти все ячейки
        iter++;
    }
    this->cells.clear();
}

