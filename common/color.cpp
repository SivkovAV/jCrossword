// (c) Copyright Aleksey Sivkov 2017-2018

#include <color.hpp>
#include "math.h"

using namespace jcw;

Color::Color()
{
}

Color::Color(const QColor color)
    : red_(ColorItem(color.red()))
    , green_(ColorItem(color.green()))
    , blue_(ColorItem(color.blue()))
{
}

Color::Color(double red, double green, double blue)
    : red_(ColorItem(red))
    , green_(ColorItem(green))
    , blue_(ColorItem(blue))
{
}

Color::Color(ColorItem red, ColorItem green, ColorItem blue)
    : red_(red)
    , green_(green)
    , blue_(blue)
{
}

double Color::getR() const
{
    return red_();
}

double Color::getG() const
{
    return green_();
}

double Color::getB() const
{
    return blue_();
}

QColor Color::color() const
{
    const auto red = static_cast<int>(round(red_()));
    const auto green = static_cast<int>(round(green_()));
    const auto blue = static_cast<int>(round(blue_()));
    return QColor(red, green, blue);
}


void Color::operator=(const Color dotColor)
{
    red_ = dotColor.red_;
    green_ = dotColor.green_;
    blue_ = dotColor.blue_;
}

bool Color::operator==(const Color &dotColor) const
{
    return (red_ == dotColor.red_ && green_ == dotColor.green_ && blue_ == dotColor.blue_);
}

bool Color::operator!=(Color &dotColor) const
{
    return !(this->operator==(dotColor));
}

Color Color::operator*(double k) const
{
    Color result = *this;
    result.red_ *= k;
    result.green_ *= k;
    result.blue_ *= k;
    return result;
}

Color Color::operator-(Color &dotColor)
{
    Color result;
    result.red_ = red_ - dotColor.red_;
    result.green_ = green_ - dotColor.green_;
    result.blue_ = blue_ - dotColor.blue_;
    return result;
}

Color Color::operator+(Color &dotColor)
{
    Color result;
    result.red_ = red_ + dotColor.red_;
    result.green_ = green_ + dotColor.green_;
    result.blue_ = blue_ + dotColor.blue_;
    return result;
}

void Color::operator+=(Color &dotColor)
{
    *this = *this + dotColor;
}

Color Color::mean(const std::vector<Color> &dotColor, double realCount)
{
    const auto count = dotColor.size();
    std::vector<ColorItem> allR(count), allG(count), allB(count);
    for (size_t i = 0; i < count; ++i)
    {
        allR[i] = dotColor[i].red_;
        allG[i] = dotColor[i].green_;
        allB[i] = dotColor[i].blue_;
    }
    const auto red = ColorItem::mean(allR, realCount);
    const auto green = ColorItem::mean(allG, realCount);
    const auto blue = ColorItem::mean(allB, realCount);
    return Color(red, green, blue);
}

Color Color::mean(const Color &dotColor1, const Color &dotColor2)
{
    const auto red = ColorItem::mean(dotColor1.red_, dotColor2.red_);
    const auto green = ColorItem::mean(dotColor1.green_, dotColor2.green_);
    const auto blue = ColorItem::mean(dotColor1.blue_, dotColor2.blue_);
    return Color(red, green, blue);
}

double Color::lenght(const Color &dotColor1, const Color &dotColor2,
    bool useHumanFactor)
{
    auto rLength = ColorItem::lenght(dotColor1.red_, dotColor2.red_);
    auto gLength = ColorItem::lenght(dotColor1.green_, dotColor2.green_);
    auto bLength = ColorItem::lenght(dotColor1.blue_, dotColor2.blue_);
    // учитываем различную важность цветовых составляющих
    // с точки зрения человеческого восприятия
    if (useHumanFactor)
    {
        rLength *= 0.3;
        gLength *= 0.59;
        bLength *= 0.11;
    }
    return sqrt(rLength * rLength + gLength * gLength + bLength * bLength);
}