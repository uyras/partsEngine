#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include <vector>

#include "PartArray.h"
#include "Part.h"
#include "config.h"
#include "StateMachineFree.h"

class PartArray; //фикс, так как используется перекрестное объявление с этим классом
class StateMachineFree;
class StateMachine
{
public:
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

    bool operator++();

    StateMachine & operator =(const StateMachine & two);

    StateMachine & operator= (const StateMachineFree & one);

    void draw();

    std::vector<Part*>::iterator begin();
    std::vector<Part*>::iterator end();

public:
    std::vector<Part*> _state;
};

#endif // STATEMACHINE_H
