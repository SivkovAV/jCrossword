#pragma once

// (c) Copyright Aleksey Sivkov 2017-2018

#include <memory>
#include <properties.hpp>
#include <palette.hpp>
#include <crossword.hpp>

namespace jcw
{

class ParametricCrossword : public Crossword
{
public:
    ParametricCrossword();
    ParametricCrossword(const Crossword &crossword);
    ParametricCrossword(const Crossword &crossword,
        const FileProperties& properties, const Palette &colors);

    ParametricCrossword operator=(const ParametricCrossword &crossword);
    int decisionCount();
    void clear();
    void erase();
    void resize(size_t height, size_t width);
    void fill(ColorIndex color);

    FileProperties properties;
    Palette palette;
};

} // namespace jcw