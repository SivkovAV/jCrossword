#pragma once

// (c) Copyright Aleksei Sivkov 2017

#include <abstract_line_resolver.hpp>

namespace jcw
{

// если задание занимает все имеющиеся клетки - заполняем линию
class ResolverSingleVariant : public AbstractLineResolver
{
public:
    ResolverSingleVariant();
    std::string description() const override;
    bool isMonohromOnly() const override;
    bool canResolve() const override;
    bool canFindSubtask() const override;
    bool process(GameLine &game, TaskLine &task) const override;
};

} // namespace jcw