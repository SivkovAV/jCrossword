// (c)Copyright Aleksey Sivkov 2018-2019

#include <jcwTest.hpp>
#include <generators.hpp>
#include <properties.hpp>
#include <parametric_crossword.hpp>
#include <painter.hpp>
#include <color_item.hpp>
#include <color.hpp>
#include <colors_claster.hpp>
#include <colors_multiclaster.hpp>
#include <resolvers_tests.hpp>

using namespace jcw;

void JCWTest::gameCellTest() const
{
    GameCell gameCell;
    QCOMPARE(gameCell.isEmpty(), false);
    QCOMPARE(gameCell.isFree(), true);
    QCOMPARE(gameCell.isPainted(), false);
    QCOMPARE(gameCell.color(), ColorIndex());

    QCOMPARE(gameCell.setEmpty(), true);
    QCOMPARE(gameCell.isEmpty(), true);
    QCOMPARE(gameCell.isFree(), false);
    QCOMPARE(gameCell.isPainted(), false);
    QCOMPARE(gameCell.color(), ColorIndex());

    const ColorIndex color = (ColorIndex)1;
    QCOMPARE(gameCell.setPainted(color, true), true);
    QCOMPARE(gameCell.isEmpty(), false);
    QCOMPARE(gameCell.isFree(), false);
    QCOMPARE(gameCell.isPainted(), true);
    QCOMPARE(gameCell.color(), color);

    gameCell = GameCell();
    QCOMPARE(gameCell.setPainted(ColorIndex()), true);
}

void JCWTest::taskCellTest() const
{
    TaskCell taskCell1;
    TaskCell taskCell2 = TaskCell(size_t(), ColorIndex());
    QCOMPARE(taskCell1.isEqual(taskCell2), false);
    QCOMPARE(taskCell2.isEqual(taskCell1), false);
    QCOMPARE(taskCell1.value() == size_t(), false);
    QCOMPARE(taskCell1.color(), ColorIndex());
    QCOMPARE(taskCell1.isChecked(), false);
    QCOMPARE(taskCell2.value() == size_t(), true);
    QCOMPARE(taskCell2.color(), ColorIndex());
    QCOMPARE(taskCell2.isChecked(), false);

    QCOMPARE(taskCell1.value() == taskCell2.value(), false);
    taskCell2.inc();
    QCOMPARE(taskCell1.value() == taskCell2.value(), true);

    taskCell1.setValue(size_t());
    QCOMPARE(taskCell1.value() == taskCell2.value(), false);
    taskCell1.inc();
    QCOMPARE(taskCell1.value() == taskCell2.value(), true);

    QCOMPARE(taskCell1.setChecked(true), true);
    QCOMPARE(taskCell1.setChecked(true), false);
    QCOMPARE(taskCell1.value() == taskCell2.value(), true);
    QCOMPARE(taskCell1.setChecked(false), true);
    QCOMPARE(taskCell1.value() == taskCell2.value(), true);

    QCOMPARE(taskCell1.isChecked(), false);
    taskCell1.invertChecking();
    QCOMPARE(taskCell1.isChecked(), true);
    QCOMPARE(taskCell1.value() == taskCell2.value(), true);

    QCOMPARE(taskCell2.color() == ColorIndex(), true);
    taskCell2.setColor((ColorIndex)1);
    QCOMPARE(taskCell2.color() == ColorIndex(), false);

    taskCell2 = taskCell1;
    QCOMPARE(taskCell1.isEqual(taskCell2), true);
    QCOMPARE(taskCell2.isEqual(taskCell1), true);
    taskCell1.invertChecking();
    QCOMPARE(taskCell1.isEqual(taskCell2), false);
    QCOMPARE(taskCell2.isEqual(taskCell1), false);
}

void JCWTest::gameLineTest() const
{
    GameLine gameLine1;
    GameLine taskLine2(10);
    GameLine gameLine3(taskLine2);

    QCOMPARE(gameLine1 == taskLine2, false);
    QCOMPARE(gameLine1 != gameLine3, true);
    QCOMPARE(taskLine2 == gameLine3, true);
    taskLine2.FreeToEmpty();
    QCOMPARE(taskLine2 == gameLine3, false);
    gameLine1.resize(taskLine2.size());
    QCOMPARE(gameLine1 == taskLine2, false);
    QCOMPARE(gameLine1 == gameLine3, true);

    gameLine3.clear();
    QCOMPARE(gameLine1 == gameLine3, false);
    gameLine1.clear();
    QCOMPARE(gameLine1 == gameLine3, true);

    gameLine1.resize(taskLine2.size());
    gameLine3.resize(taskLine2.size());
    QCOMPARE(gameLine1 == gameLine3, true);
    QCOMPARE(gameLine1 == taskLine2, false);
    QCOMPARE(gameLine3 == taskLine2, false);

    QCOMPARE(gameLine1.paintedCellsCount(), (size_t)0);
    gameLine1[1].setPainted(ColorIndex());
    QCOMPARE(gameLine1.paintedCellsCount(), (size_t)1);
    QCOMPARE(gameLine1 == gameLine3, false);
    gameLine3[1].setPainted(gameLine1[1].color());
    QCOMPARE(gameLine1 == gameLine3, true);
    gameLine1[2].setPainted(ColorIndex());
    QCOMPARE(gameLine1.paintedCellsCount(), (size_t)2);
    gameLine1.inversion();
    QCOMPARE(gameLine1 == gameLine3, false);
    QCOMPARE(gameLine1.subLine(3, 6) == gameLine3.subLine(3, 6), true);
    gameLine3.mergeLine(gameLine1.subLine(1, 8), 1, 8);
    QCOMPARE(gameLine1 == gameLine3, true);

    gameLine3.addCells(3);
    QCOMPARE(gameLine1 == gameLine3, false);
    while (gameLine3.size() != gameLine1.size())
    {
        gameLine1.addCell(GameCell());
    }
    QCOMPARE(gameLine1 == gameLine3, true);

    QCOMPARE(gameLine1.setPainted(ColorIndex()), true);
    QCOMPARE(gameLine1 == gameLine3, false);
    QCOMPARE(gameLine3.setFree(), true);
    QCOMPARE(gameLine1 == gameLine3, false);
    QCOMPARE(gameLine3.setEmpty(), true);
    QCOMPARE(gameLine1 == gameLine3, false);
    QCOMPARE(gameLine3.setFree(), true);
    QCOMPARE(gameLine1 == gameLine3, false);
    QCOMPARE(gameLine3.setPainted(ColorIndex()), true);
    QCOMPARE(gameLine1 == gameLine3, true);

    QCOMPARE(gameLine1.maxColorsCount(), 1u);
    gameLine1[2].setPainted((ColorIndex)7u);
    QCOMPARE(gameLine1.maxColorsCount(), 8u);
}

void JCWTest::taskLineTest() const
{
    TaskLine taskLine1;
    TaskLine taskLine2(10);
    TaskLine taskLine3(taskLine2);

    QCOMPARE(taskLine1 == taskLine2, false);
    QCOMPARE(taskLine1 != taskLine3, true);
    QCOMPARE(taskLine2 == taskLine3, true);

    taskLine3.clear();
    QCOMPARE(taskLine1 == taskLine3, true);

    // подготовка игровой строки вида X_#___XX_X_
    GameLine gameLine(10);
    gameLine[0].setPainted(ColorIndex());
    gameLine[2].setPainted((ColorIndex)1);
    gameLine[6].setPainted(ColorIndex());
    gameLine[7].setPainted(ColorIndex());
    gameLine[9].setPainted(ColorIndex());
    taskLine1 = TaskLine::getTaskByGame(gameLine);
    QCOMPARE(taskLine1 == taskLine3, false);
    taskLine3 = taskLine1;
    QCOMPARE(taskLine1 == taskLine3, true);
    taskLine3[0].setValue(3);
    QCOMPARE(taskLine1.isSolution(gameLine), true);
    QCOMPARE(taskLine2.isSolution(gameLine), false);
    QCOMPARE(taskLine3.isSolution(gameLine), false);

    taskLine1.inversion();
    QCOMPARE(taskLine1.isSolution(gameLine), false);
    taskLine1.inversion();
    QCOMPARE(taskLine1.isSolution(gameLine), true);
    
    QCOMPARE(taskLine1.isResolved(), false);
    QCOMPARE(taskLine1.isResolved(gameLine), true);
    QCOMPARE(taskLine3.setChecked(true), true);
    QCOMPARE(taskLine3.isResolved(), true);
    QCOMPARE(taskLine3.setChecked(0, false), true);
    QCOMPARE(taskLine3.isResolved(), false);

    QCOMPARE(taskLine1.maxValue(), 2u);

    size_t paintedCellsCount = 0;
    for (size_t i = 0; i < taskLine1.size(); i++)
    {
        paintedCellsCount += taskLine1[i].value();
    }
    QCOMPARE(taskLine1.paintedCellsCount(), paintedCellsCount);

    QCOMPARE(taskLine1.emptyCellsCount(), 1u);

    size_t min = taskLine1.paintedCellsCount() + taskLine1.emptyCellsCount();
    QCOMPARE(taskLine1.minimumGameCellsCount(), min);

    GameLine tmpGameLine(gameLine.size());
    taskLine1.fillGameLine(tmpGameLine);
    QCOMPARE(taskLine1 != TaskLine::getTaskByGame(tmpGameLine), true);
    QCOMPARE(taskLine1.isMonohrome(), false);
    QCOMPARE(taskLine2.isMonohrome(), true);

    taskLine2.resize(taskLine1.size() * 2);
    QCOMPARE(taskLine2.size(), taskLine1.size() * 2);
    while (taskLine1.size() < taskLine2.size())
    {
        taskLine1.addCell(TaskCell());
    }
    taskLine3.addCells(taskLine1.size() - taskLine3.size());
    QCOMPARE(taskLine1.size() == taskLine3.size(), true);
    QCOMPARE(taskLine1 == taskLine3, false);

    const size_t lastIndex = taskLine1.size() - 1;
    taskLine3.mergeLine(taskLine1.subLine(0, lastIndex), 0, lastIndex);
    QCOMPARE(taskLine1 == taskLine3, true);

    QCOMPARE(taskLine1.maxColorsCount(), 2u);
}

void JCWTest::gameTest() const
{
    const size_t height = 10u;
    const size_t width = 15u;
    Game game1;
    Game game2(height, width);
    QCOMPARE(game1.size(), 0u);
    QCOMPARE(game1.sizeInside(), 0u);
    QCOMPARE(game2.size(), height);
    QCOMPARE(game2.sizeInside(), width);
    game1.resize(height);
    QCOMPARE(game1.size(), height);
    QCOMPARE(game1.sizeInside(), 0u);
    game1.resize(width, height);
    QCOMPARE(game1.size(), width);
    QCOMPARE(game1.sizeInside(), height);

    game1[0][2].setPainted(ColorIndex());
    game2[2][0].setPainted(ColorIndex());
    QCOMPARE(game1.getTransposed() == game2, true);
    QCOMPARE(game1 == game2, false);
    game1.transpose();
    QCOMPARE(game1 == game2, true);
    game1.setFree();
    game2.setFree();
    QCOMPARE(game1 == game2, true);
    game2.setEmpty();
    QCOMPARE(game1 == game2, false);
    QCOMPARE(game1[0][0].isFree(), true);
    QCOMPARE(game2[0][0].isEmpty(), true);

    game1.setFree();
    game2 = game1;
    game1[0][2].setPainted(ColorIndex());
    game2[game2.size() - 1][2].setPainted(ColorIndex());
    QCOMPARE(game1 == game2, false);
    game2.inversion();
    QCOMPARE(game1 == game2, true);

    game1.setFree();
    game2 = game1;
    game1[0][2].setPainted(ColorIndex());
    game2[0][game2.sizeInside() - 1 - 2].setPainted(ColorIndex());
    QCOMPARE(game1 == game2, false);
    game2.innerInversion();
    QCOMPARE(game1 == game2, true);
    game2 = Game(game1);
    QCOMPARE(game1 == game2, true);
    game2 = Game((std::vector<GameLine>)game1);
    QCOMPARE(game1 == game2, true);

    game1[0][3].setEmpty();
    game2 = game1.subGame(1, game1.sizeInside() - 2, 0, game1.size() / 2);
    QCOMPARE(game2.size(), game1.size() / 2);
    QCOMPARE(game2.sizeInside(), game1.sizeInside() - 2);
    QCOMPARE(game2[0][0].isFree(), true);
    QCOMPARE(game2[0][1].isPainted(), true);
    QCOMPARE(game2[0][2].isEmpty(), true);

    QCOMPARE(game2.maxColorsCount(), 1u);
    game2[0][0].setPainted((ColorIndex)1u);
    QCOMPARE(game2.maxColorsCount(), 2u);
}

void JCWTest::taskTest() const
{
    const size_t height = 10u;
    const size_t width = 15u;
    Game game(height, width);
    game[0][2].setEmpty();
    game[1][3].setPainted((ColorIndex)1);
    game[1][5].setPainted();
    game[1][6].setPainted();
    game[1][7].setPainted();
    Task leftTask = Task::leftTaskByGame(game);
    Task topTask = Task::topTaskByGame(game);

    Task task(leftTask);
    QCOMPARE(task.isEqual(leftTask), true);
    QCOMPARE(task.isEqual(topTask), false);
    QCOMPARE(leftTask.isEqual(task), true);
    QCOMPARE(topTask.isEqual(task), false);
    
    QCOMPARE(leftTask.cellsCountMap(), topTask.cellsCountMap());
    QCOMPARE(leftTask.cellsCountMap().at((ColorIndex)0), 3u);
    QCOMPARE(leftTask.cellsCountMap().at((ColorIndex)1), 1u);
    QCOMPARE(leftTask.cellsCountMap().size(), 2u);

    QCOMPARE(leftTask.isResolved(), false);
    leftTask.setChecked(true);
    QCOMPARE(leftTask.isResolved(), true);

    QCOMPARE(leftTask.maxLenght() == leftTask.cellsCountMap().size(), true);
    QCOMPARE(topTask.maxLenght() == topTask.cellsCountMap().size(), false);
    QCOMPARE(leftTask.maxValue() == 3, true);
    QCOMPARE(topTask.maxValue() == 1, true);

    leftTask.setChecked(false);
    QCOMPARE(leftTask[1][0].value(), 1u);
    QCOMPARE(leftTask[1][1].value(), 3u);
    QCOMPARE(leftTask[1][0].color(), ColorIndex(1));
    QCOMPARE(leftTask[1][1].color(), ColorIndex(0));
    task.innerInversion();
    QCOMPARE(leftTask == task, false);
    leftTask.innerInversion();
    QCOMPARE(leftTask == task, true);
    QCOMPARE(leftTask[1][0].value(), 3u);
    QCOMPARE(leftTask[1][1].value(), 1u);
    QCOMPARE(leftTask[1][0].color(), ColorIndex(0));
    QCOMPARE(leftTask[1][1].color(), ColorIndex(1));

    task.inversion();
    QCOMPARE(leftTask == task, false);
    leftTask.inversion();
    QCOMPARE(leftTask == task, true);
    QCOMPARE(leftTask[8][0].value(), 3u);
    QCOMPARE(leftTask[8][1].value(), 1u);
    QCOMPARE(leftTask[8][0].color(), ColorIndex(0));
    QCOMPARE(leftTask[8][1].color(), ColorIndex(1));

    task = leftTask.subTask(8, 1);
    QCOMPARE(task.size() == 1u, true);
    QCOMPARE(task.maxLenght() == 2u, true);
    QCOMPARE(task[0] == leftTask[8], true);

    task = leftTask.subTask(8, 1, 0, 1);
    QCOMPARE(task.size() == 1u, true);
    QCOMPARE(task.maxLenght() == 2u, true);
    QCOMPARE(task[0][0].isEqual(leftTask[8][0]), true);
}

void JCWTest::paletteTest() const
{
    Palette palette(2u);
    QCOMPARE(palette.main.size(), 2u);
    QCOMPARE(palette.main[0] != palette.main[1], true);
    const std::string colorName1 = Palette::toString(palette.main.at(0));
    const std::string colorName2 = Palette::toString(palette.main.at(1));
    QCOMPARE(colorName1 != colorName2, true);
    QCOMPARE(Palette::fromString(colorName1), palette.main.at(0));
    QCOMPARE(Palette::fromString(colorName2), palette.main.at(1));
    QCOMPARE(Palette::fromString(colorName1) != Palette::fromString(colorName2), true);
}

void JCWTest::crosswordTest() const
{
    const size_t height = 10u;
    const size_t width = 15u;
    Game game = RandomGenegator::game(height, width, Palette(2u));
    Crossword crossword(game);
    Crossword crossword1;
    Crossword crossword2(game, Task::leftTaskByGame(game), Task::topTaskByGame(game));
    Crossword crossword3(Task::leftTaskByGame(game), Task::topTaskByGame(game));

    QCOMPARE(crossword == crossword1, false);
    QCOMPARE(crossword == crossword2, true);
    QCOMPARE(crossword == crossword3, false);

    Task topTask = crossword.topTask();
    Task leftTask = crossword.leftTask();
    TaskLine leftTaskLine = leftTask[0];
    GameLine gameLine = crossword.game()[0];
    crossword.verticalInversion();
    topTask.innerInversion();
    leftTask.inversion();
    QCOMPARE(topTask.isEqual(crossword.topTask()), true);
    QCOMPARE(leftTask == crossword.leftTask(), true);
    QCOMPARE(leftTaskLine == (crossword.leftTask()[leftTask.size()-1]), true);
    QCOMPARE(gameLine == (crossword.game()[crossword.height()-1]), true);

    topTask = crossword.topTask();
    leftTask = crossword.leftTask();
    TaskLine topTaskLine = topTask[0];
    gameLine = crossword.game().getTransposed()[0];
    crossword.horizontalInversion();
    topTask.inversion();
    leftTask.innerInversion();
    QCOMPARE(topTask.isEqual(crossword.topTask()), true);
    QCOMPARE(leftTask == crossword.leftTask(), true);
    QCOMPARE(topTaskLine == (crossword.topTask()[topTask.size() - 1]), true);
    QCOMPARE(gameLine == crossword.game().getTransposed()[crossword.width() - 1], true);
    

    crossword1 = crossword.getTransposed();
    QCOMPARE(crossword1.game().getTransposed(), crossword.game());
    QCOMPARE(crossword1.leftTask(), crossword.topTask());
    QCOMPARE(crossword1.topTask(), crossword.leftTask());

    crossword.transpose();
    QCOMPARE(crossword1, crossword);

    QCOMPARE(crossword.isMonohrome(), false);
    QCOMPARE(Crossword(RandomGenegator::game(height, width)).isMonohrome(), true);

    QCOMPARE(crossword.isResolved(), true);
    QCOMPARE(crossword.hasNoTaskConflict(), true);
    leftTask = crossword.leftTask();
    leftTask[0][0] = 3;
    crossword = Crossword(crossword.game(), leftTask, crossword.topTask());
    QCOMPARE(crossword.isResolved(), false);
    QCOMPARE(crossword.hasNoTaskConflict(), false);
    crossword.recreateTaskByGame();
    QCOMPARE(crossword.isResolved(), true);
    QCOMPARE(crossword.hasNoTaskConflict(), true);

    QCOMPARE(crossword.isTaskResolved(), false);
    crossword.setTasksAsChecked(true);
    QCOMPARE(crossword.isTaskResolved(), true);
    crossword.setTasksAsChecked(false);
    QCOMPARE(crossword.isTaskResolved(), false);

    const size_t leftTaskMaxValue = crossword.leftTask().maxValue();
    const size_t topTaskMaxValue = crossword.topTask().maxValue();
    const size_t maxTaskValue = std::max(leftTaskMaxValue, topTaskMaxValue);
    QCOMPARE(crossword.maxTaskValue(), maxTaskValue);

    crossword.clear();
    for (size_t i = 0; i < crossword.height(); i++)
    {
        for (size_t j = 0; j < crossword.width(); j++)
        {
            QCOMPARE(crossword.game()[i][j].isFree(), true);
        }
    }

    crossword.setEmpty();
    for (size_t i = 0; i < crossword.height(); i++)
    {
        for (size_t j = 0; j < crossword.width(); j++)
        {
            QCOMPARE(crossword.game()[i][j].isEmpty(), true);
        }
    }

    ColorIndex color;
    crossword.clear();
    QCOMPARE(crossword.setPainted(0u, 2u, ColorIndex()), true);
    QCOMPARE(crossword.setPainted(0u, 2u, ColorIndex()), false);
    QCOMPARE(crossword.game()[0][2].isPainted(&color), true);
    QCOMPARE(color, ColorIndex());
    QCOMPARE(crossword.setFree(0u, 2u), true);
    QCOMPARE(crossword.setFree(0u, 2u), false);
    QCOMPARE(crossword.game()[0][2].isFree(), true);
    QCOMPARE(crossword.setEmpty(0u, 2u), true);
    QCOMPARE(crossword.setEmpty(0u, 2u), false);
    QCOMPARE(crossword.game()[0][2].isEmpty(), true);

    QCOMPARE(crossword.leftTask()[0][0].isChecked(), false);
    QCOMPARE(crossword.setLeftTaskChecked(0, 0, true), true);
    QCOMPARE(crossword.setLeftTaskChecked(0, 0, true), false);
    QCOMPARE(crossword.leftTask()[0][0].isChecked(), true);

    QCOMPARE(crossword.topTask()[0][0].isChecked(), false);
    QCOMPARE(crossword.setTopTaskChecked(0, 0, true), true);
    QCOMPARE(crossword.setTopTaskChecked(0, 0, true), false);
    QCOMPARE(crossword.topTask()[0][0].isChecked(), true);

    QCOMPARE(crossword.topTaskHeight(), crossword.topTask().maxLenght());
    QCOMPARE(crossword.leftTaskWidth(), crossword.leftTask().maxLenght());

    auto r = crossword.maxColorsCount();
    QCOMPARE(crossword.maxColorsCount(), 2u);
    crossword.setPainted(0, 0, (ColorIndex)4u);
    QCOMPARE(crossword.maxColorsCount(), 5u);
}

void JCWTest::filePropertiesTest() const
{
    FileProperties fileProperties;
    QCOMPARE(fileProperties.count("id"), 1u);
    QCOMPARE(fileProperties.count("author"), 1u);
    QCOMPARE(fileProperties.count("creating_date"), 1u);
    QCOMPARE(fileProperties.count("gui_name"), 1u);
    QCOMPARE(fileProperties.count("is_ready_to_play"), 1u);
    QCOMPARE(fileProperties.at("is_ready_to_play"), std::string());
    fileProperties.markAsReadyToPlay();
    QCOMPARE(fileProperties.at("is_ready_to_play"), std::string("1"));
}

void JCWTest::parametricCrosswordTest() const
{
    const size_t height = 10u;
    const size_t width = 15u;
    Palette palette(2u);
    Game game = RandomGenegator::game(height, width, palette);
    auto crossword = ParametricCrossword(Crossword(game));
    auto crossword2 = ParametricCrossword();
    auto crossword3 = ParametricCrossword(Crossword(game), FileProperties(), palette);

    QCOMPARE(crossword == crossword2, false);
    QCOMPARE(crossword == crossword3, true);
    QCOMPARE(crossword2 == crossword3, false);
    crossword3.erase();
    QCOMPARE(crossword2.game() == crossword3.game(), true);
    QCOMPARE(crossword2.leftTask() == crossword3.leftTask(), true);
    QCOMPARE(crossword2.topTask() == crossword3.topTask(), true);
    crossword3 = ParametricCrossword(Crossword(), FileProperties(), Palette());
    QCOMPARE(crossword2 == crossword3, true);
    crossword2.operator=(crossword);
    QCOMPARE(crossword == crossword2, true);
    QCOMPARE(crossword == crossword3, false);

    QCOMPARE(crossword.decisionCount() == 1u, true);

    crossword.clear();
    for (size_t i = 0; i < crossword.height(); i++)
    {
        for (size_t j = 0; j < crossword.width(); j++)
        {
            QCOMPARE(crossword.game()[i][j].isFree(), true);
        }
    }

    ColorIndex color;
    crossword.fill((ColorIndex)3u);
    for (size_t i = 0; i < crossword.height(); i++)
    {
        for (size_t j = 0; j < crossword.width(); j++)
        {
            QCOMPARE(crossword.game()[i][j].isPainted(&color), true);
            QCOMPARE(color, (ColorIndex)3u);
        }
    }

    crossword.resize((size_t)(1.5 * height), (size_t)(1.5 * 2 * width));
    QCOMPARE(crossword.height(), (size_t)(1.5 * height));
    QCOMPARE(crossword.width(), (size_t)(1.5 * 2 * width));
}

// генератор таблицы задания в виде "гребенки"
Task generateTask(size_t size, size_t innerSize)
{
    Task task(size);
    size_t lineSize = innerSize;
    for (auto& taskLine : task)
    {
        taskLine.resize(lineSize);
        if (innerSize != 0)
        {
            --lineSize;
            if (lineSize == 0)
            {
                lineSize = innerSize;
            }
        }
    }
    return task;
};

void JCWTest::colorTest() const
{
    double value1 = 27.6, value2 = 70.21;
    ColorItem color1;
    ColorItem color2 = ColorItem(value2);
    QCOMPARE(color1(), 0.0);
    QCOMPARE(color2(), value2);
    ColorItem color3 = ColorItem(value1);
    QCOMPARE(color3(), value1);
    QCOMPARE((color3 + color2)(), value1 + value2);
    QCOMPARE((color3 + value2)(), value1 + value2);
    QCOMPARE((color3 - color2)(), value1 - value2);
    QCOMPARE((color3 - value2)(), value1 - value2);
    QCOMPARE(color3 == color2, false);
    QCOMPARE(color3 == value1, true);
    color3 *= value2;
    QCOMPARE(color3(), value1 * value2);

    std::vector<ColorItem> colors = { ColorItem(3.), ColorItem(30.), ColorItem(300.) };
    QCOMPARE((ColorItem::mean(colors, colors.size()))(), 111.0);
    QCOMPARE((ColorItem::mean(colors, 2))(), 166.5);
    QCOMPARE((ColorItem::mean(ColorItem(3.), ColorItem(30.)))(),16.5);
    QCOMPARE(ColorItem::lenght(ColorItem(3.), ColorItem(30.)), 27.0);
    QCOMPARE(ColorItem::lenght(ColorItem(30.), ColorItem(3.)), 27.0);
}

void JCWTest::dotColorTest() const
{
    Color dotColor;
    QCOMPARE(dotColor.getR(), 0.0);
    QCOMPARE(dotColor.getG(), 0.0);
    QCOMPARE(dotColor.getB(), 0.0);

    QColor color = QColor(255, 157, 30);
    dotColor = Color(color);
    QCOMPARE(dotColor.getR(), 255.0);
    QCOMPARE(dotColor.getG(), 157.0);
    QCOMPARE(dotColor.getB(), 30.0);
    QCOMPARE(dotColor.color(), color);

    dotColor = Color(77.3, 90.1, 160.6);
    QCOMPARE(dotColor.getR(), 77.3);
    QCOMPARE(dotColor.getG(), 90.1);
    QCOMPARE(dotColor.getB(), 160.6);

    ColorItem jcwColor(77.7);
    Color dotColor2 = Color(jcwColor, jcwColor + 1.1, jcwColor - 10.3);
    QCOMPARE(dotColor2.getR(), jcwColor());
    QCOMPARE(dotColor2.getG(), (jcwColor + 1.1)());
    QCOMPARE(dotColor2.getB(), (jcwColor - 10.3)());

    QCOMPARE(dotColor == dotColor2, false);
    QCOMPARE(dotColor != dotColor2, true);
    dotColor = dotColor2;
    QCOMPARE(dotColor == dotColor2, true);

    dotColor2 = dotColor * 2.1;
    QCOMPARE(dotColor2.getR(), dotColor.getR() * 2.1);
    QCOMPARE(dotColor2.getG(), dotColor.getG() * 2.1);
    QCOMPARE(dotColor2.getB(), dotColor.getB() * 2.1);

    Color dotColor3 = dotColor + dotColor2;
    QCOMPARE(dotColor3.getR(), dotColor.getR() + dotColor2.getR());
    QCOMPARE(dotColor3.getG(), dotColor.getG() + dotColor2.getG());
    QCOMPARE(dotColor3.getB(), dotColor.getB() + dotColor2.getB());

    dotColor3 = dotColor - dotColor2;
    QCOMPARE(dotColor3.getR(), dotColor.getR() - dotColor2.getR());
    QCOMPARE(dotColor3.getG(), dotColor.getG() - dotColor2.getG());
    QCOMPARE(dotColor3.getB(), dotColor.getB() - dotColor2.getB());

    dotColor3 = dotColor;
    dotColor += dotColor2;
    QCOMPARE(dotColor.getR(), dotColor3.getR() + dotColor2.getR());
    QCOMPARE(dotColor.getG(), dotColor3.getG() + dotColor2.getG());
    QCOMPARE(dotColor.getB(), dotColor3.getB() + dotColor2.getB());

    std::vector<Color> сolors = { Color(), Color(9.0, 9.0, 9.0), Color()};
    QCOMPARE(Color::mean(сolors, 3), Color(3.0, 3.0, 3.0));
    QCOMPARE(Color::mean(сolors, 2), Color(4.5, 4.5, 4.5));
    QCOMPARE(Color::mean(Color(1, 1, 1), Color(3, 3, 3)), Color(2, 2, 2));
    QCOMPARE(Color::lenght(Color(1, 1, 1), Color(3, 3, 3)), sqrt(12));
}

void JCWTest::colorClasterTest() const
{
    ColorsClaster colorsClaster;
    colorsClaster.addDot(Color());
    colorsClaster.addDot(Color(3., 0., 0.));
    colorsClaster.addDot(Color(0., 9, 0.));

    QCOMPARE(colorsClaster.dotsCount(), 3u);

    QCOMPARE(colorsClaster.center(), Color());
    colorsClaster.recalculateCenter();
    QCOMPARE(colorsClaster.center(), Color(1., 3, 0.));

    colorsClaster.eraseDot(1);
    colorsClaster.recalculateCenter();
    QCOMPARE(colorsClaster.center(), Color(0., 4.5, 0.));

    colorsClaster.setCenter(Color(77., 0., 0.));
    QCOMPARE(colorsClaster.center(), Color(77., 0., 0.));
    QCOMPARE(colorsClaster.lenghtToCenter(Color(3., 0., 0.)), 74.);;

    colorsClaster.setCenter(Color(0., 27., 0.));
    QCOMPARE(colorsClaster.midLenghtToCenter(), (18. + 27.) / 2.);

    QCOMPARE(colorsClaster.farDotIndex(), 0);
    colorsClaster.addDot(Color(100., 9., 0.));
    QCOMPARE(colorsClaster.farDotIndex(), 2);

    QCOMPARE(colorsClaster.isFixedCenter(), false);

    QCOMPARE(colorsClaster.dotByDotIndex(2), Color(100.0, 9.0, 0.0));
    QCOMPARE(colorsClaster.dotByDotIndex(1), Color(0.0, 9.0, 0.0));
    QCOMPARE(colorsClaster.dotByDotIndex(0), Color());

    colorsClaster.clear();
    QCOMPARE(colorsClaster.center(), Color(0.0, 27., 0.));
    colorsClaster.recalculateCenter();
    QCOMPARE(colorsClaster.center(), Color());
}

void JCWTest::image() const
{
    Image image;
    QCOMPARE(image.height(), 0u);
    QCOMPARE(image.width(), 0u);

    size_t height = 100;
    size_t width = 200;
    image = Image(height, width);
    QCOMPARE(image.height(), height);
    QCOMPARE(image.width(), width);
    image.decreaseSize(2u);
    QCOMPARE(image.height(), height / 2);
    QCOMPARE(image.width(), width / 2);
    image.increasePixelsByDot(4);
    QCOMPARE(image.height(), height * 2);
    QCOMPARE(image.width(), width * 2);

    image.trimBorders(0.02);
    QCOMPARE(image.height(), (size_t)((1.0 - 2.0 * 0.02) * (height * 2)));
    QCOMPARE(image.width(),  (size_t)((1.0 - 2.0 * 0.02) * (width * 2)));

    image.resize(height, width);
    QCOMPARE(image.height(), height);
    QCOMPARE(image.width(), width);

    for (size_t x = 0; x < image.width(); x++)
    {
        const auto red   = (10 * x) % 255u;
        const auto green = (20 * x) % 255u;
        const auto blue  = (30 * x) % 255u;
        image.setDot(x, 1, red, green, blue);
        image.setDotAARRGGBB(x, 2, 0xffaa7755);
    }
    Image subImage = image.copy_(10, 1, width / 2, height / 2);
    for (size_t x = 0; x < subImage.width(); x++)
    {
        QCOMPARE(subImage.dot(x, 0).color(), image.dot(x + 10, 1).color());
        QCOMPARE(subImage.color(x, 0), image.color(x + 10, 1));
        QCOMPARE(subImage.dot(x, 1).color(), QColor(0xffaa7755));
        QCOMPARE(subImage.dotAARRGGBB(x, 1), 0xffaa7755);
        QCOMPARE(subImage.dot(x, 2).color(), QColor(0, 0, 0));
    }

    image.transformToGrey();
    for (size_t x = 0; x < image.width(); x++)
    {
        const auto color = image.color(x, 1);
        QCOMPARE(color.getR(), color.getG());
        QCOMPARE(color.getG(), color.getB());
        QCOMPARE(color.getR(), color.getB());
    }

    image.trimMonohromeLines();
    QCOMPARE(image.height(), 2u);
    QCOMPARE(image.width(), width);

    QCOMPARE(image.isValideColors(), true);
    image.setDot(0, 0, -1.0, 0.0, 0.0);
    QCOMPARE(image.isValideColors(), false);
    image.setDot(0, 0, 0.0, 0.0, 0.0);
    QCOMPARE(image.isValideColors(), true);
    image.setDot(0, 0, 256.0, 0.0, 0.0);
    QCOMPARE(image.isValideColors(), false);
}

void JCWTest::colorMulticlasterTest() const
{
    // изображение с 4 явно выраженными группами точек
    Image image(3u, 3u);
    // кластер 1
    image.setDot(0, 0, 50,  51,  58);
    image.setDot(1, 0, 49,  50,  50);
    image.setDot(2, 0, 51,  49,  52);
    // кластер 2
    image.setDot(0, 1, 200, 200, 200);
    image.setDot(1, 1, 200, 200, 200);
    image.setDot(2, 1, 200, 200, 200);
    // кластер 3
    image.setDot(0, 2, 255, 255, 255);
    image.setDot(1, 2, 255, 255, 255);
    // кластер 4
    image.setDot(2, 2, 0, 0, 0);
    // мультикластер на 3 кластера
    ColorsMultiClaster multiClaster = ColorsMultiClaster({
        ColorsClaster(Color(50,  50,  50)),
        ColorsClaster(Color(200, 200, 200)),
        ColorsClaster(Color(255, 255, 255))
    });
    multiClaster.recalculateImage(image);
    QCOMPARE(image.color(0, 0), Color(50,  50,  50));
    QCOMPARE(image.color(1, 0), Color(50,  50,  50));
    QCOMPARE(image.color(2, 0), Color(50,  50,  50));
    QCOMPARE(image.color(0, 1), Color(200, 200, 200));
    QCOMPARE(image.color(1, 1), Color(200, 200, 200));
    QCOMPARE(image.color(2, 1), Color(200, 200, 200));
    QCOMPARE(image.color(0, 2), Color(255, 255, 255));
    QCOMPARE(image.color(1, 2), Color(255, 255, 255));
    QCOMPARE(image.color(2, 2), Color(50, 50, 50));

    Palette palette = multiClaster.palette();
    QCOMPARE(palette.back[0], Color(50,  50,  50).color());
    QCOMPARE(palette.main[0], Color(200, 200, 200).color());
    QCOMPARE(palette.main[1], Color(255, 255, 255).color());

    QCOMPARE(multiClaster.biggestClasterIndex(), 0);
    multiClaster[0].addDot(Color(40, 50, 50));
    QCOMPARE(multiClaster.biggestClasterIndex(), 0);
    multiClaster[2].addDot(Color());
    QCOMPARE(multiClaster.biggestClasterIndex(), 0);
    multiClaster[2].addDot(Color());
    QCOMPARE(multiClaster.biggestClasterIndex(), 2);
    multiClaster[1].addDot(Color());
    QCOMPARE(multiClaster.biggestClasterIndex(), 2);
    multiClaster[1].addDot(Color());
    QCOMPARE(multiClaster.biggestClasterIndex(), 1);

    Image tmpImage = image;
    ColorsMultiClaster multiClaster2 = ColorsMultiClaster(image, 3);
    QCOMPARE(multiClaster2.size(), multiClaster.size());
    for (size_t i = 0; i < multiClaster.size(); i++)
    {
        const auto centerColor = multiClaster[i].center();
        const auto dotsCount = multiClaster[i].dotsCount();
        bool isFinded = false;
        for (size_t i = 0; i < multiClaster2.size(); i++)
        {
            if (multiClaster2[i].center() == multiClaster[i].center())
            {
                isFinded = true;
                QCOMPARE(multiClaster[i].dotsCount(), multiClaster2[i].dotsCount());
            }
        }
        QCOMPARE(isFinded, true);
    }

    multiClaster2.recalculateImage(image);
    for (size_t x = 0; x < image.width(); x++)
    {
        for (size_t y = 0; y < image.height(); y++)
        {
            QCOMPARE(tmpImage.color(x, y), image.color(x, y));
        }
    }

    const auto colorsCount = 10u;
    multiClaster = ColorsMultiClaster(colorsCount);
    palette = multiClaster.palette();
    QCOMPARE(palette.main.size(), (colorsCount - 1));
}

void JCWTest::lineResolversTest() const
{
    QCOMPARE(testNoTaskResolver(), true);
    QCOMPARE(testSingleVariant(), true);
    QCOMPARE(testAllTaskChecked(), true);
    QCOMPARE(testResolverFirstLeftFigure(), true);
    QCOMPARE(testResolverAllPainted(), true);
    QCOMPARE(testResolverFillNotPainted(), true);
    QCOMPARE(testResolverCrossFinder(), true);
    // todo: сделать тесты для всех алгоритмов
}

// todo: нужно реализовать побайтовое сравнение сформированных 
// png-файлов с эталонами; эталоны залить в git
void JCWTest::painterTest() const
{
    const QString exeFilePath = QApplication::applicationDirPath();
    const std::string dirname = exeFilePath.toStdString() + "/";

    // тест отрисовки линии игры и линии задания различной длины
    for (size_t taskCellsCount : {0, 1, 5, 6})
    {
        for (size_t gameCellsCount : {0, 1, 5, 6})
        {
            const std::string filename = dirname + "test_empty_line_"
                + std::to_string(taskCellsCount)
                + std::string("_") + std::to_string(gameCellsCount)
                + std::string(".png");
            const GameLine gameLine = GameLine(gameCellsCount);
            const TaskLine taskLine = TaskLine(taskCellsCount);
            Painter::draw(gameLine, taskLine, filename);
        }
    }

    // тест отрисовки игр различного размера
    const auto taskSizeVariants = {0u, 1u, 5u,  6u};
    const auto gameSizeVariants = {0u, 1u, 10u, 11u};
    for (size_t topTaskSize : taskSizeVariants)
    {
        for (size_t leftTaskSize : taskSizeVariants)
        {
            for (size_t gameWidth : gameSizeVariants)
            {
                for (size_t gameHeight : gameSizeVariants)
                {
                    const auto filename = dirname + "test_empty_crossword_"
                        + std::to_string(leftTaskSize)
                        + std::string("_") + std::to_string(topTaskSize)
                        + std::string("_") + std::to_string(gameWidth)
                        + std::string("_") + std::to_string(gameHeight)
                        + std::string(".png");
                    Game game(gameHeight, gameWidth);
                    Task leftTask = generateTask(gameHeight, leftTaskSize);
                    Task topTask = generateTask(gameWidth, topTaskSize);
                    Painter::draw(Crossword(game, leftTask, topTask), filename);
                }
            }
        }
    }

    // тест отрисовки реального кроссворда, его транспонированной копии
    // и отдельной его строки исходного кроссворда
    Crossword testCrossword = TestGenegator::testCrossword();
    Painter::draw(testCrossword, dirname + "test_crossword.png");

    const auto invercedCrossword = testCrossword.getTransposed();
    Painter::draw(invercedCrossword, dirname + "test_cossword_inverced.png");

    Painter::draw(testCrossword.game()[0], testCrossword.leftTask()[0],
        dirname + "testline.png");

    Painter::draw(Palette(7u), dirname + "testpalette.png");
}