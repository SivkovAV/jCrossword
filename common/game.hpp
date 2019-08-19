#pragma once

// (c)Copyright Aleksey Sivkov 2018

#include <game_line.hpp>
#include <lines_vector.hpp>

namespace jcw
{

class Game : public LinesVector<GameLine>
{
public:
    Game(size_t height = 0, size_t width = 0);
    Game(const std::vector<GameLine> &gameLines);

    void resize(size_t height);
    void resize(size_t height, size_t width);
    size_t sizeInside() const;
    void setEmpty();
    void setFree();
    Game getTransposed() const;
    void transpose();              // транспонирование
    Game subGame(size_t left, size_t right, size_t top, size_t bottom) const;
};

} // namespace jcw