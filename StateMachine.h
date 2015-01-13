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
     * @brief reset сбросить состояние в начальное, т.е. все состояния по нулям
     */
    void reset();

    /**
     * @brief hardReset Жестко обнуляет состояния частиц, не трогая их намагниченности.
     * Функция делает вид, что система только что сгенерировалась в таком состоянии.
     */
    void hardReset();

    /**
     * @brief randomize Переворачивает несколько случайных частиц
     * @param count Количество частиц, которые должны быть перевернуты
     * @return если перевернуть одну частицу, возвращает ее номер, иначе count. При ошибке возвращает -1.
     */
    int randomize(int count=1);

    void setSystem(PartArray*);

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

    StateMachine & operator= (const StateMachineFree & one);
    bool operator==(const StateMachineFree & one);
    bool operator==(const StateMachine & one);

    void draw();

    std::vector<Part*>::iterator begin();
    std::vector<Part*>::iterator end();

    std::vector<Part*> _state;
};

#endif // STATEMACHINE_H
