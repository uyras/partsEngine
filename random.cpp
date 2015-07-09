#include "random.h"
Random*  Random::_self;

Random *Random::Instance(int srand)
{
    if(!Random::_self)
    {
        Random::_self = new Random(srand);
    }
    return Random::_self;
}

void Random::standart()
{
    this->randmax = RAND_MAX;
    this->_randmode = 0;
}

int Random::next(const int maxValue)
{
    return this->next(0,maxValue);
}

int Random::next(const int minValue, const int maxValue)
{
    return int(
                floor(
                   (double)(rand())
                   /
                   (double)(randmax)
                   *
                   (double)(maxValue-minValue)
                   )
                )+minValue;
}

int Random::next()
{
    return rand();
}

double Random::nextDouble()
{
    return (double)(rand())
            /
            (double)(randmax);
}

unsigned long Random::max()
{
    return randmax;
}

void Random::srand(int value)
{
    if (value==0)
        std::srand(time(NULL));
    else
        std::srand(value);
}

Random::Random(int srand)
{
    this->standart();
    this->srand(srand);
}

Random::~Random()
{
    delete Random::_self;
}

