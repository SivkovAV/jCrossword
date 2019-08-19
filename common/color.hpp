#pragma once

// (c) Copyright Aleksey Sivkov 2017-2018

#include <color_item.hpp>
#include <QColor>

namespace jcw
{
class Color
{
public:
    Color();
    Color(const QColor color);
    Color(double red, double green, double blue);
    Color(ColorItem red, ColorItem green, ColorItem blue);
    double getR() const;
    double getG() const;
    double getB() const;
    QColor color() const;

    void operator=(const Color dotColor);
    bool operator==(const Color &dotColor) const;
    bool operator!=(Color &dot) const;
    Color operator*(double k) const;
    Color operator-(Color &dotColor);
    Color operator+(Color &dotColor);
    void operator+=(Color &dotColor);

    // расчет среднего значения; realCount не обязательно равен dotColors.size() !!!
    static Color mean(const std::vector<Color> &dotColors, double realCount);
    // расчет среднего значения двух точек
    static Color mean(const Color &dotColor1, const Color &dotColor2);
    static double lenght(const Color &dotColor1, const Color &dotColor2,
        bool useHumanFactor = false);

private:
    ColorItem red_, green_, blue_;
};

} // namespace jcw
