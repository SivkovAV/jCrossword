#pragma once

// (c) Copyright Aleksey Sivkov 2017

#include <game_cell.hpp>
#include <line.hpp>

namespace jcw
{

class GameLine : public Line<GameCell>
{
public:
    GameLine(size_t cellsСount = 0);
    GameLine(Line<GameCell> line);

    // следующие методы возвращают true, если хоть одна клетка изменилась
    bool setFree();
    bool setEmpty();
    bool setPainted(ColorIndex color);

    // помечает все свободные клетки как пустые
    bool FreeToEmpty();

    size_t paintedCellsCount() const;

    // возвращает количество цветов, встречающихся на игровой линии
    size_t maxColorsCount() const;

    GameLine subLine(size_t beginCellIndex, size_t endCellIndex) const;
    void mergeLine(GameLine subline, size_t beginCellIndex, size_t endCellIndex);
};

} // namespace jcw
