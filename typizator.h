#include <stddef.h>
#include <mpi.h>
#include "Vect.h"
#include "Part.h"

class typizator
{
public:
    MPI_Datatype _vect, _part;

    typizator(void)
    {
    }

    MPI_Datatype& vect(){
        if (!this->_vect){
            //описывается класс вектора
            int len[5]={1,1,1,1,1}; //описываем где сколько элементов
            MPI_Aint pos[5] = {
                0,
                offsetof(Vect,x),
                offsetof(Vect,y),
                offsetof(Vect,z),
                sizeof(Vect)
            };
            MPI_Datatype typ[5] = { MPI_LB, MPI_DOUBLE, MPI_DOUBLE, MPI_DOUBLE, MPI_UB };
            MPI_Type_create_struct( 5, len, pos, typ, &this->_vect );
            MPI_Type_commit( &this->_vect );

            /* старая реалиация, не передавался z
            MPI_Type_contiguous( 3, MPI_DOUBLE, &this->_vect );//описываем вектор
            MPI_Type_commit( &this->_vect ); //добавляем вектор в реестр */
        }
        return this->_vect;
    }

    MPI_Datatype& part(){
        if (!this->_part){
            //описываем тип "частица"
            int len[6]={1,1,1,1,1,1}; //описываем где сколько элементов
            MPI_Aint pos[6] = {
                0,
                offsetof(Part,pos),
                offsetof(Part,m),
                offsetof(Part,h),
                offsetof(Part,sector),
                sizeof(Part)
            };
            MPI_Datatype typ[6] = {
                MPI_LB,
                this->vect(),
                this->vect(),
                this->vect(),
                MPI_UNSIGNED_SHORT,
                MPI_UB };
            MPI_Type_create_struct( 6, len, pos, typ, &this->_part );
            MPI_Type_commit( &this->_part );
        }
        return this->_part;
    }

    ~typizator(void)
    {
        //MPI_Type_free( &(this->vect()) );
        //MPI_Type_free( &(this->part()) );
    }
};
typizator MPI_types;
