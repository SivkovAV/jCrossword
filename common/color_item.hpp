#pragma once

// (c) Copyright Aleksey Sivkov 2017-2018

#include <vector>

namespace jcw
{

class ColorItem
{
public:
    ColorItem();
    ColorItem(double newValue);

    double operator()() const;
    void operator=(const ColorItem &jceColor);
    ColorItem operator+(const ColorItem &jceColor);
    ColorItem operator+(const double jceColor);
    ColorItem operator-(const ColorItem &jceColor);
    ColorItem operator-(const double jceColor);
    bool operator==(const double jceColor) const;
    bool operator==(const ColorItem &jceColor) const;
    void operator*=(double k);

    // ������ �������� ��������; realCount �� ����������� ����� dotColors.size() !!!
    static ColorItem mean(const std::vector<ColorItem> &jceColors, double realCount);
    // ������ �������� �������� ��� ���� ������
    static ColorItem mean(const ColorItem &jceColor1, const ColorItem &jceColor2);
    // ������ ����������
    static double lenght(const ColorItem &jceColor1, const ColorItem &jceColor2);

private:
    double value_;
};

} // namespace jcw