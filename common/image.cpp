// (c) Copyright Aleksey Sivkov 2017-2018

#include <image.hpp>

using namespace jcw;

// коэффициенты важности оттенков цветов для челосека (наука)
static auto humanColorFactorForRed = 0.3;
static auto humanColorFactorForGreen = 0.59;
static auto humanColorFactorForBlue = 0.11;

Image::Image()
{
}

Image::Image(size_t height, size_t width)
{
    dots_.resize(height);
    for (auto &dotLine : dots_)
    {
        dotLine.resize(width);
    }
}

size_t Image::height() const
{
    return dots_.size();
}

size_t Image::width() const
{
    if (dots_.size() <= 0)
    {
        return 0;
    }
    return dots_[0].size();
}

void Image::setDot(size_t x, size_t y,
    double red, double green, double blue)
{
    dots_[y][x] = Color(red, green, blue);
}

void Image::setDotAARRGGBB(size_t x, size_t y, unsigned argb)
{
    dots_[y][x] = Color(
        (argb & 0xff0000u) >> 16u, (argb & 0xff00u) >> 8u, argb & 0xffu);
}

Color& Image::dot(size_t x, size_t y)
{
    if (dots_.size() <= y || dots_[y].size() <= x)
    {
        return Color();
    }
    return dots_[y][x];
}

Color Image::color(size_t x, size_t y) const
{
    if (dots_.size() <= y || dots_[y].size() <= x)
    {
        return Color();
    }
    return dots_.at(y).at(x);
}

unsigned Image::dotAARRGGBB(size_t x, size_t y) const
{
    if (dots_.size() <= y || dots_[y].size() <= x || x < 0 || y < 0)
    {
        return 0;
    }

    unsigned RR = (unsigned)dots_[y][x].getR(),
             GG = (unsigned)dots_[y][x].getG(),
             BB = (unsigned)dots_[y][x].getB();

    if (dots_[y][x].getR() - RR >= 0.5)
    {
        RR++;
    }
    
    if (dots_[y][x].getG() - GG >= 0.5)
    {
        GG++;
    }
    if (dots_[y][x].getB() - BB >= 0.5)
    {
        BB++;
    }
    unsigned AARRGGBB = 0xff000000 + BB;
    AARRGGBB += (GG << 8);
    AARRGGBB += (RR << 16);
    return AARRGGBB;
}

void Image::resize(size_t height, size_t width)
{
    // если изображение пустое - создаем его; если height==0 или width==0 - обнуление и завершение метода
    if (dots_.size() == 0)
    {
        dots_.resize(height);
    }
    if (dots_.size() == 0)
    {
        return;
    }
    for (auto &dotLine : dots_)
    {
        if (dotLine.size() == 0)
        {
            dotLine.resize(width);
        }
    }
    if (width == 0)
    {
        for (auto &dotLine : dots_)
        {
            dotLine.resize(0);
        }
        return;
    }

    // если изображение имеет строки неравной длины - выравниваем их по первой строке
    unsigned firstLineWidth = dots_[0].size();
    for (auto &dotLine : dots_)
    {
        if (dotLine.size() != firstLineWidth)
        {
            dotLine.resize(firstLineWidth);
        }
    }
    
    size_t oldHeight = dots_.size();
    size_t oldWidth = dots_[0].size();
    std::vector<std::vector<Color>> jcwDotsNew(height);

    const auto h = getElements(oldHeight, height);
    const auto w = getElements(oldWidth, width);

    for (size_t hi = 0; hi < height; hi++)
    {
        jcwDotsNew[hi].resize(width);
        for (size_t wi = 0; wi < width; wi++)
        {
            std::vector<Color> jcwDotsLocal;
            double count = 0;
            for (auto& hl : h[hi])
            {
                for (auto& wl : w[wi])
                {
                    jcwDotsLocal.push_back(dots_[hl.index][wl.index] * hl.weight * wl.weight);
                    count += hl.weight * wl.weight;
                }
            }

            jcwDotsNew[hi][wi] = Color::mean(jcwDotsLocal, count);
        }
    }
    dots_ = jcwDotsNew;
}

void Image::decreaseSize(unsigned kTimes)
{
    // если изображение пустое - создаем его; если height==0 или width==0 - обнуление и завершение метода
    if (kTimes < 1 || dots_.size() == 0)
    {
        return;
    }
    for (auto &dotLine : dots_)
    {
        if (dotLine.size() == 0)
        {
            return;
        }
    }

    // если изображение имеет строки неравной длины - выравниваем их по первой строке
    unsigned firstLineWidth = dots_[0].size();
    for (auto &dotLine : dots_)
    {
        if (dotLine.size() != firstLineWidth)
        {
            dotLine.resize(firstLineWidth);
        }
    }

    const unsigned oldHeight = dots_.size();
    const unsigned oldWidth = dots_[0].size();
    const auto height = oldHeight / kTimes;
    const auto width = oldWidth / kTimes;

    std::vector<std::vector<Color>> result(height);
    for (auto& dotsLine : result)
    {
        dotsLine.resize(width);
    }

    for (int i = height-1; i>=0; i--)
    {
        if (i * kTimes < oldHeight)
        {
            for (int j = width - 1; j >= 0; j--)
            {
                result[i][j] = dots_[i*kTimes][j*kTimes];
            }
        }
    }
    dots_ = result;
}

void Image::increasePixelsByDot(size_t times)
{
    const auto height = this->height();
    const auto width = this->width();

    if (height <= 0 || width <= 0)
    {
        return;
    }

    std::vector<std::vector<Color>> jcwDotsNew(height * times);
    for (auto &dotsLine : jcwDotsNew)
    {
        dotsLine.resize(width * times);
    }

    for (size_t y = 0; y < height; ++y)
    {
        for (size_t x = 0; x < width; ++x)
        {
            for (size_t k = 0; k < times; ++k)
            {
                for (size_t l = 0; l < times; ++l)
                {
                    const size_t newY = times * y + k;
                    const size_t newX = times * x + l;
                    jcwDotsNew[newY][newX] = dots_[y][x];
                }
            }
        }
    }

    dots_ = jcwDotsNew;
}

void Image::transformToGrey()
{
    const auto xSize = dots_.size();
    for (size_t i = 0; i < xSize; ++i)
    {
        const auto ySize = dots_[i].size();
        for (size_t j = 0; j < ySize; ++j)
        {
            const auto &dot = dots_[i][j];
            const auto grey =
                dot.getB() * humanColorFactorForBlue +
                dot.getG() * humanColorFactorForGreen +
                dot.getR() * humanColorFactorForRed;
            dots_[i][j] = Color(grey, grey, grey);
        }
    }
}

void Image::trimBorders(double percentValue)
{
    // trim по вертикали
    const auto height = static_cast<double>(dots_.size());
    const auto verticalTrimSizeDouble = round(height * percentValue);
    if (height - 2 * verticalTrimSizeDouble <= 0.)
    {
        return;
    }
    const auto verticalTrimSize = static_cast<size_t>(verticalTrimSizeDouble);
    dots_.erase(dots_.begin(), dots_.begin() + verticalTrimSize);
    dots_.erase(dots_.end() - verticalTrimSize, dots_.end());

    // trim по горизонтали
    const auto width = static_cast<double>(dots_[0].size());
    const auto horizontalTrimSizeDouble = round(width * percentValue);
    if (width - 2 * horizontalTrimSizeDouble <= 0.)
    {
        return;
    }
    const auto horizontalTrimSize = static_cast<size_t>(horizontalTrimSizeDouble);
    for (auto &line : dots_)
    {
        line.erase(line.begin(), line.begin() + horizontalTrimSize);
        line.erase(line.end() - horizontalTrimSize, line.end());
    }
}

void Image::trimMonohromeLines()
{
    if (dots_.size() == 0)
    {
        return;
    }

    const auto xSize = dots_[0].size();
    if (xSize == 0)
    {
        return;
    }
    const auto color = dots_[0][0];
    trimHorizontalBorders(color, xSize, true);
    trimHorizontalBorders(color, xSize, false);

    const auto ySize = dots_.size();
    if (ySize == 0)
    {
        return;
    }
    trimVerticalLines(color, ySize);
}

bool Image::isValideColors()
{
    for (auto &dotLine : dots_)
    {
        for (auto &dot : dotLine)
        {
            if (dot.getR() < 0 || dot.getR() > 255 ||
                dot.getG() < 0 || dot.getG() > 255 ||
                dot.getB() < 0 || dot.getB() > 255)
            {
                return false;
            }
        }
    }
    return true;
}

Image Image::copy_(size_t leftTopX, size_t leftTopy, size_t width, size_t height)
{
    Image result(height, width);
    for (auto y = 0; y < height; ++y)
    {
        for (auto x = 0; x < width; ++x)
        {
            result.dots_[y][x] = dots_[y + leftTopy][x + leftTopX];
        }
    }
    return result;
}

void Image::trimHorizontalBorders(const Color &color, size_t length, bool isTop)
{
    auto index = isTop ? 0 : (dots_.size() - 1);
    bool isErased = true;
    while (isErased && dots_.size() > 0)
    {
        bool isSingleColor = true;
        for (size_t i = 0; i < length && isSingleColor; ++i)
        {
            if (!(dots_[index][i] == color))
            {
                isSingleColor = false;
            }
        }
        if (isSingleColor)
        {
            if (isTop)
            {
                dots_.erase(dots_.begin(), dots_.begin() + 1);
            }
            else
            {
                dots_.erase(dots_.end() - 1, dots_.end());
                --index;
            }
            isErased = true;
        }
        else
        {
            isErased = false;
        }
    }
}

void Image::trimVerticalLines(const Color &color, size_t length)
{
    for (auto index = 0; index < dots_[0].size() && dots_[0].size() > 0; ++index)
    {
        bool isSingleColor = true;
        for (size_t i = 0; i < length && isSingleColor; ++i)
        {
            if (!(dots_[i][index] == color))
            {
                isSingleColor = false;
            }
        }
        if (isSingleColor)
        {
            for (size_t i = 0; i < length && isSingleColor; ++i)
            {
                dots_[i].erase(dots_[i].begin() + index, dots_[i].begin() + index + 1);
            }
            --index;
        }
    }
}

std::vector<std::vector<Image::ImageElement>> Image::getElements(
    size_t oldSize, size_t newSize)
{
    std::vector<std::vector<ImageElement>> result(newSize);
    for (size_t i = 0; i < newSize; i++)
    {
        std::vector<Color> jcwDotsLocal;

        int indexCorrection = 0;
        double k = 1 - (((1.0 * oldSize / newSize) * i) - ((unsigned)((1.0 * oldSize / newSize) * i)));
        if (k < 1)
        {
            indexCorrection++;
            result[i].push_back({ (unsigned)((1.0 * oldSize / newSize) * i), k });
        }
        for (int j = (1.0 * oldSize / newSize) * i + indexCorrection; j < (unsigned)((1.0 * oldSize / newSize) * (i + 1)); j++)
        {
            result[i].push_back({ static_cast<unsigned>(j), 1 });
        }
        //if (i < newSize - 1)    // теоретически это условие лишнее; практически из-за неточности вычислений в формате double это позволяет отфильтровать попадание невалидных индексов в массив result
        {
            k = (1.0 * oldSize / newSize) * (i + 1) - (unsigned)((1.0 * oldSize / newSize) * (i + 1));
            if (k > 0.00001)    // теоретически достаточно сравнить с нулем; практически из-за неточности вычислений в формате double это позволяет отфильтровать попадание невалидных индексов в массив result
            {
                const auto oldSize_ = static_cast<double>(oldSize);
                const auto newSize_ = static_cast<double>(newSize);
                const auto k_ = oldSize_ / newSize_;
                const auto index = static_cast<unsigned>(k_ * (i + 1u));
                result[i].push_back({ index, k });
            }
        }
    }
    return result;
}