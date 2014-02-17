#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include <vector>

#include "PartArray.h"
#include "Part.h"
#include "config.h"

class PartArray; //фикс, так как используется перекрестное объявление с этим классом
class StateMachine
{
public:
    StateMachine();
    StateMachine(PartArray*);

    /**
     * @brief reset сбросить состояние в начальное состояние, т.е. все состояния по нулям
     */
    void reset();
    /**
     * @brief randomize Переворачивает несколько случайных частиц
     * @param count Количество частиц, которые должны быть перевернуты
     * @return если перевернуть одну частицу, возвращает ее номер, иначе 0
     */
    int randomize(int count=1);

    /**
     * @brief rotate Поворачивает частицу
     * @param num Номер частицы, которую нужно повернуть
     */
    void rotate(int num);

    void setSystem(PartArray*);
    PartArray* getSystem();

    /**
     * @brief isInitial Проверяет, находится ли система в начальном состоянии
     * @return true если поворот всех спинов - 0.
     */
    bool isInitial();

    /**
     * @brief next переключиться на следующее состояние
     * @return false если система возвращается в начальное состояние
     */
    bool next();

    bool operator++(int);
    bool operator++();

    StateMachine & operator =(const StateMachine & two);
    StateMachine* copy(PartArray* system = NULL);

    void resize(int size = -1);

    void draw();

    std::vector<bool>::iterator begin();
    std::vector<bool>::iterator end();

public:
    PartArray* _system;
    std::vector<bool> _state;
};

#endif // STATEMACHINE_H
