#pragma once

// (c) Copyright Aleksey Sivkov 2017-2018

#include <color.hpp>

namespace jcw
{

class Image
{
public:
    Image();
    Image(size_t height, size_t width);

    size_t height() const;
    size_t width() const;

    void setDot(size_t x, size_t y, double R, double G, double B);
    void setDotAARRGGBB(size_t x, size_t y, unsigned AARRGGBB);
    Color& dot(size_t x, size_t y);
    Color color(size_t x, size_t y) const;
    unsigned dotAARRGGBB(size_t x, size_t y) const;

    // ��������� ������� � ���������� ����� ������ ����� ������
    void resize(size_t height, size_t width);
    // ���������� ������� ����������� � kTimes ���
    void decreaseSize(unsigned kTimes);
    // ����������� ���-�� �������� � times ���
    void increasePixelsByDot(size_t times = 20);
    // �������������� ����������� � �����-������ 
    void transformToGrey();
    // �������� percentValue-��������� �������� �� �����
    void trimBorders(double percentValue);
    // ������� �������� ����������� ������� (������ ���� ����� dots_[0][0])
    void trimMonohromeLines();      

    // �������� ���������� ������� ������� ���
    // ������� ����� �������� ����� �� ���� ������
    bool isValideColors();

    Image copy_(size_t leftTopX, size_t leftTopy, size_t width, size_t height);

private:
    void trimHorizontalBorders(const Color &color, size_t length, bool isTop);
    void trimVerticalLines(const Color &color, size_t length);

    struct ImageElement
    {
        unsigned index;
        double weight;
    };
    std::vector <std::vector<ImageElement>> getElements(
        size_t oldSize, size_t newSize);

    std::vector<std::vector<Color>> dots_; // [������][������]
};

} // namespace jcw

