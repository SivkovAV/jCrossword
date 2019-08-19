#pragma once

// (c)Copyright Aleksey Sivkov 2018
/*
    Тесты для публичных методов основных классов библиотеки jcw
*/

#include <QObject>
#include <QtTest/QtTest>

class JCWTest : public QObject
{
    Q_OBJECT

private slots:
    void gameCellTest() const;
    void taskCellTest() const;
    void gameLineTest() const;
    void taskLineTest() const;
    void gameTest() const;
    void taskTest() const;
    void paletteTest() const;
    void crosswordTest() const;
    void filePropertiesTest() const;
    void parametricCrosswordTest() const;
    void colorTest() const;
    void dotColorTest() const;
    void colorClasterTest() const;
    void image() const;
    void colorMulticlasterTest() const;

    void lineResolversTest() const;
    void painterTest() const;
};
