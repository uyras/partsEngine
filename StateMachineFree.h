#ifndef STATEMACHINEFREE_H
#define STATEMACHINEFREE_H
#include <vector>
#include <iostream>
#include "StateMachine.h"

class StateMachine;
/**
 * @brief The StateMachineFree class представляет собой класс, не привязанный к какой-либо системе.
 * Поддерживает стандартное представление StateMachine и умеет копировать из него состояния
 */
class StateMachineFree
{
public:
    StateMachineFree();

    StateMachineFree & operator= (const StateMachine & one);

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

    std::vector<bool> _state;

    void draw();

    void reset();
};

#endif // STATEMACHINEFREE_H
