// (c) Copyright Aleksey Sivkov 2017

#include <abstract_line_resolver.hpp>

using namespace jcw;

AbstractLineResolver::AbstractLineResolver(
    AbstractLineResolverCollector *collector)
    : collector_(collector)
{
}

bool AbstractLineResolver::drawFigure(GameLine& game, TaskLine &task,
    size_t leftGameIndex, size_t rightGameIndex, size_t taskIndex) const
{
    bool result = false;
    if (task.isResolved())
    {
        return result;
    }
    const auto taskColor = task[taskIndex].color();

    // закраска фигуры по переданным размерам
    if (drawFigureOnly(game, taskColor, leftGameIndex, rightGameIndex, taskIndex))
    {
        result = true;
    }

    // ищем левую и правую границы обнаруженной части фигуры
    size_t left = leftGameIndex, right = rightGameIndex;
    findFigureBorders(game, taskIndex, left, right, taskColor);

    // если длина обнаруженной части совпадает с полным размером - нашли целую фигуру!
    const size_t figureLength = right - left + 1u;
    if (figureLength == task[taskIndex].value() &&
        complateFigure(game, task, left, right, taskIndex))
    {
        result = true;
    }
    else
    {
        if (right - left + 1 > task[taskIndex].value())
        {
            throw "найдена фигура некорректного размера";
        }
        ColorIndex color;
        // проверяем, не ограничена ли фигура слева и справа препятствием
        // и не совпадает ли ее граница с текущими (защита от зацикливания)
        if (right + 1 < game.size() &&
            isBlockedCell(game, taskColor, right + 1) &&
            !(right + 1 - task[taskIndex].value() == leftGameIndex &&
                right == rightGameIndex) &&
            drawFigure(game, task, right + 1 - task[taskIndex].value(), right, taskIndex))
        {
            result = true;
        }
        if (left >= 1 &&
            isBlockedCell(game, taskColor, left - 1) &&
            !(left == leftGameIndex &&
                left + task[taskIndex].value() - 1 == rightGameIndex) &&
            drawFigure(game, task, left, left + task[taskIndex].value() - 1, taskIndex))
        {
            result = true;
        }
    }
    return result;
}

bool AbstractLineResolver::externalProcess(
    GameLine &game, size_t beginGameIndex, size_t endGameIndex,
    TaskLine &task, size_t beginTaskIndex, size_t endTaskIndex) const
{
    if (!collector_ || task.isResolved())
    {
        return false;
    }
    GameLine subGame = game.subLine(beginGameIndex, endGameIndex);
    TaskLine subTask = task.subLine(beginTaskIndex, endTaskIndex);
    if (collector_->execute(subGame, subTask))
    {
        game.mergeLine(subGame, beginGameIndex, endGameIndex);
        task.mergeLine(subTask, beginTaskIndex, endTaskIndex);
        return true;
    }
    return false;
}

bool AbstractLineResolver::drawFigureOnly(
    GameLine &game, ColorIndex taskColor,
    size_t beginGameIndex, size_t endGameIndex, size_t taskIndex) const
{
    bool result = false;
    for (size_t i = beginGameIndex; i <= endGameIndex; ++i)
    {
        if (game[i].setPainted(taskColor))
        {
            result = true;
        }
    }
    return result;
}

void AbstractLineResolver::findFigureBorders(
    GameLine &game, ColorIndex taskColor,
    size_t &leftCellIndex, size_t &rightCellIndex, size_t taskIndex) const
{
    int left = static_cast<int>(leftCellIndex);
    int right = static_cast<int>(rightCellIndex);

    ColorIndex color;
    const auto gameSize = game.size();
    while (left - 1 >= 0 && game[left - 1].isPainted(&color) && color == taskColor)
    {
        left--;
    }
    while (right + 1 < gameSize &&
        game[right + 1].isPainted(&color) && color == taskColor)
    {
        right++;
    }

    leftCellIndex = static_cast<size_t>(left);
    rightCellIndex = static_cast<size_t>(right);
}

bool AbstractLineResolver::setTaskCellAsResolved(
    GameLine &game, TaskLine &task,
    size_t taskIndex) const
{
    bool result = false;
    const auto gameSize = game.size();
    if (task[taskIndex].setChecked(true))
    {
        result = true;
        if (task.isResolved())
        {
            for (size_t i = 0; i < gameSize; ++i)
            {
                if (game[i].isFree())
                {
                    game[i].setEmpty();
                    result = true;
                }
            }
        }
    }
    return result;
}

bool AbstractLineResolver::fillLeftSizeCellsAsEmpty(
    GameLine &game, size_t leftBorder) const
{
    bool result = false;
    for (size_t i = 0; i < leftBorder; ++i)
    {
        if (game[i].setEmpty())
        {
            result = true;
        }
    }
    return result;
}

bool AbstractLineResolver::fillRightSizeCellsAsEmpty(
    GameLine &game, size_t rightBorder) const
{
    bool result = false;
    for (size_t i = rightBorder + 1; i < game.size(); ++i)
    {
        if (game[i].setEmpty())
        {
            result = true;
        }
    }
    return result;
}

bool AbstractLineResolver::complateFigure(GameLine &game, TaskLine &task,
    size_t left, size_t right, size_t taskIndex) const
{
    const auto color = task[taskIndex].color();

    // отмечаем факт того, что фигура найдена
    bool result = setTaskCellAsResolved(game, task, taskIndex);

    // смотрим, если ли что левее; если есть - пытаемся отметить границу и
    // запускаем алгоритмы решения для левой подстроки
    // иначе заполняем все справа крестами
    if (taskIndex == 0)
    {
        if (fillLeftSizeCellsAsEmpty(game, left))
        {
            result = true;
        }
    }
    else
    {
        // отмечаем границу
        if (task[taskIndex - 1].color() == color && game[left - 1].setEmpty())
        {
            result = true;
        }
        // запуск решения для левой подзадачи
        while (left >= 1 && game[left - 1].isEmpty())
        {
            left--;
        }
        if (left >= 1)
        {
            if (externalProcess(game, 0, left - 1, task, 0, taskIndex - 1))
            {
                result = true;
            }
        }
    }

    // смотрим, если ли что правее; если нет - пытаемся отметить границу и
    // запускаем алгоритмы решения для правой подстроки
    if (taskIndex == task.size() - 1 && fillRightSizeCellsAsEmpty(game, right))
    {
        result = true;
    }
    else
    {
        // отмечаем границу
        if (task[taskIndex + 1].color() == color && game[right + 1].setEmpty())
        {
            result = true;
        }
        // запуск решения для правой подзадачи
        while (right + 1 < game.size() && game[right + 1].isEmpty())
        {
            right++;
        }
        if (right + 1 < game.size())
        {
            if (externalProcess(
                game, right + 1, game.size() - 1,
                task, taskIndex + 1, task.size() - 1))
            {
                result = true;
            }
        }
    }
    return result;
}

bool AbstractLineResolver::isBlockedCell(
    const GameLine &game, ColorIndex taskColor, size_t gameCellIndex) const
{
    if (game[gameCellIndex].isEmpty())
    {
        return true;
    }
    ColorIndex color;
    return game[gameCellIndex].isPainted(&color) && taskColor != color;
}