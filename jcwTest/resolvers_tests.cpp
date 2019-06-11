// (c) Copyright Aleksey Sivkov 2018

#include <resolvers_tests.hpp>

using namespace jcw;

bool testResolver(AbstractLineResolver *resolver, LineTest test)
{
    const std::string faildTestWarning = "resolver's test faild";
    if (!resolver->process(test.game, test.task))
    {
        return false;
    }

    if (test.game != test.resultGame)
    {
        return false;
    }

    if (test.task != test.resultTask)
    {
        return false;
    }

    return true;
}

bool jcw::testNoTaskResolver()
{
    const auto cellsCount = 10;
    GameLine game(cellsCount), resultGame(cellsCount);
    resultGame.setEmpty();

    ResolverNoTask resolver;

    return testResolver(&resolver, LineTest{ game, TaskLine(), resultGame, TaskLine() });
}

bool jcw::testSingleVariant()
{
    const size_t taskSize = 5u;
    TaskLine task(taskSize);
    for (size_t i = 0; i < taskSize; ++i)
    {
        task[i].setValue(rand() % 3 + 1);
        task[i].setColor((i / 2) % 2);
    }
    const auto gameSize = task.minimumGameCellsCount();
    TaskLine resultTask = task;
    GameLine game(gameSize), resultGame(gameSize);
    resultTask.fillGameLine(resultGame);

    ResolverSingleVariant resolver;
    return testResolver(&resolver, LineTest{game, task, resultGame, resultTask });
}

bool jcw::testAllTaskChecked()
{
    const auto gameSize = 100;
    const auto colorsCount = 5;
    GameLine game(gameSize), resultGame(gameSize);
    for (size_t i = 0; i < gameSize; ++i)
    {
        const bool isPainted = rand() % 2;
        if (isPainted)
        {
            const auto color = static_cast<ColorIndex>(rand() % colorsCount);
            game[i].setPainted(color);
            resultGame[i].setPainted(color);
        }
        else
        {
            resultGame[i].setEmpty();
        }
    }
    auto task = TaskLine::getTaskByGame(game);
    const auto taskSize = task.size();
    for (auto i = 0; i < taskSize; ++i)
    {
        task[i].setChecked(true);
    }

    ResolverAllTaskChecked resolver;
    return testResolver(&resolver, LineTest{ game, task, resultGame, task});
}

bool jcw::testResolverFirstLeftFigure()
{
    ResolverFirstLeftFigure resolver;

    // test1 5|2|__X__#_______________ -> 5|2|XXX__###_____________
    GameLine game(20);
    game[2].setEmpty();
    game[5].setPainted(0);
    TaskLine task(2);
    task[0].setColor(0);
    task[0].setValue(5);
    task[1].setColor(1);
    task[1].setValue(2);
    GameLine resultGame = game;
    resultGame[0].setEmpty();
    resultGame[1].setEmpty();
    resultGame[6].setPainted(0);
    resultGame[7].setPainted(0);
    TaskLine resultTask = task;
    bool result1 = 
        testResolver(&resolver, LineTest{game, task, resultGame, resultTask});

    // test2 5|2|X___#_X______________ -> X|2|X#####X______________
    game.setFree();
    game[0].setEmpty();
    game[4].setPainted(0);
    game[6].setEmpty();
    resultGame = game;
    resultGame[1].setPainted(0);
    resultGame[2].setPainted(0);
    resultGame[3].setPainted(0);
    resultGame[5].setPainted(0);
    resultTask[0].setChecked(true);
    bool result2 =
        testResolver(&resolver, LineTest{game, task, resultGame, resultTask});

    return result1 && result2;
}

bool jcw::testResolverAllPainted()
{
    const auto taskLineSize = 10;
    TaskLine task(taskLineSize);
    for (size_t i = 0; i<taskLineSize; ++i)
    {
        task[i].setValue(rand() % 4 + 1);
        task[i].setColor(rand() % 2);
    }
    TaskLine resultTask = task;
    const auto minimumGameCellsCount = resultTask.minimumGameCellsCount();
    const auto additionalCellsCount = 10;
    GameLine game(minimumGameCellsCount + additionalCellsCount);
    resultTask.fillGameLine(game, additionalCellsCount / 2);
    GameLine resultGame = game;
    resultGame.FreeToEmpty();

    ResolverAllPainted resolver;
    return testResolver(&resolver,
        LineTest{ game, task, resultGame, resultTask });
}

bool jcw::testResolverFillNotPainted()
{
    // 3|1|1|X_#_XXXX__ -> X###XXXX#%
    GameLine game(10);
    game[0].setEmpty();
    game[2].setPainted(0);
    game[4].setEmpty();
    game[5].setEmpty();
    game[6].setEmpty();
    game[7].setEmpty();
    TaskLine task(3);
    task[0].setColor(0);
    task[0].setValue(3);
    task[1].setColor(0);
    task[1].setValue(1);
    task[2].setColor(1);
    task[2].setValue(1);
    GameLine resultGame = game;
    resultGame[1].setPainted(0);
    resultGame[3].setPainted(0);
    resultGame[8].setPainted(0);
    resultGame[9].setPainted(1);
    TaskLine resultTask = task;
    for (size_t i = 0; i < task.size(); ++i)
    {
        resultTask[i].setChecked(true);
    }

    ResolverFillNotPainted resolver;
    return testResolver(&resolver,
        LineTest{ game, task, resultGame, resultTask });
}

bool jcw::testResolverCrossFinder()
{
    ResolverCrossFinder resolver;

    // test1  3|10|_#__________________ -> 3|10|_#________%%%_______
    GameLine game(20);
    game[1].setPainted(0);
    TaskLine task(2);
    task[0].setColor(0);
    task[0].setValue(3);
    task[1].setColor(1);
    task[1].setValue(10);
    GameLine resultGame = game;
    resultGame[10].setPainted(1);
    resultGame[11].setPainted(1);
    resultGame[12].setPainted(1);
    TaskLine resultTask = task;
    bool result1 = testResolver(&resolver,
        LineTest{game, task, resultGame, resultTask});

    // test2  3|10|_#__________________ -> 3|10|_#________####______
    task[1].setColor(0);
    resultTask = task;
    resultGame[10].setPainted(0);
    resultGame[11].setPainted(0);
    resultGame[12].setPainted(0);
    resultGame[13].setPainted(0);
    bool result2 = testResolver(&resolver,
        LineTest{ game, task, resultGame, resultTask });

    // test3  2|1|__X_ -> 2|1|##X#
    GameLine game2(4);
    game2[2].setEmpty();
    TaskLine task2(2);
    task2[0].setColor(0);
    task2[0].setValue(2);
    task2[1].setColor(0);
    task2[1].setValue(1);
    GameLine resultGame2 = game2;
    resultGame2[0].setPainted(0);
    resultGame2[1].setPainted(0);
    resultGame2[3].setPainted(0);
    TaskLine resultTask2 = task2;
    resultTask2[0].setChecked(true);
    resultTask2[1].setChecked(true);
    bool result3 = testResolver(&resolver,
        LineTest{game, task, resultGame, resultTask});

    return result1 && result2 && result3;
}