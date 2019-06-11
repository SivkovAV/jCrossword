#pragma once

// (c) Copyright Aleksey Sivkov 2017
/*
    Интерфейс объекта-алгоритма поиска финального или промежуточного
решения игровой строки. Решением занимается метод process(). 
    Недопустимо передавать в process() игровую ситуацию, содержащее коллизии
(возможен выбрас исключений). Допустимо передавать в process() частично
решенную игровую ситуацию. Если process() вернул true, значит метод внес
позитивные изменения (закрасил клетку, пометил пустую клетку, сделал новую
отметку в задании и т.п.)
*/

#include <task_line.hpp>
#include <memory>

namespace jcw
{

struct LineTest
{
    GameLine game;
    TaskLine task;
    GameLine resultGame;
    TaskLine resultTask;
};

class AbstractLineResolver;
typedef std::shared_ptr<AbstractLineResolver> SharedLineResolver;

class AbstractLineResolverCollector
{
public:
    virtual bool execute(GameLine &game, TaskLine &task) const = 0;
};

class AbstractLineResolver
{
public:
    // родитель - алгоритм-владелец более высокого уровня
    AbstractLineResolver(AbstractLineResolverCollector *collector = nullptr);
    // возвращает текстовое описание сути алогоритма (для gui)
    virtual std::string description() const = 0;
    // если true - алгоритм нельзя использовать для многоцветных задач
    virtual bool isMonohromOnly() const = 0;
    // если false - алгоритм не может дать (редактирует только задание)
    virtual bool canResolve() const = 0;
    // если true - алгоритм может находить игровую подстроку и
    // гарантированно соответствующую ей подстроку задания
    // для запуска прочих алгоритмов в наденной локальной подзадаче
    virtual bool canFindSubtask() const = 0;
    // запуск алгоритма
    virtual bool process(GameLine &game, TaskLine &task) const = 0;

protected:
    // внесение заведомо верных данных о фигуре + постобработка
    bool drawFigure(GameLine &game, TaskLine &task,
        size_t beginGameIndex, size_t endGameIndex, size_t taskIndex) const;
    // запуск внешних алгоритмов для решения подзадачи
    bool externalProcess(
        GameLine &game, size_t beginGameIndex, size_t endGameIndex,
        TaskLine &task, size_t beginTaskIndex, size_t endTaskIndex) const;

    AbstractLineResolverCollector *collector_;

private:
    // внесение заведомо верных данных о фигуре
    bool drawFigureOnly(GameLine &game, ColorIndex taskColor,
        size_t beginGameIndex, size_t endGameIndex, size_t taskIndex) const;
    void findFigureBorders(GameLine &game, ColorIndex taskColor,
        size_t &leftBorder, size_t &rightBorder, size_t taskIndex) const;
    bool setTaskCellAsResolved(GameLine &game, TaskLine &task,
        size_t taskIndex) const;
    bool fillLeftSizeCellsAsEmpty(GameLine &game, size_t leftBorder) const;
    bool fillRightSizeCellsAsEmpty(GameLine &game, size_t rightBorder) const;
    bool complateFigure(GameLine &game, TaskLine &task,
        size_t leftBorder, size_t rightBorder, size_t taskIndex) const;
    bool isBlockedCell(const GameLine &game, ColorIndex taskColor,
        size_t gameCellIndex) const;


};

}