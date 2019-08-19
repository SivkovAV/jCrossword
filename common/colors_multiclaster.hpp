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
    // перерисовка рисунка цветами мультикластера;
    // при этом цвета кластеров фона заменяются первым из них
    void recalculateImage(Image &image, bool useFloidStainberg = false);

private:
    // все точки изображения добавляются в кластер
    void addImageDots(const Image &image);
    void removeAllDots();
    // возвращает среднее значение цветовой длины собственных точек до текущего центра
    double midLengthToCenter() const;
    // пересчет центров всех кластеров
    void recalculateCenters();
    // ищем точку, расположенную дальше других от центра своего кластера;
    // перемещаем эту точку в указанный кластер и делаем ее его центром
    void setMostFarDotAsClasterCenter(ColorsClaster& dotClaster);
    static ColorsMultiClaster createMultiClaster(const Image &image,
        size_t colorsCount, bool useFloidStainberg = false);
};

} // namespace jcw
