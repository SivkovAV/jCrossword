// (c) Copyright Aleksey Sivkov 2017

#include <crossword.hpp>
#include <painter.hpp>
#include <string>

using namespace jcw;

Crossword::Crossword()
    : game_(Game())
    , leftTask_(Task())
    , topTask_(Task())
{
}

Crossword::Crossword(const Game &game)
    : game_(game)
{
    recreateTaskByGame();
}

Crossword::Crossword(const Game &game, const Task &leftTask, const Task &topTask)
    : game_ (game)
    , leftTask_ (leftTask)
    , topTask_(topTask)
{
}

Crossword::Crossword(const Task &leftTask, const Task &topTask)
    : leftTask_(leftTask)
    , topTask_(topTask)
{
    game_.resize(leftTask_.size(), topTask_.size());
}

void Crossword::verticalInversion()
{
    game_.inversion();
    leftTask_.inversion();
    topTask_.innerInversion();
}

void Crossword::horizontalInversion()
{
    game_.innerInversion();
    leftTask_.innerInversion();
    topTask_.inversion();
}

Crossword Crossword::getTransposed() const
{
    const auto transposedGame = game_.getTransposed();
    return Crossword(transposedGame, topTask_, leftTask_);
}

void Crossword::transpose()
{
    *this = getTransposed();
}

bool Crossword::isMonohrome() const
{
    const auto leftTaskMap = leftTask_.cellsCountMap();
    const auto topTaskMap = topTask_.cellsCountMap();
    if (leftTaskMap.size() > 1 || topTaskMap.size() > 1)
    {
        return false;
    }
    if (leftTaskMap.size() == 1 && topTaskMap.size() == 1 &&
        leftTaskMap.begin()->first != topTaskMap.begin()->first)
    {
        return false;
    }
    return true;
}

void Crossword::recreateTaskByGame()
{
    leftTask_ = Task::leftTaskByGame(game_);
    topTask_ = Task::topTaskByGame(game_);
}

bool Crossword::operator==(const Crossword &crossword) const
{
    return crossword.game() == game_ &&
        crossword.leftTask() == leftTask_ &&
        crossword.topTask() == topTask_;
}


Game Crossword::game() const
{
    return game_;
}

Task Crossword::leftTask() const
{
    return leftTask_;
}

Task Crossword::topTask() const
{
    return topTask_;
}

bool Crossword::isResolved() const
{
    const Task leftTask = Task::leftTaskByGame(game_);
    const Task topTask = Task::topTaskByGame(game_);
    return leftTask.isEqual(leftTask_) && topTask.isEqual(topTask_);
}

bool Crossword::isTaskResolved() const
{
    const auto isLeftTaskResolved = leftTask_.isResolved();
    const auto isTopTaskResolved = topTask_.isResolved();
    return isLeftTaskResolved && isTopTaskResolved;
}

bool Crossword::hasNoTaskConflict() const
{
    const auto leftTaskCellCountMap = leftTask_.cellsCountMap();
    const auto topTaskCellCountMap = topTask_.cellsCountMap();
    return leftTaskCellCountMap == topTaskCellCountMap;
}

size_t Crossword::maxColorsCount() const
{
    return std::max({
        game_.maxColorsCount(),
        leftTask_.maxColorsCount(),
        topTask_.maxColorsCount(),
    });
}

size_t Crossword::maxTaskValue() const
{
    const auto leftMaxValue = leftTask_.maxValue();
    const auto topMaxValue = topTask_.maxValue();
    return leftMaxValue > topMaxValue ? leftMaxValue : topMaxValue;
}

size_t Crossword::width() const
{
    return game_.sizeInside();
}

size_t Crossword::height() const
{
    return game_.size();
}

void Crossword::setEmpty()
{
    game_.setEmpty();
}

bool Crossword::setEmpty(size_t row, size_t column, bool noThrow)
{
    return game_[row][column].setEmpty(noThrow);
}

bool Crossword::setFree(size_t row, size_t column)
{
    return game_[row][column].setFree();
}

bool Crossword::setPainted(size_t row, size_t column,
    ColorIndex color, bool noThrow)
{
    return game_[row][column].setPainted(color, noThrow);
}

bool Crossword::setLeftTaskChecked(size_t row, size_t column, bool isChecked)
{
    return leftTask_[row][column].setChecked(isChecked);
}

bool Crossword::setTopTaskChecked(size_t row, size_t column, bool isChecked)
{
    return topTask_[column][row].setChecked(isChecked);
}

size_t Crossword::topTaskHeight() const
{
    return topTask_.maxLenght();
}

size_t Crossword::leftTaskWidth() const
{
    return leftTask_.maxLenght();
}

void Crossword::clear()
{
    game_.setFree();
}

void Crossword::setTasksAsChecked(bool isChecked)
{
    leftTask_.setChecked(isChecked);
    topTask_.setChecked(isChecked);
}