#ifndef PARTARRAYBOOST_H
#define PARTARRAYBOOST_H

#include <boost/mpi.hpp>
#include <boost/serialization/access.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/split_free.hpp>
#include "Vect.h"
#include "Part.h"
#include "StateMachine.h"
#include "StateMachineFree.h"
#include "PartArray.h"

namespace boost {
namespace serialization {

template<class Archive> void serialize(Archive & ar, Vect & v, const unsigned int version)
{
    ar & v.x;
    ar & v.y;
    ar & v.z;
}

template<class Archive> void serialize(Archive & ar, Part & p, const unsigned int version)
{
    ar & p.pos;
    ar & p.m;
    ar & p.h;
    ar & p.e;
    ar & p.eArray;
    ar & p.r;
    ar & p._vol;
    ar & p.sector;
    ar & p.state;
    ar & p.shape;
    ar & p.h1;
    ar & p.w1;
}

template<class Archive> void serialize(Archive & ar, StateMachine & v, const unsigned int version)
{
    ar & v._state;
}

template<class Archive> void load(Archive & ar, PartArray & v, const unsigned int version)
{
    ar & v.E1;
    ar & v.E2;
    ar & v.eIncrementalTemp;
    ar & v.parts;
    ar & v.size;
    v._construct();
}

template<class Archive> void save(Archive & ar,const PartArray & v, const unsigned int version)
{
    ar & v.E1;
    ar & v.E2;
    ar & v.eIncrementalTemp;
    ar & v.parts;
    ar & v.size;
}

} // namespace serialization
} // namespace boost

BOOST_SERIALIZATION_SPLIT_FREE(PartArray)

namespace mpi=boost::mpi;

class PartArrayBoost : public PartArray
{
public:
    PartArrayBoost(mpi::communicator *comm=0);
    ~PartArrayBoost();

protected:
    mpi::environment *env;
    mpi::communicator *comm;
};

#endif // PARTARRAYBOOST_H
