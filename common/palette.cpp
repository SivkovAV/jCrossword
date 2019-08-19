// (c) Copyright Aleksey Sivkov 2017

#include <palette.hpp>

using namespace jcw;

std::vector<QColor> mainColorsExample{
    // todo пополнить набор до 10
    QColor(0, 150, 255),
    QColor("lightGreen"),
    QColor("Red"),
    QColor("Yellow"),
    QColor("10, 10, 10"),
    QColor("Green"),
    QColor("Gray"),
};

Palette::Palette(size_t mainColorsCount)
{
    reset(mainColorsCount);
}

void Palette::reset(size_t mainColorsCount)
{
    // "black" "gray" "blue" "lightGreen" "lightBlue");

    back.resize(2);
    back[0] = QColor(255, 255, 255);    // основной цвет фона
    back[1] = QColor(230, 230, 250);    // дополнительный цвет фона

    mainColorsCount = mainColorsCount < 1u ? 1u : mainColorsCount;
    main.resize(mainColorsCount);
    for (size_t i = 0; i < main.size(); i++)
    {
        main[i] = mainColorsExample.at(i % mainColorsExample.size());
    }
    gameBack = QColor(240, 255, 255);
    taskBack = QColor(230, 230, 230);
    taskText = QColor(0, 0, 0);
    lines = QColor(0, 0, 0);
}

std::string Palette::toString(QColor color)
{
    const auto string = std::to_string(color.red())
        + " " + std::to_string(color.green())
        + " " + std::to_string(color.blue());
    return string;
}

QColor Palette::fromString(std::string string)
{
    try
    {
        int space1 = string.find_first_of(" "),
            space2 = string.find_last_of(" ");
        if (space1 >= 0 && space2 >= 0 && space1 != space2)
        {
            const auto len = string.length();
            const auto r = string.substr(0, space1);
            const auto g = string.substr(space1 + 1, space2 - (space1 + 1));
            const auto b = string.substr(space2 + 1, len - (space2 + 1));
            return QColor(std::stoi(r), std::stoi(g), std::stoi(b));
        }
    }
    catch (...) {};
    return QColor(0, 0, 0);
}