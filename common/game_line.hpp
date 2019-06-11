#pragma once

// (c) Copyright Aleksey Sivkov 2017

#include <game_cell.hpp>
#include <line.hpp>

namespace jcw
{

class GameLine : public Line<GameCell>
{
public:
    GameLine(size_t cells�ount = 0);
    GameLine(Line<GameCell> line);

    // ��������� ������ ���������� true, ���� ���� ���� ������ ����������
    bool setFree();
    bool setEmpty();
    bool setPainted(ColorIndex color);

    // �������� ��� ��������� ������ ��� ������
    bool FreeToEmpty();

    size_t paintedCellsCount() const;

    // ���������� ���������� ������, ������������� �� ������� �����
    size_t maxColorsCount() const;

    GameLine subLine(size_t beginCellIndex, size_t endCellIndex) const;
    void mergeLine(GameLine subline, size_t beginCellIndex, size_t endCellIndex);
};

} // namespace jcw
