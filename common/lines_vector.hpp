#pragma once

// (c)Copyright Aleksey Sivkov 2019
/*
     ласс јгрегатора строк (игра или задание).
ќт него унаследованы классы Game и Task.
*/


#include <vector>

namespace jcw
{

template <typename LineType>
class LinesVector : public std::vector<LineType>
{
public:
    LinesVector();
    LinesVector(size_t size);
    LinesVector(const std::vector<LineType> &taskLines);

    size_t maxColorsCount() const;
    void inversion();      // развотот по первой оси (дл€ Game - по вертикали)
    void innerInversion(); // развотот по второй оси (дл€ Game - по горизонтали)
};

template <typename LineType>
LinesVector<LineType>::LinesVector()
{
}

template <typename LineType>
LinesVector<LineType>::LinesVector(size_t size)
    : std::vector<LineType>(size)
{
}

template <typename LineType>
LinesVector<LineType>::LinesVector(const std::vector<LineType> &taskLines)
    : std::vector<LineType>(taskLines)
{
}

template <typename LineType>
size_t LinesVector<LineType>::maxColorsCount() const
{
    size_t colorsCount = 0;
    const auto size = this->size();
    for (size_t i = 0; i < size; ++i)
    {
        const LineType &line = (*this).at(i);
        const auto maxColorsCount = line.maxColorsCount();
        if (maxColorsCount > colorsCount)
        {
            colorsCount = maxColorsCount;
        }
    }
    return colorsCount;
}

template <typename LineType>
void LinesVector<LineType>::inversion()
{
    std::reverse(begin(), end());
}

template <typename LineType>
void LinesVector<LineType>::innerInversion()
{
    const auto size = this->size();
    for (size_t i = 0; i < size; ++i)
    {
        (*this)[i].inversion();
    }
}

} // namespace jcw