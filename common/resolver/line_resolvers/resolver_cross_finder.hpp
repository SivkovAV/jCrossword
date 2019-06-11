#pragma once

// (c) Copyright Aleksei Sivkov 2017

#include <abstract_line_resolver.hpp>

namespace jcw
{

    class ResolverCrossFinder : public AbstractLineResolver
    {
    public:
        ResolverCrossFinder(AbstractLineResolverCollector *collector = nullptr);
        std::string description() const override;
        bool isMonohromOnly() const override;
        bool canResolve() const override;
        bool canFindSubtask() const override;
        bool process(GameLine &game, TaskLine &task) const override;
    };

} // namespace jcw