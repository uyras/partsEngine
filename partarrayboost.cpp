#include "partarrayboost.h"

PartArrayBoost::PartArrayBoost(mpi::communicator *comm)
{
    this->env = 0;
    if (comm==0){
        this->env = new mpi::environment();
        comm = new mpi::communicator();
    }
    this->comm = comm;
}

PartArrayBoost::~PartArrayBoost()
{
    if (this->env!=0)
        env->~environment();
}

