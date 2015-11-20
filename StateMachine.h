#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include <vector>

#include "PartArray.h"
#include "Part.h"
#include "config.h"
#include "random.h"
#include "statemachinebase.h"
#include "StateMachineFree.h"

class PartArray; //фикс, так как используется перекрестное объявление с этим классом
class StateMachineFree;

class StateMachine:
        public StateMachineBase
{
public:
    StateMachine();
    StateMachine(PartArray *system);
    StateMachine(PartArray & system);

    virtual void reset();
    virtual void setLast();
    virtual int randomize(int count=1);
    virtual bool isFirst();
    virtual bool isLast();
    virtual bool isHalfLast();
    virtual bool next();
    virtual bool halfNext();
    virtual bool prev();
    virtual bool halfPrev();
    virtual bool operator++();
    virtual bool operator--();
    virtual bool operator++(int);
    virtual bool operator--(int);
    virtual std::string toString() const;
    virtual bool fromString(const std::string&);
    virtual bool operator[](const unsigned long int) const;
    virtual unsigned long int size() const;

    void connect(PartArray *system);
    void disconnect();
    bool connected() const;

    /**
     * @brief hardReset Жестко обнуляет состояния частиц, не трогая их намагниченности.
     * Функция делает вид, что система только что сгенерировалась в таком состоянии.
     */
    void hardReset();

    StateMachine &  operator+=(const unsigned long int val);
    bool operator==(const StateMachine &one) const;


    StateMachine& operator=(const StateMachineFree &state);
    StateMachine& operator=(const StateMachine & state);

private:
    PartArray* _system;
    //#ifdef WITH_BOOST
    //void add(const boost::multiprecision::cpp_int & b);
    //#endif //WITH_BOOST
};

#endif // STATEMACHINE_H
