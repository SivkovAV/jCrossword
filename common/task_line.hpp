#pragma once

// (c) Copyright Aleksey Sivkov 2017

#include <task_cell.hpp>
#include <game_line.hpp>

namespace jcw
{

class TaskLine : public Line<TaskCell>
{
public:
    TaskLine(size_t cells�ount = 0);
    TaskLine::TaskLine(Line<TaskCell> &line);

    static TaskLine getTaskByGame(const GameLine &gameLine);
    bool isSolution(GameLine &gameLine) const;
    bool isResolved() const;
    bool isResolved(const GameLine &game) const;
    // ���������� ����� ������� ����� ������� � ������
    size_t maxValue() const;
    // ���������� ���������� ������, ������� ������ ���� ���������
    size_t paintedCellsCount() const;
    // ���������� ���������� �������������� ������ ������
    size_t emptyCellsCount() const;
    // ���������� ����������� ���-�� ������, ������ ��� ��������� �������
    size_t minimumGameCellsCount() const;
    // ��������� ������� ����� ��������, ������������ ����������� ���������;
    // ������ ���, ������� ����� � ������ � firstCellFromLeft
    bool fillGameLine(GameLine &game, size_t firstCellFromLeft = 0);
    bool setChecked(bool isChecked);
    bool setChecked(size_t cellIndex, bool isChecked);
    bool isMonohrome() const;
    size_t maxColorsCount() const;
};

} // namespace jcw
