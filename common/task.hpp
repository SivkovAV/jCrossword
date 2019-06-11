#pragma once

// (c)Copyright Aleksey Sivkov 2018

#include <task_line.hpp>
#include <game.hpp>
#include <map>
#include <lines_vector.hpp>

namespace jcw
{

class Task : public LinesVector<TaskLine>
{
public:
    Task(size_t size = 0);
    Task(const std::vector<TaskLine> &taskLines);

    bool isEqual(const Task &task) const;
    std::map<ColorIndex, size_t> cellsCountMap() const;
    bool isResolved() const;
    size_t maxLenght() const;
    size_t maxValue() const;
    bool setChecked(bool isChecked);
    Task subTask(size_t verticalIndex, size_t verticalLength) const;
    Task subTask(size_t verticalIndex, size_t verticalLength,
        size_t horizontalIndex, size_t horizontalLength) const;
    static const Task leftTaskByGame(const Game &game);
    static const Task topTaskByGame(const Game &game);

private:
    template<typename T>
    std::vector<T> subVector(const std::vector<T> &srcVector,
        size_t firstIndex, size_t length) const;
};

template<typename T>
std::vector<T> Task::subVector(const std::vector<T> &srcVector,
    size_t firstIndex, size_t length) const
{
    const auto size = srcVector.size();
    if (firstIndex >= size)
    {
        return std::vector<T>();
    }

    if (firstIndex + length > size)
    {
        length = size - firstIndex;
    }
    if (length == 0)
    {
        return std::vector<T>();
    }

    std::vector<T> dstVector(length);
    for (size_t i = 0; i < length; ++i)
    {
        dstVector[i] = srcVector[firstIndex + i];
    }
    return dstVector;
};

} // namespace jcw