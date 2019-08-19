// (c)Copyright Aleksey Sivkov 2018

#include <task.hpp>

using namespace jcw;

Task::Task(size_t size)
    : LinesVector<TaskLine>(size)
{
}

Task::Task(const std::vector<TaskLine> &taskLines)
    : LinesVector<TaskLine>(taskLines)
{
}

bool Task::isEqual(const Task &task) const
{
    // аналог operator==, но без учета isChecked
    const auto size = this->size();
    if (task.size() != size)
    {
        return false;
    }

    for (size_t i = 0; i < size; ++i)
    {
        const auto innerSize = (*this)[i].size();
        if (task[i].size() != innerSize)
        {
            return false;
        }

        for (size_t j = 0; j < innerSize; ++j)
        {
            auto &ownCell = (*this)[i][j];
            auto &otherCell = task[i][j];
            if (otherCell.color() != ownCell.color() ||
                otherCell.value() != ownCell.value())
            {
                return false;
            }
        }
    }

    return true;
}

std::map<ColorIndex, size_t> Task::cellsCountMap() const
{
    std::map<ColorIndex, size_t> taskCellsCount;
    for (const auto &taskLine : *this)
    {
        const auto cellsCount = taskLine.size();
        for (size_t i = 0; i < cellsCount; ++i)
        {
            const auto &cell = taskLine[i];
            taskCellsCount[cell.color()] += cell.value();
        }
    }
    return taskCellsCount;
}

bool Task::isResolved() const
{
    for (auto& line : *this)
    {
        if (line.isResolved() == false)
        {
            return false;
        }
    }
    return true;
}

size_t Task::maxLenght() const
{
    size_t lenght = 0;

    if (size() > 0)
    {
        lenght = (*this)[0].size();
        for (auto &taskLine : (*this))
        {
            if (lenght < taskLine.size())
            {
                lenght = taskLine.size();
            }
        }
    }
    return lenght;
}

size_t Task::maxValue() const
{
    size_t maxValue = 0;
    for (const auto &taskLine : *this)
    {
        const auto localMaxValue = taskLine.maxValue();
        if (maxValue < localMaxValue)
        {
            maxValue = localMaxValue;
        }
    }
    return maxValue;
}

bool Task::setChecked(bool isChecked)
{
    bool result = false;

    for (auto &taskLine : *this)
    {
        if (taskLine.setChecked(isChecked))
        {
            result = true;
        }
    }
    return result;
}


Task Task::subTask(size_t verticalIndex, size_t verticalLength) const
{
    return subVector(*this, verticalIndex, verticalLength);
}

Task Task::subTask(size_t verticalIndex, size_t verticalLength,
    size_t horizontalIndex, size_t horizontalLength) const
{
    const auto task = subTask(verticalIndex, verticalLength);
    return subVector(task, horizontalIndex, horizontalLength);
}

Task  const Task::leftTaskByGame(const Game &game)
{
    const auto height = game.size();
    Task resultTask(height);
    for (size_t i = 0; i < height; ++i)
    {
        resultTask[i] = TaskLine::getTaskByGame(game[i]);
    }
    return resultTask;
}

Task const Task::topTaskByGame(const Game &game)
{
    return leftTaskByGame(game.getTransposed());
}