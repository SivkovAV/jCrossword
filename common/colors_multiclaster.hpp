#pragma once

// (c) Copyright Aleksey Sivkov 2018

#include <colors_claster.hpp>
#include <palette.hpp>
#include <image.hpp>

namespace jcw
{

class ColorsMultiClaster : public std::vector<ColorsClaster>
{
public:
    ColorsMultiClaster();
    ColorsMultiClaster(const std::vector<ColorsClaster> &dotsClaster);
    ColorsMultiClaster(size_t colorsCount);
    ColorsMultiClaster(Image &image, size_t colorsCount,
        bool useFloidStainberg = false);

    Palette palette() const;
    const int biggestClasterIndex() const;
    // ����������� ������� ������� ��������������;
    // ��� ���� ����� ��������� ���� ���������� ������ �� ���
    void recalculateImage(Image &image, bool useFloidStainberg = false);

private:
    // ��� ����� ����������� ����������� � �������
    void addImageDots(const Image &image);
    void removeAllDots();
    // ���������� ������� �������� �������� ����� ����������� ����� �� �������� ������
    double midLengthToCenter() const;
    // �������� ������� ���� ���������
    void recalculateCenters();
    // ���� �����, ������������� ������ ������ �� ������ ������ ��������;
    // ���������� ��� ����� � ��������� ������� � ������ �� ��� �������
    void setMostFarDotAsClasterCenter(ColorsClaster& dotClaster);
    static ColorsMultiClaster createMultiClaster(const Image &image,
        size_t colorsCount, bool useFloidStainberg = false);
};

} // namespace jcw
