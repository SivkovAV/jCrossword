// (c) Copyright Aleksey Sivkov 2017-2018

#include <generators.hpp>

using namespace jcw;

Game RandomGenegator::game(size_t height, size_t width, const Palette &palette)
{
    const size_t colorsCount = palette.main.size();
    Game resultGame(height, width);
    for (auto &gameLine : resultGame)
    {
        for (size_t i = 0; i < width; ++i)
        {
            switch (rand() % 3)
            {
            case 0: gameLine[i].setEmpty(); break;
            case 1: gameLine[i].setPainted(rand() % colorsCount); break;
            }
        }
    }
    return resultGame;
}

Task RandomGenegator::leftTask(size_t height, size_t width, const Palette &palette)
{
    const auto game = RandomGenegator::game(height, width, palette);
    return Task::leftTaskByGame(game);

}

Task RandomGenegator::topTask(size_t width, size_t height, const Palette& palette)
{
    return leftTask(height, width, palette);
}

GameLine TestGenegator::testGameLine()
{
    const size_t width = 6;
    GameLine line(width);
    line[1].setPainted(0);
    line[2].setEmpty();
    line[4].setPainted(1);
    line[5].setPainted(1);
    return line;
}

Crossword TestGenegator::testCrossword()
{
    const size_t height = 2;
    const size_t width = 6;

    Game game(height);
    game[0] = testGameLine();
    game[1].resize(width);
    game[1][0].setPainted(0);
    game[1][1].setEmpty();
    game[1][4].setPainted(0);

    Task leftTask = Task::leftTaskByGame(game);
    Task topTask = Task::topTaskByGame(game);
    leftTask[0][1].setChecked(true);
    topTask[1][0].setChecked(true);

    return Crossword(game, leftTask, topTask);
}