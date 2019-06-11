#pragma once

// (c) Copyright Aleksey Sivkov 2017

#include <task_cell.hpp>
#include <game_line.hpp>

namespace jcw
{

class TaskLine : public Line<TaskCell>
{
public:
    TaskLine(size_t cellsСount = 0);
    TaskLine::TaskLine(Line<TaskCell> &line);

    static TaskLine getTaskByGame(const GameLine &gameLine);
    bool isSolution(GameLine &gameLine) const;
    bool isResolved() const;
    bool isResolved(const GameLine &game) const;
    // возвращает самое большое число задания в строке
    size_t maxValue() const;
    // возвращает количество клеток, которые должны быть закрашены
    size_t paintedCellsCount() const;
    // возвращает количество гарантированно пустых клеток
    size_t emptyCellsCount() const;
    // возвращает минимальное кол-во клеток, нужное для отрисовки задания
    size_t minimumGameCellsCount() const;
    // заполняет игровую линию решением, нарисованным максимально компактно;
    // делает это, начиная слева с клетки № firstCellFromLeft
    bool fillGameLine(GameLine &game, size_t firstCellFromLeft = 0);
    bool setChecked(bool isChecked);
    bool setChecked(size_t cellIndex, bool isChecked);
    bool isMonohrome() const;
    size_t maxColorsCount() const;
};

} // namespace jcw
