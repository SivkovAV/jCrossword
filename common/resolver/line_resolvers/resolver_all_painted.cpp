// (c) Copyright Aleksey Sivkov 2017

#include <resolver_all_painted.hpp>

using namespace jcw;

ResolverAllPainted::ResolverAllPainted()
{
}

std::string ResolverAllPainted::description() const
{
    return "Если все закрашенные клетки найдены - "
        "помечает все остальные клетоки пустыми";
}

bool ResolverAllPainted::isMonohromOnly() const
{
    return false;
}

bool ResolverAllPainted::canResolve() const
{
    return true;
}

bool ResolverAllPainted::canFindSubtask() const
{
    return false;
}

bool ResolverAllPainted::process(GameLine &game, TaskLine &task) const
{
    bool result = false;
    if (task.paintedCellsCount() == game.paintedCellsCount())
    {
        if (game.FreeToEmpty())
        {
            result = true;
        };

        if (task.setChecked(true))
        {
            result = true;
        }
    }
    return result;
}
