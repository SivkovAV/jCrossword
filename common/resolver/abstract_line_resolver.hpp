#pragma once

// (c) Copyright Aleksey Sivkov 2017
/*
    ��������� �������-��������� ������ ���������� ��� ��������������
������� ������� ������. �������� ���������� ����� process(). 
    ����������� ���������� � process() ������� ��������, ���������� ��������
(�������� ������ ����������). ��������� ���������� � process() ��������
�������� ������� ��������. ���� process() ������ true, ������ ����� ����
���������� ��������� (�������� ������, ������� ������ ������, ������ �����
������� � ������� � �.�.)
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
    // �������� - ��������-�������� ����� �������� ������
    AbstractLineResolver(AbstractLineResolverCollector *collector = nullptr);
    // ���������� ��������� �������� ���� ���������� (��� gui)
    virtual std::string description() const = 0;
    // ���� true - �������� ������ ������������ ��� ������������ �����
    virtual bool isMonohromOnly() const = 0;
    // ���� false - �������� �� ����� ���� (����������� ������ �������)
    virtual bool canResolve() const = 0;
    // ���� true - �������� ����� �������� ������� ��������� �
    // �������������� ��������������� �� ��������� �������
    // ��� ������� ������ ���������� � �������� ��������� ���������
    virtual bool canFindSubtask() const = 0;
    // ������ ���������
    virtual bool process(GameLine &game, TaskLine &task) const = 0;

protected:
    // �������� �������� ������ ������ � ������ + �������������
    bool drawFigure(GameLine &game, TaskLine &task,
        size_t beginGameIndex, size_t endGameIndex, size_t taskIndex) const;
    // ������ ������� ���������� ��� ������� ���������
    bool externalProcess(
        GameLine &game, size_t beginGameIndex, size_t endGameIndex,
        TaskLine &task, size_t beginTaskIndex, size_t endTaskIndex) const;

    AbstractLineResolverCollector *collector_;

private:
    // �������� �������� ������ ������ � ������
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