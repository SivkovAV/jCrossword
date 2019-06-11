// (c) Copyright Aleksey Sivkov 2017

#include <resolver_single_variant.hpp>

using namespace jcw;

ResolverSingleVariant::ResolverSingleVariant()
{
}

std::string ResolverSingleVariant::description() const
{
    return "Если задание занимает все имеющиеся клетки - заполняем линию";
}

bool ResolverSingleVariant::isMonohromOnly() const
{
    return false;
}

bool ResolverSingleVariant::canResolve() const
{
    return true;
}

bool ResolverSingleVariant::canFindSubtask() const
{
    return false;
}

bool ResolverSingleVariant::process(GameLine &game, TaskLine &task) const
{
    bool result = false;
    const auto taskSize = task.size();
    const auto lineSize = game.size();

    if (task.minimumGameCellsCount() == lineSize)
    {
        if (task.fillGameLine(game))
        {
            result = true;
        }
        if (task.setChecked(true))
        {
            result = true;
        }
    }
    return result;
}
