// (c) Copyright Aleksey Sivkov 2017-2018

#include <game_line.hpp>
#include <set>

using namespace jcw;

GameLine::GameLine(size_t cells—ount)
    : Line(cells—ount)
{
}

GameLine::GameLine(Line<GameCell> line)
    : Line{line}
{
}

bool GameLine::setFree()
{
    bool result = false;
    for (auto& cell : cells_)
    {
        if (cell.setFree())
        {
            result = true;
        }
    }
    return result;
}

bool GameLine::setEmpty()
{
    bool result = false;
    for (auto &cell : cells_)
    {
        if (cell.setEmpty())
        {
            result = true;
        }
    }
    return result;
}

bool GameLine::setPainted(ColorIndex color)
{
    bool result = false;
    for (auto &cell : cells_)
    {
        if (cell.setPainted(color))
        {
            result = true;
        }
    }
    return result;
}

bool GameLine::FreeToEmpty()
{
    bool result = false;
    for (auto &cell : cells_)
    {
        if (cell.isFree() && cell.setEmpty())
        {
            result = true;
        }
    }
    return result;
}

size_t GameLine::paintedCellsCount() const
{
    size_t result = 0;
    for (auto &cell : cells_)
    {
        if (cell.isPainted())
        {
            result++;
        }
    }
    return result;
}

size_t GameLine::maxColorsCount() const
{
    std::set<ColorIndex> colorMap;
    for (auto &cell : cells_)
    {
        if (cell.isPainted())
        {
            colorMap.insert(cell.color());
        }
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

GameLine GameLine::subLine(size_t beginCellIndex, size_t endCellIndex) const
{
    return GameLine(Line::subLine(beginCellIndex, endCellIndex));
}

void GameLine::mergeLine(
    GameLine subline, size_t beginCellIndex, size_t endCellIndex)
{
    Line<GameCell> line;
    for (size_t i = beginCellIndex, j = 0; i <= endCellIndex; ++i, ++j)
    {
        cells_.at(i) = subline[j];
    }
}