#pragma once

// (c) Copyright Aleksey Sivkov 2017

#include <QColor>
#include <vector>

namespace jcw
{

class Palette
{
public:
    Palette(size_t mainColorsCount = 1u);
    void reset(size_t mainColorsCount = 1u);

    static std::string toString(QColor color);
    static QColor fromString(std::string string);

    std::vector<QColor> back; // цвета фона
    std::vector<QColor> main; // цвета закрашенных клеток
    QColor gameBack;          // цвет фона кроссворда
    QColor taskBack;          // цвет фона в клетках задания
    QColor taskText;          // цвет текста в клетках задания
    QColor lines;             // цвета линий
};

} // namespace jcw