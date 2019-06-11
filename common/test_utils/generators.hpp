#pragma once

// (c) Copyright Aleksey Sivkov 2017-2018

#include <game.hpp>
#include <task.hpp>
#include <Palette.hpp>
#include <crossword.hpp>

namespace jcw
{
    
class RandomGenegator
{
public:
    static Game game(size_t height, size_t width, const Palette &palette = Palette());
    static Task leftTask(size_t height, size_t width, const Palette &palette = Palette());
    static Task topTask(size_t width, size_t height, const Palette &palette = Palette());
};

class TestGenegator
{
public:
    static GameLine testGameLine();
    static Crossword testCrossword();
};

} // namespace jcw