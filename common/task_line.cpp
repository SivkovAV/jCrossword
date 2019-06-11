// (c) Copyright Aleksey Sivkov 2017

#include <task_line.hpp>
#include <set>

using namespace jcw;

TaskLine::TaskLine(size_t cells—ount)
    : Line(cells—ount)
{
}

TaskLine::TaskLine(Line<TaskCell> &line)
    : Line(line)
{
}

TaskLine TaskLine::getTaskByGame(const GameLine &gameLine)
{
    TaskLine taskLine;
    size_t currentTaskLineIndex = -1;
    bool isLineFinded = false;
    ColorIndex color;
    const auto size = gameLine.size();
    for (size_t i = 0; i < size; i++)
    {
        if (gameLine[i].isPainted() == false || color != gameLine[i].color())
        {
            isLineFinded = false;
        }
        if (gameLine[i].isPainted())
        {
            if (isLineFinded == false)
            {
                color = gameLine[i].color();
                isLineFinded = true;
                currentTaskLineIndex++;
                taskLine.addCells();
                taskLine[currentTaskLineIndex].setValue(1);
                taskLine[currentTaskLineIndex].setColor(color);
            }
            else
            {
                taskLine[currentTaskLineIndex].inc();
            }
        }
    }
    return taskLine;
}

bool TaskLine::isSolution(GameLine &gameLine) const
{
    return (TaskLine::getTaskByGame(gameLine) == (*this));
}

bool TaskLine::isResolved() const
{
    const auto cellsCount = cells_.size();
    for (size_t j = 0; j < cellsCount; ++j)
    {
        if (cells_[j].isChecked() == false)
        {
            return false;
        }
    }
    return true;
}

bool TaskLine::isResolved(const GameLine &game) const
{
    const auto task = TaskLine::getTaskByGame(game);
    if (task != *this)
    {
        return false;
    }
    return true;
}

size_t TaskLine::maxValue() const
{
    const auto size = cells_.size();
    size_t maxValue = 0;
    for (size_t i = 0; i < size; i++)
    {
        if (maxValue < cells_[i].value())
        {
            maxValue = cells_[i].value();
        }
    };
    return maxValue;
}

size_t TaskLine::paintedCellsCount() const
{
    const auto size = cells_.size();
    size_t count = 0;
    for (size_t i = 0; i < size; i++)
    {
        count += cells_[i].value();
    };
    return count;
}

size_t TaskLine::emptyCellsCount() const
{
    const auto size = cells_.size();
    size_t count = 0;
    for (size_t i = 0; i < size; i++)
    {
        if (i + 1 < size && cells_[i].color() == cells_[i + 1].color())
        {
            count++;
        }
    };
    return count;
}

size_t TaskLine::minimumGameCellsCount() const
{
    return paintedCellsCount() + emptyCellsCount();
}

bool TaskLine::fillGameLine(GameLine &game, size_t firstCellFromLeft)
{
    if (game.size() - firstCellFromLeft < minimumGameCellsCount())
    {
        throw("too smole game line");
    }

    bool result = false;

    size_t currentCellIndex = firstCellFromLeft;
    for (size_t i = 0; i < this->size(); ++i)
    {
        for (size_t j = 0; j < cells_[i].value(); ++j)
        {
            if (game[currentCellIndex].setPainted(cells_[i].color()))
            {
                result = true;
            }
            currentCellIndex++;
        }
        if (i + 1 <  this->size() && cells_[i].color() == cells_[i + 1].color())
        {
            if (game[currentCellIndex].setEmpty())
            {
                result = true;
            }
            currentCellIndex++;
        }
        if (cells_[i].setChecked(true))
        {
            result = true;
        }
    }

    return result;
}

bool TaskLine::setChecked(bool isChecked)
{
    bool result = false;
    for (auto& cell : cells_)
    {
        if (cell.setChecked(isChecked))
        {
            result = true;
        }
    }
    return result;
}

bool TaskLine::setChecked(size_t cellIndex, bool isChecked)
{
    if (cellIndex > cells_.size())
    {
        throw "wrone cell index";
    }
    bool result = false;
    if (cells_[cellIndex].setChecked(isChecked))
    {
        result = true;
    }
    return result;
}

bool TaskLine::isMonohrome() const
{
    const auto size = cells_.size();
    if (size <= 1)
    {
        return true;
    }
    ColorIndex color = cells_[0].color();
    for (size_t i = 1; i < size; ++i)
    {
        if (cells_[i].color() != color)
        {
            return false;
        }
    }
    return true;
}

size_t TaskLine::maxColorsCount() const
{
    std::set<ColorIndex> colorMap;
    for (auto &cell : cells_)
    {
        colorMap.insert(cell.color());
    }
    if (colorMap.size() == 0u)
    {
        return 0u;
    }
    size_t colorsCount = 0;
    for (auto &mapItem : colorMap)
    {
        if (mapItem > colorsCount)
        {
            colorsCount = mapItem;
        }
    }
    return colorsCount + 1u;
}