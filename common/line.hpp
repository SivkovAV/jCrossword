#pragma once

// (c) Copyright Aleksey Sivkov 2017

#include <vector>
#include <algorithm>

namespace jcw
{

template <typename CellType>
class Line
{
public:
    Line(size_t cellsСount = 0);
    CellType& operator[](size_t cellIndex);
    CellType operator[](size_t cellIndex) const;
    bool operator==(const Line &line) const;
    bool operator!=(const Line &line) const;
    void clear();
    void resize(unsigned newSize);
    void addCells(size_t cellsСount = 1);   // добавляет клетки в конец 
    void addCell(CellType cell);            // добавляет клетку в конец 
    size_t size() const;                    
    void inversion();                       // разворачивает линию
    Line subLine(size_t beginCellIndex, size_t endCellIndex) const;
    void mergeLine(Line subline, size_t beginCellIndex, size_t endCellIndex);

protected:
    std::vector<CellType> cells_;
};

template <typename CellType>
Line<CellType>::Line(size_t cellsСount)
{
    addCells(cellsСount);
}

template <typename CellType>
CellType& Line<CellType>::operator[](size_t cellIndex)
{
    if (cellIndex < 0 || cellIndex >= cells_.size())
    {
        throw std::exception("index out of range");
    }
    return cells_[cellIndex];
}

template <typename CellType>
CellType Line<CellType>::operator[](size_t cellIndex) const
{
    if (cellIndex < 0 || cellIndex >= cells_.size())
    {
        throw std::exception("index out of range");
    }
    return cells_[cellIndex];
}

template <typename CellType>
bool Line<CellType>::operator == (const Line &line) const
{
    if (line.cells_.size() != cells_.size())
    {
        return false;
    }

    const auto size = cells_.size();
    for (size_t i = 0; i < size; i++)
    {
        if (line.cells_[i].isEqual(cells_[i]) == false)
        {
            return false;
        }
    }

    return true;
}

template <typename CellType>
bool Line<CellType>::operator != (const Line &line) const
{
    return !(*this == line);
}

template <typename CellType>
void Line<CellType>::clear()
{
    cells_.clear();
}

template <typename CellType>
void Line<CellType>::resize(unsigned newSize)
{
    cells_.resize(newSize);
}

template <typename CellType>
void Line<CellType>::addCells(size_t cellsСount)
{
    CellType cell;
    cells_.reserve(cellsСount);
    for (size_t i = 0; i < cellsСount; i++)
    {
        cells_.emplace_back(cell);
    }
}

template <typename CellType>
void Line<CellType>::addCell(CellType cell)
{
    cells_.push_back(cell);
};

template <typename CellType>
size_t Line<CellType>::size() const
{
    return cells_.size();
}

template <typename CellType>
void Line<CellType>::inversion()
{
    std::reverse(cells_.begin(), cells_.end());
}

template <typename CellType>
Line<CellType> Line<CellType>::subLine(
    size_t beginCellIndex, size_t endCellIndex) const
{
    if (endCellIndex < beginCellIndex)
    {
        return Line<CellType>();
    }
    const auto size = endCellIndex - beginCellIndex + 1;
    Line<CellType> line(size);
    for (size_t i = 0; i < size; ++i)
    {
        line[i] = cells_[beginCellIndex + i];
    }
    return line;
}

template <typename CellType>
void Line<CellType>::mergeLine(Line subline,
    size_t beginCellIndex, size_t endCellIndex)
{
    if (beginCellIndex >= cells_.size())
    {
        return;
    }
   
    const auto endIndex = std::min(cells_.size(), endCellIndex);
    for (auto i = beginCellIndex; i <= endIndex; ++i)
    {
        cells_[i] = subline[i - beginCellIndex];
    }
}

} // namespace jcw