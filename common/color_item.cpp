// (c) Copyright Aleksey Sivkov 2017-2018

#include <color_item.hpp>

using namespace jcw;

ColorItem::ColorItem()
    : value_(0)
{
}

ColorItem::ColorItem(double value)
    : value_(value)
{
}

double ColorItem::operator()() const
{
    return value_;
}

void ColorItem::operator=(const ColorItem &jceColor)
{
    value_ = jceColor.value_;
}

ColorItem ColorItem::operator+(const ColorItem &jceColor)
{
    return ColorItem(value_ + jceColor.value_);
}

ColorItem ColorItem::operator+(const double jceColor)
{
    return ColorItem(value_ + jceColor);
}

ColorItem ColorItem::operator-(const ColorItem &jceColor)
{
    return ColorItem(value_ - jceColor.value_);
}

ColorItem ColorItem::operator-(const double jceColor)
{
    return ColorItem(value_ - jceColor);
}

bool ColorItem::operator==(const double jceColor) const
{
    return (value_ == jceColor);
}

bool ColorItem::operator==(const ColorItem &jceColor) const
{
    return (value_ == jceColor.value_);
}

void ColorItem::operator*=(double k)
{
    value_ *= k;
}

ColorItem ColorItem::mean(const std::vector<ColorItem>& colors, double realCount)
{
    if (colors.size() == 0)
    {
        return ColorItem();
    }
    double result = 0.;
    for (const ColorItem &color : colors)
    {
        result += color.value_;
    }
    result /= realCount;
    return ColorItem(result);
}

ColorItem ColorItem::mean(const ColorItem& color1, const ColorItem& color2)
{
    return ColorItem((color1.value_ + color2.value_) / 2.);
}

double ColorItem::lenght(const ColorItem& color1, const ColorItem& color2)
{
    double result = color1.value_ - color2.value_;
    return (result >= 0 ? result : -result);
}