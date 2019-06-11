// (c)Copyright Aleksey Sivkov 2018

#include <game.hpp>

using namespace jcw;

Game::Game(size_t height, size_t width)
{
    resize(height, width);
}

Game::Game(const std::vector<GameLine> &gameLines)
    : LinesVector<GameLine>(gameLines)
{
}

void Game::resize(size_t height)
{
    std::vector<GameLine>::resize(height);
}

void Game::resize(size_t height, size_t width)
{
    std::vector<GameLine>::resize(height);
    for (auto &gameLine : *this)
    {
        gameLine.resize(width);
    }
}

size_t Game::sizeInside() const
{
    if (size() == 0)
    {
        return size_t();
    }
    return (*this)[0].size();
}

void Game::setEmpty()
{
    for (auto &gameLine : *this)
    {
        gameLine.setEmpty();
    };
}

void Game::setFree()
{
    for (auto &gameLine : *this)
    {
        gameLine.setFree();
    };
}

Game Game::getTransposed() const
{
    const auto size = this->size();
    const auto sizeInside = this->sizeInside();
    Game transposedGame(sizeInside, size);
    for (size_t i = 0; i < size; ++i)
    {
        for (size_t j = 0; j < sizeInside; ++j)
        {
            transposedGame[j][i] = (*this)[i][j];
        }
    }
    return transposedGame;
}

void Game::transpose()
{
    *this = getTransposed();
}

Game Game::subGame(size_t left, size_t right, size_t top, size_t bottom) const
{
    Game tempGame(std::vector<GameLine>(begin() + top, begin() + bottom));
    for (auto &line : tempGame)
    {
        line = line.subLine(left, right);
    }
    return tempGame;
}