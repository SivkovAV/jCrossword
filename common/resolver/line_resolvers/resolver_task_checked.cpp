// (c) Copyright Aleksey Sivkov 2017

#include <resolver_task_checked.hpp>

using namespace jcw;

ResolverAllTaskChecked::ResolverAllTaskChecked()
{
}

std::string ResolverAllTaskChecked::description() const
{
    return "Если все задание помечено - отмечаем свободные клетки крестами";
}

bool ResolverAllTaskChecked::isMonohromOnly() const
{
    return false;
}

bool ResolverAllTaskChecked::canResolve() const
{
    return true;
}

bool ResolverAllTaskChecked::canFindSubtask() const
{
    return false;
}

bool ResolverAllTaskChecked::process(GameLine &game, TaskLine &task) const
{
    const auto taskSize = task.size();
    const auto gameSize = game.size();
    bool result = false;
    if (task.isResolved())
    {
        for (size_t i = 0; i < gameSize; ++i)
        {
            if (game[i].isFree())
            {
                game[i].setEmpty();
                result = true;
            }
        }
    }
    return result;
}
