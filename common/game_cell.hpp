#pragma once

// (c) Copyright Aleksey Sivkov 2017
/*
    ����� ��������� ������ ������ �������� ����.
*/

#include <color_index.hpp>

namespace jcw
{
// ��������� ������
enum GameCellStatus
{
    empty,    // ������ ��������, ��� ������ (X)
    painted,  // ������ ���������
    free,     // ������ �������� (�� empty � �� painted)
};

class GameCell
{
public:
    GameCell();

    bool isPainted(ColorIndex *color = nullptr) const;
    bool isFree() const;
    bool isEmpty() const;
    bool isEqual(const GameCell &cell) const;
     
    // ��������� ������ ���������� ������� - ���������� �� ������
    bool setFree();                                                            
    bool setEmpty(bool noThrow = false);                                    
    bool setPainted(ColorIndex color = ColorIndex(), bool noThrow = false);
    ColorIndex color() const;
    
private:
    GameCellStatus status_ = GameCellStatus::free;
    ColorIndex color_ = (ColorIndex)0u;
};

} // namespace jcw
