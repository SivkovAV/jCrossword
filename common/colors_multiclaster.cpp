// (c) Copyright Aleksey Sivkov 2018

#include <colors_multiclaster.hpp>
#include <map>

using namespace jcw;

ColorsMultiClaster::ColorsMultiClaster()
{
}

ColorsMultiClaster::ColorsMultiClaster(
    const std::vector<ColorsClaster> &dotsClaster)
    : std::vector<ColorsClaster>{dotsClaster}
{
}

ColorsMultiClaster::ColorsMultiClaster(size_t colorsCount)
    : std::vector<ColorsClaster>(colorsCount)
{
}

ColorsMultiClaster::ColorsMultiClaster(Image &image, size_t colorsCount,
    bool useFloidStainberg)
{
    *this = createMultiClaster(image, colorsCount, useFloidStainberg);
}

Palette ColorsMultiClaster::palette() const
{
    const auto colorsCount = size();
    if (colorsCount == 0)
    {
        return jcw::Palette();
    }

    jcw::Palette palette;

    palette.back.resize(2);
    const auto backgroundColor = this->at(0).center().color();
    palette.back[0] = backgroundColor;
    palette.back[1] = backgroundColor;

    const auto mainColorsCount = colorsCount - 1;
    palette.main.resize(mainColorsCount);
    for (size_t i = 0; i < mainColorsCount; ++i)
    {
        auto qColor = this->at(i + 1).center().color();
        palette.main[i] = qColor;
    }

    return palette;
}

const int ColorsMultiClaster::biggestClasterIndex() const
{
    const auto colorsCount = this->size();
    if (colorsCount == 0)
    {
        return -1;
    }
    auto index = 0;
    auto dotsCount = 0;
    for (size_t i = 0; i < colorsCount; ++i)
    {
        const auto localDotsCount = this->at(i).dotsCount();
        if (localDotsCount > dotsCount)
        {
            dotsCount = localDotsCount;
            index = i;
        }
    }
    return index;
}

void ColorsMultiClaster::recalculateImage(Image &image, bool useFloidStainberg)
{
    if (size() == 0)
    {
        return;
    }

    const auto height = image.height();
    const auto width = image.width();
    for (size_t y = 0; y < height; ++y)
    {
        for (size_t x = 0; x < width; ++x)
        {
            auto bestClaster = &(this->at(0));
            auto minDistance = bestClaster->lenghtToCenter(image.dot(x, y));
            for (auto& claster : *this)
            {
                const auto distance = claster.lenghtToCenter(image.dot(x, y));
                if (distance < minDistance)
                {
                    minDistance = distance;
                    bestClaster = &(claster);
                }
            }
            if (useFloidStainberg == false)
            {
                auto &dot = image.dot(x, y);
                auto center = bestClaster->center();
                image.dot(x, y) = bestClaster->center();
            }
            else
            {
                const auto error = bestClaster->center() - image.dot(x, y);
                image.dot(x, y) = bestClaster->center();

                const auto k = sqrt(2.0) / (2.0*(sqrt(2.0) + 1.0));
                const auto k2 = k / sqrt(2.0);
                if (x + 1 < width)
                {
                    image.dot(x + 1, y) += error * k;
                }
                if (y + 1 < width && x + 1 < width)
                {
                    image.dot(x + 1, y + 1) += error * k2;
                }
                if (y + 1 < width)
                {
                    image.dot(x, y + 1) += error * k;
                }
                if (y + 1 < width && x - 1 >= 0)
                {
                    image.dot(x - 1, y + 1) += error * k2;
                }
            }
        }
    }
}

void ColorsMultiClaster::addImageDots(const Image &image)
{
    // значение, заведомо большее максимально возможного расстояния между цветами
    const double maxColorLength = 256.0 * sqrt(3.0) + 1;
    const size_t height = image.height();
    const size_t width = image.width();
    auto &clasters = *((std::vector<ColorsClaster>*)this);
    for (size_t x = 0; x < width; x++)
    {
        for (size_t y = 0; y < height; y++)
        {

            auto bestClaster = &clasters[0];
            const auto dotColor = image.color(x, y);
            double minColorLength = maxColorLength;
            for (auto& claster : clasters)
            {
                const auto colorLength = claster.lenghtToCenter(dotColor);
                if (colorLength < minColorLength)
                {
                    minColorLength = colorLength;
                    bestClaster = &claster;
                }
            }
            bestClaster->addDot(dotColor, y, x);
        };
    }
}

void ColorsMultiClaster::removeAllDots()
{
    for (auto &claster : *this)
    {
        claster.clear();
    }
}

double ColorsMultiClaster::midLengthToCenter() const
{
    double length = 0;
    size_t allDotsCount = 0u;
    for (const auto &claster : *this)
    {
        const size_t dotsCount = claster.dotsCount();
        allDotsCount += dotsCount;
        length += claster.midLenghtToCenter() * static_cast<double>(dotsCount);
    }
    return length / static_cast<double>(allDotsCount);
}

void ColorsMultiClaster::recalculateCenters()
{
    for (auto& claster : *this)
    {
        claster.recalculateCenter();
    }
}

void ColorsMultiClaster::setMostFarDotAsClasterCenter(ColorsClaster& dotClaster)
{
    // todo: refactor this
    if (dotClaster.isFixedCenter())
    {
        return;
    }
    // size_t - номер кластера, int - номер его наиболее удаленной от центра точки
    std::map<size_t, int> farDots;
    const auto clasterCount = this->size();
    for (size_t i = 0; i < clasterCount; ++i)
    {
        if (this->at(i).dotsCount() > 0)
        {
            farDots[i] = this->at(i).farDotIndex();
        }
    }
    if (farDots.size() < 1)
    {
        return;
    }

    auto farDotClasterIndex = farDots.begin()->first;
    auto farDotIndex = farDots.begin()->second;
    const auto &farClaster = (*this)[farDotClasterIndex];
    const auto firstDotColor = farClaster.dotByDotIndex(farDotIndex);
    auto lengthToFarClasterCenter = farClaster.lenghtToCenter(firstDotColor);
    for (auto & dot : farDots)
    {
        const auto &claster = (*this)[dot.first];
        const auto dotColor = claster.dotByDotIndex(dot.second);
        const auto lengthToClasterCenter = claster.lenghtToCenter(dotColor);
        if (lengthToClasterCenter > lengthToFarClasterCenter)
        {
            farDotClasterIndex = dot.first;
            farDotIndex = dot.second;
            lengthToFarClasterCenter = lengthToClasterCenter;
        }
    };

    auto &farrestDotClaster = (*this)[farDotClasterIndex];
    const auto dotColor = farrestDotClaster.dotByDotIndex(farDotIndex);
    farrestDotClaster.eraseDot(farDotIndex);
    dotClaster.setCenter(dotColor);
    dotClaster.addDot(dotColor);
}

ColorsMultiClaster ColorsMultiClaster::createMultiClaster(
    const Image &image, size_t colorsCount, bool useFloidStainberg)
{
    const auto height = image.height();
    const auto width = image.width();
    if (height <= 0 && width <= 0 || colorsCount < 1)
    {
        return ColorsMultiClaster();
    }
    ColorsMultiClaster clasters(colorsCount);

    // предварительное размещение точек по кластерам
    for (size_t y = 0; y < height; ++y)
    {
        for (size_t x = 0; x < width; ++x)
        {
            const auto clasterIndex = (y * width + x) % colorsCount;
            clasters.at(clasterIndex).addDot(image.color(x, y), y, x);
        };
    }

    clasters.recalculateCenters();
    if (colorsCount >= height * width)
    {
        return clasters;
    }

    const int colorBorder = 10;
    ColorsMultiClaster prewClaster(colorsCount);
    double midLenght = 0, midLenghtPrew = 1;
    while (midLenght < midLenghtPrew)
    {
        // переразмещение пустых кластеров : если кластер пуст - размещаем
        // его центр в точке, наиболее удаленной от центра своего кластера
        for (auto it = clasters.begin(); it != clasters.end(); ++it)
        {
            if (it->dotsCount() == 0)
            {
                clasters.setMostFarDotAsClasterCenter(*it);
            }
            for (auto localIt = clasters.begin(); localIt != it; ++localIt)
            {
                if (localIt->lenghtToCenter(it->center()) <= colorBorder)
                {
                    clasters.setMostFarDotAsClasterCenter(*it);
                }
            }
        }

        clasters.removeAllDots();
        clasters.addImageDots(image);
        prewClaster = clasters;
        midLenghtPrew = clasters.midLengthToCenter();
        clasters.recalculateCenters();
        midLenght = clasters.midLengthToCenter();
    }
    clasters = prewClaster;

    return clasters;
}