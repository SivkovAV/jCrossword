// (c) Copyright Aleksey Sivkov 2017-2018

#include <parametric_crossword.hpp>

using namespace jcw;

ParametricCrossword::ParametricCrossword()
    : Crossword(Crossword())
{
}

ParametricCrossword::ParametricCrossword(const Crossword &crossword)
    : Crossword(crossword)
    , properties(FileProperties())
    , palette(Palette())
{
}

ParametricCrossword::ParametricCrossword(const Crossword &crossword,
    const FileProperties &properties, const Palette &palette)
    : Crossword(crossword)
    , properties(properties)
    , palette(palette)
{
}

ParametricCrossword ParametricCrossword::operator=(
    const ParametricCrossword &crossword)
{
    game_ = crossword.game();
    leftTask_ = crossword.leftTask();
    topTask_ = crossword.topTask();
    properties = crossword.properties;
    palette = crossword.palette;
    return *this;
}

int ParametricCrossword::decisionCount()
{
    // todo: resolve task and store this
    properties["is_ready_to_play"] = "1";
    return 1;
}

void ParametricCrossword::clear()
{
    Crossword::clear();
    setTasksAsChecked(false);
}

void ParametricCrossword::erase()
{
    topTask_.clear();
    leftTask_.clear();
    game_.clear();
}

void ParametricCrossword::resize(size_t height, size_t width)
{
    game_.resize(height);
    for (auto &gameLine : game_)
    {
        gameLine.resize(width);
    }
    //topTask_.resize(width);
    //leftTask_.resize(height);
    Crossword::recreateTaskByGame();
}

void ParametricCrossword::fill(ColorIndex color)
{
    for (auto &gameLine : game_)
    {
        gameLine.setPainted(color);
    }
}