// (c) Copyright Aleksey Sivkov 2017

#include <resolver_no_tasks.hpp>

using namespace jcw;

ResolverNoTask::ResolverNoTask()
{
}

std::string ResolverNoTask::description() const
{
    return "Если задание пустое (отсутствует) - помечает все клетки пустыми";
}

bool ResolverNoTask::isMonohromOnly() const
{
    return false;
}

bool ResolverNoTask::canResolve() const
{
    return true;
}

bool ResolverNoTask::canFindSubtask() const
{
    return false;
}

bool ResolverNoTask::process(GameLine &game, TaskLine &task) const
{
    bool result = false;
    const auto lineSize = game.size();

    // если задание пустое
    if (task.size() == 0)
    {
        if (game.setEmpty())
        {
            result = true;
        }
    }

    return result;
}