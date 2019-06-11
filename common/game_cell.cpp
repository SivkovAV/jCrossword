// (c) Copyright Aleksey Sivkov 2017

#include <game_cell.hpp>
#include <exception>

using namespace jcw;

GameCell::GameCell()
    : status_(GameCellStatus::free)
    , color_(0)
{
}

bool GameCell::isPainted(ColorIndex *color) const
{
    if (color != nullptr && status_ == GameCellStatus::painted)
    {
        *color = color_;
    }
    return (status_ == GameCellStatus::painted);
}

bool GameCell::isFree() const
{
    return (status_ == GameCellStatus::free);
}

bool GameCell::isEmpty() const
{
    return (status_ == GameCellStatus::empty);
}

bool GameCell::isEqual(const GameCell &cell) const
{
    return (cell.color_ == color_ && cell.status_ == status_);
}

bool GameCell::setFree()
{
    if (status_ == GameCellStatus::free)
    {
        return false;
    }
    status_ = GameCellStatus::free;
    return true;
}

bool GameCell::setEmpty(bool noThrow)
{
    if (status_ == GameCellStatus::empty)
    {
        return false;
    }
    if (status_ == GameCellStatus::painted && noThrow == false)
    {
        throw std::exception("cell modification");
    }
    status_ = GameCellStatus::empty;
    return true;
}

bool GameCell::setPainted(ColorIndex color, bool noThrow)
{
    if (status_ == GameCellStatus::painted && color_ == color)
    {
        return false;
    }
    if (status_ == GameCellStatus::empty && noThrow == false)
    {
        throw std::exception("cell modification");
    }
    color_ = color;
    status_ = GameCellStatus::painted;
    return true;
}

ColorIndex GameCell::color() const
{
    if (status_ == GameCellStatus::painted)
    {
        return color_;
    }
    return ColorIndex();
}