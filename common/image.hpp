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

    // изменение размера с пересчетом цвета каждой новой клетки
    void resize(size_t height, size_t width);
    // уменьшение размера изображени¤ в kTimes раз
    void decreaseSize(unsigned kTimes);
    // увеличивает кол-во пикселов в times раз
    void increasePixelsByDot(size_t times = 20);
    // преобразование изображение к черно-белому 
    void transformToGrey();
    // обрезает percentValue-процентов картинки по кра¤м
    void trimBorders(double percentValue);
    // обрезка удаление одноцветной границы (ищетс¤ цвет точки dots_[0][0])
    void trimMonohromeLines();      

    // проверка отсутстви¤ слишком больших или
    // слишком малых значений цвета во всех точках
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

    std::vector<std::vector<Color>> dots_; // [высота][ширина]
};

} // namespace jcw

