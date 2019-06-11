#pragma once

// (c) Copyright Aleksey Sivkov 2017
/*
     ласс реализует объект клетки игрового пол€.
*/

#include <color_index.hpp>

namespace jcw
{
// состо€ние клетки
enum GameCellStatus
{
    empty,    // клетка помечена, как пуста€ (X)
    painted,  // клетка закрашена
    free,     // клетка свободна (не empty и не painted)
};

class GameCell
{
public:
    GameCell();

    bool isPainted(ColorIndex *color = nullptr) const;
    bool isFree() const;
    bool isEmpty() const;
    bool isEqual(const GameCell &cell) const;
     
    // следующие методы возвращают признак - изменилась ли клетка
    bool setFree();                                                            
    bool setEmpty(bool noThrow = false);                                    
    bool setPainted(ColorIndex color = ColorIndex(), bool noThrow = false);
    ColorIndex color() const;
    
private:
    GameCellStatus status_ = GameCellStatus::free;
    ColorIndex color_ = (ColorIndex)0u;
};

} // namespace jcw
