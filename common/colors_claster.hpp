#pragma once

// (c) Copyright Aleksey Sivkov 2017-2018

#include <color.hpp>
#include <game_cell.hpp>

namespace jcw
{

class ColorsClaster
{
public:
    ColorsClaster(bool isFixedCenter = false);
    ColorsClaster(Color& center, bool isFixedCenter = false);
    
    void setCenter(const Color &center);
    // добавление точки в кластер
    unsigned addDot(const Color &jcwDot, unsigned x = 0, unsigned y = 0);
    // удаление точки с номером dotIndex
    void eraseDot(unsigned dotIndex);
    // удаление всех точек с сохранением цвета центра
    void clear();
    // перерасчет центра кластера
    Color recalculateCenter();
    // расчет расстояния точки до центра кластера
    double lenghtToCenter(const Color& jcwDot) const;
    // возвращает среднее значение цветовой длины собственных точек до текущего центра
    double midLenghtToCenter() const;
    // возвращает кол-во точек в кластере
    size_t dotsCount() const;
    // возвращает копию цветового центра кластера    
    Color center() const;
    // возвращает номер точки кластера, наиболее удаленной от центра кластера
    int farDotIndex() const;
    // метод перемещает в текущий кластер и делает своим центром точку,
    // максимально удаленную от центра других кластеров
    //void setCenterFromFarDot(std::vector<ColorsClaster> &clasters);
    bool isFixedCenter() const;
    Color dotByDotIndex(size_t index) const;

private:
    std::vector<Color> jcwDots_;
    Color center_;
    // если true - центра данного кластера не нужно расчитывать (определен пользователем)
    bool isFixedCenter_ = false;                        
};

}

