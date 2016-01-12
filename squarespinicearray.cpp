#include "squarespinicearray.h"

SquareSpinIceArray::SquareSpinIceArray()
{
    this->_type = "squarespinice";
    SysLoader::reg<SquareSpinIceArray>(type());
}

SquareSpinIceArray::SquareSpinIceArray(const SquareSpinIceArray &sys)
    :PartArray(sys)
{
    //копируем содержание ячеек
    vector<SquareSpinIceCell*>::const_iterator iter = sys.cells.begin();
    SquareSpinIceCell *tempCell, *oldCell;
    while(iter!= sys.cells.end()){
        tempCell = new SquareSpinIceCell();
        oldCell = *iter;
        for (int i=0;i<this->count();i++){
            if (*(oldCell->top)==*(this->parts[i]))
                tempCell->top=this->parts[i];

            if (*(oldCell->right)==*(this->parts[i]))
                tempCell->right=this->parts[i];

            if (*(oldCell->bottom)==*(this->parts[i]))
                tempCell->bottom=this->parts[i];

            if (*(oldCell->left)==*(this->parts[i]))
                tempCell->left=this->parts[i];
        }

        tempCell->column = oldCell->column;
        tempCell->row = oldCell->row;
        tempCell->pos = oldCell->pos;
        this->cells.push_back(tempCell);
        iter++;
    }
}

SquareSpinIceArray::~SquareSpinIceArray()
{
    this->clearCells();
}

SquareSpinIceArray &SquareSpinIceArray::operator =(const SquareSpinIceArray &sys)
{
    if (this == &sys) return *this;

    this->PartArray::operator =(sys);

    //копируем содержание ячеек
    vector<SquareSpinIceCell*>::const_iterator iter = sys.cells.begin();
    SquareSpinIceCell *tempCell, *oldCell;
    while(iter!= sys.cells.end()){
        tempCell = new SquareSpinIceCell();
        oldCell = *iter;
        for (int i=0;i<this->count();i++){
            if (*(oldCell->top)==*(this->parts[i]))
                tempCell->top=this->parts[i];

            if (*(oldCell->right)==*(this->parts[i]))
                tempCell->right=this->parts[i];

            if (*(oldCell->bottom)==*(this->parts[i]))
                tempCell->bottom=this->parts[i];

            if (*(oldCell->left)==*(this->parts[i]))
                tempCell->left=this->parts[i];
        }

        tempCell->column = oldCell->column;
        tempCell->row = oldCell->row;
        tempCell->pos = oldCell->pos;
        this->cells.push_back(tempCell);
        iter++;
    }

    return *this;
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

StateMachineFree SquareSpinIceArray::groundState()
{
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
    return state;
}

StateMachineFree SquareSpinIceArray::maximalState()
{
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
    return state;
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

