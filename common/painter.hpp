#pragma once

// (c) Copyright Aleksey Sivkov 2017
/*
    ����� �������� ������� �����, ���������� ��� ����� ������� � png-���� (Qt).
������ ��� ������� ���������� (����� � ���� ������������� ��������).
Todo: ���� ������ ������� ��� ������������� ������ ����� ����������
*/

#include <crossword.hpp>
#include <palette.hpp>

namespace jcw
{

class Painter
{
public:
    Painter();

    static std::string draw(const GameLine &game, const TaskLine &task,
        std::string filename = "");
    static void draw(const Crossword &crossword, std::string filename = "");
    static void draw(const Palette &palette, std::string filename = "");

    static const size_t borderSize = 1u;
    static const size_t cellSize = 20u;
    static const size_t meanCellSize = cellSize + borderSize;
    static const size_t groupCellsCount = 5u;

private:
    static void checkFilename(std::string &filename);
};

} // namespace jcw