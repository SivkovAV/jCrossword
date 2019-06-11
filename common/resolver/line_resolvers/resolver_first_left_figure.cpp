// (c) Copyright Aleksey Sivkov 2017

#include <resolver_first_left_figure.hpp>

using namespace jcw;

ResolverFirstLeftFigure::ResolverFirstLeftFigure(
    AbstractLineResolverCollector *collector)
    : AbstractLineResolver(collector)
{
}

std::string ResolverFirstLeftFigure::description() const
{
    return "���������� ������ ����� ������:\0"
    "���� ������� ����� ��������� ��������� ������, ������ � ����� ������� "
    "    ������ ������ �� ������ - ��� ������ ���������� ��� ������.\0"
    "���� ��������� ����������� ������, ����� ����������� � ������ ������ - "
    "    ������ ���� ������ ������������� ��� ���������, ����� ����������� � "
    "    ������ ������";
}

bool ResolverFirstLeftFigure::isMonohromOnly() const
{
    return false;
}

bool ResolverFirstLeftFigure::canResolve() const
{
    return true;
}

bool ResolverFirstLeftFigure::canFindSubtask() const
{
    return true;
}

bool ResolverFirstLeftFigure::process(GameLine &game, TaskLine &task) const
{
    bool result = false;
    const auto length = task[0].value();
    size_t i = 0, shift = 0;
    while (i < length)
    {
        // ���������� ������ ����� ������ ������
        if (game[i + shift].isEmpty())
        {
            shift += i + 1; i = 0;
            for (size_t j = 0; j < shift; ++j)
            {
                if (game[j].setEmpty())
                {
                    result = true;
                }
            }
        }
        else
        {
            if (game[i + shift].isPainted())
            {
                if (drawFigure(game, task, i + shift + 1, length + shift - 1, 0))
                {
                    result = true;
                }
                return result;
            }
            i++;
        }
    }

    return result;
}
