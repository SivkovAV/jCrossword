// (c) Copyright Aleksey Sivkov 2017

#include <resolver_fill_not_painted.hpp>

using namespace jcw;

ResolverFillNotPainted::ResolverFillNotPainted()
{
}

std::string ResolverFillNotPainted::description() const
{
    // �������� ResolverSingleVariant ����� ���������� ������� ������� �������,
    // �� ��� �� ���; ������ �������� �� ��������� ������� ������ ������ �����
    // ��������� �������� ����������� �����
    return "���������� ���� �������� ������, ���� �� ���-�� ����� ���������� "
        "������ � ������� �� ������� ��� �����������";
}

bool ResolverFillNotPainted::isMonohromOnly() const
{
    return false;
}

bool ResolverFillNotPainted::canResolve() const
{
    return true;
}

bool ResolverFillNotPainted::canFindSubtask() const
{
    return false;
}

bool ResolverFillNotPainted::process(GameLine &game, TaskLine &task) const
{
    if (task.size() == 0)
    {
        return false;
    }

    const auto gameSize = game.size();
    const auto taskSize = task.size();

    // ������� ���-�� ������, ������� ����� ���������
    int needPaintedCellsCount = 0;
    for (size_t j = 0; j < taskSize; ++j)
        needPaintedCellsCount += task[j].value();

    // ������� ���-�� ������, ���������� ��� ������
    int emptyCellsCount = 0;
    for (size_t i = 0; i < gameSize; ++i)
    {
        if (game[i].isEmpty())
        {
            ++emptyCellsCount;
        }
    }

    if (needPaintedCellsCount != gameSize - emptyCellsCount)
    {
        return false;
    }

    bool result = false;
    int taskCellNumber = 0;
    int paintedCellCcount = 0;
    for (size_t i = 0; i < gameSize; ++i)
    {
        if (game[i].isEmpty() == false)
        {
            if (game[i].setPainted(task[taskCellNumber].color()))
            {
                result = true;
            }
            ++paintedCellCcount;
            if (paintedCellCcount == task[taskCellNumber].value())
            {
                paintedCellCcount = 0;
                ++taskCellNumber;
            }
        }
    }
    for (size_t j = 0; j < taskSize; ++j)
    {
        if (task[j].setChecked(true))
        {
            result = true;
        }
    }

    return result;
}