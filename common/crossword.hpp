#pragma once

// (c) Copyright Aleksey Sivkov 2017-2018

#include <game.hpp>
#include <task.hpp>
#include <map>

namespace jcw
{

class Crossword
{
public:
    Crossword();
    Crossword(const Game &game);
    Crossword(const Game &game, const Task &leftTask, const Task &topTask);
    Crossword(const Task &leftTask, const Task &topTask);

    void verticalInversion();
    void horizontalInversion();
    Crossword getTransposed() const;
    void transpose();
    bool isMonohrome() const;
    void recreateTaskByGame();
    bool operator==(const Crossword& crossword) const;
    Game game() const;
    Task leftTask() const;
    Task topTask() const;
    bool isResolved() const;
    bool isTaskResolved() const;
    bool hasNoTaskConflict() const;
    size_t maxColorsCount() const;
    
    size_t maxTaskValue() const; // получение самого большого значения задания
    size_t width() const;        // длина кроссворда в клетках
    size_t height() const;       // высота кроссворда в клетках
    void setEmpty();
    bool setEmpty(size_t row, size_t column, bool noThrow = false);
    bool setFree(size_t row, size_t column);
    bool setPainted(size_t row, size_t column,
        ColorIndex color = ColorIndex(), bool noThrow = false);
    bool setLeftTaskChecked(size_t row, size_t column, bool isChecked);
    bool setTopTaskChecked(size_t row, size_t column, bool isChecked);
    size_t topTaskHeight() const;
    size_t leftTaskWidth() const;
    void clear();
    void setTasksAsChecked(bool isChecked);

protected:
    // game - набор игровых строк (game_[i] - это строка, а не столбец) 
    Game game_;
    Task leftTask_;
    Task topTask_;
};

} // namespace jcw