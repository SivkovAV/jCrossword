// (c) Copyright Aleksey Sivkov 2017-2018

#include "colors_claster.hpp"
#include <map>

using namespace jcw;

ColorsClaster::ColorsClaster(bool isFixedCenter)
    : isFixedCenter_(isFixedCenter)
{
}

ColorsClaster::ColorsClaster(Color& center, bool isFixedCenter)
    : center_(center)
    , isFixedCenter_(isFixedCenter)
{
}

void ColorsClaster::setCenter(const Color& _center)
{
    center_ = _center;
}

unsigned ColorsClaster::addDot(const Color& jcwDot, unsigned x, unsigned y)
{
    jcwDots_.push_back(jcwDot);
    return jcwDots_.size() - 1;
}

void ColorsClaster::eraseDot(unsigned dotIndex)
{
    if (dotIndex >= 0 && dotIndex < jcwDots_.size())
    {
        jcwDots_.erase(jcwDots_.begin() + dotIndex);
    }
}

void ColorsClaster::clear()
{
    jcwDots_.clear();
}

Color ColorsClaster::recalculateCenter()
{
    if (isFixedCenter_)
    {
        return center_;
    }
    center_ = Color::mean(jcwDots_, jcwDots_.size());
    return center_;
}

double ColorsClaster::lenghtToCenter(const Color& jcwDot) const
{
    return Color::lenght(center_, jcwDot);
}

double ColorsClaster::midLenghtToCenter() const
{
    if (jcwDots_.size() == 0)
    {
        return 0;
    }
    double midLenghtSum = 0;
    auto dotsCount = 0.;
    for (auto& jcwDot : jcwDots_)
    {
        ++dotsCount;
        midLenghtSum += lenghtToCenter(jcwDot);
    }
    return midLenghtSum / static_cast<double>(dotsCount);
}

size_t ColorsClaster::dotsCount() const
{
    return jcwDots_.size();
}

Color ColorsClaster::center() const
{
    return center_;
}

int ColorsClaster::farDotIndex() const
{
    int farDotIndex = 0;
    if (jcwDots_.size() == 0)
    {
        return -1;
    }

    int dotCount = jcwDots_.size();
    auto & farDot = jcwDots_[0];
    for (int i = 1; i < dotCount; ++i)
    {
        if (lenghtToCenter(jcwDots_[i]) > lenghtToCenter(jcwDots_[farDotIndex]))
        {
            farDotIndex = i;
        }
    }
    return farDotIndex;
}

bool ColorsClaster::isFixedCenter() const
{
    return isFixedCenter_;
}

Color ColorsClaster::dotByDotIndex(size_t index) const
{
    return jcwDots_.at(index);
}