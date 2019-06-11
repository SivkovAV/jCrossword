// (c) Copyright Aleksey Sivkov 2017

#include <resolver_cross_finder.hpp>

using namespace jcw;

ResolverCrossFinder::ResolverCrossFinder(
    AbstractLineResolverCollector *collector)
    : AbstractLineResolver(collector)
{
}

std::string ResolverCrossFinder::description() const
{
    return "закрашивание областей, гарантированно занятых фигурами task[i]";
}

bool ResolverCrossFinder::isMonohromOnly() const
{
    return false;
}

bool ResolverCrossFinder::canResolve() const
{
    return true;
}

bool ResolverCrossFinder::canFindSubtask() const
{
    return true;
}

bool ResolverCrossFinder::process(GameLine &game, TaskLine &task) const
{
    const auto gameSize = game.size();
    const auto taskSize = task.size();
    std::vector<size_t> lastIndex(taskSize);
    std::vector<size_t> firstIndex(taskSize);

    // ищем "самые правые" границы для всех фигур задания
    size_t beginPos = 0;
    for (size_t i = 0; i < taskSize; ++i)
    {
        bool isFinded = false;
        size_t shift = 0;
        while (!isFinded && beginPos + shift < gameSize &&
            shift <= task[i].value())
        {
            if (game[beginPos + shift].isEmpty())
            {
                if (shift == task[i].value())
                {
                    isFinded = true;
                }
                else
                {
                    beginPos += shift + 1;
                    shift = 0;
                }
            }
            else if (game[beginPos + shift].isFree())
            {
                ++shift;
            }
            else
            {
                isFinded = true;
            }
        }
        lastIndex[i] = beginPos + task[i].value() - 1;
        beginPos += task[i].value();
        if (i + 1 < taskSize && task[i + 1].color() == task[i].color())
        {
            ++beginPos;
        }
    }

    // ищем "самые левые" границы для всех фигур задания
    int64_t endPos = gameSize - 1;
    for (size_t i = taskSize - 1; i != UINT_FAST32_MAX; --i)
    {
        bool isFinded = false;
        int64_t shift = 0;
        while (!isFinded && endPos - shift >= 0 && shift <= task[i].value())
        {
            if (game[endPos - shift].isEmpty())
            {
                if (shift == task[i].value())
                {
                    isFinded = true;
                }
                else
                {
                    endPos -= shift + 1;
                    shift = 0;
                }
            }
            else if (game[endPos - shift].isFree())
            {
                ++shift;
            }
            else
            {
                isFinded = true;
            }
        }
        firstIndex[i] = endPos - (task[i].value() - 1);
        endPos -= task[i].value();
        if (i >= 1 && task[i - 1].color() == task[i].color())
        {
            --endPos;
        }
    }

    // закрашиваем пересечения, если таковые имеются
    auto result = false;
    for (size_t i = 0; i < taskSize; ++i)
    {
        if (firstIndex[i] <= lastIndex[i] &&
            drawFigure(game, task, firstIndex[i], lastIndex[i], i))
        {
            result = true;
        }
    }

    return result;
}