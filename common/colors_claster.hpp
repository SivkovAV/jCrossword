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
    // ���������� ����� � �������
    unsigned addDot(const Color &jcwDot, unsigned x = 0, unsigned y = 0);
    // �������� ����� � ������� dotIndex
    void eraseDot(unsigned dotIndex);
    // �������� ���� ����� � ����������� ����� ������
    void clear();
    // ���������� ������ ��������
    Color recalculateCenter();
    // ������ ���������� ����� �� ������ ��������
    double lenghtToCenter(const Color& jcwDot) const;
    // ���������� ������� �������� �������� ����� ����������� ����� �� �������� ������
    double midLenghtToCenter() const;
    // ���������� ���-�� ����� � ��������
    size_t dotsCount() const;
    // ���������� ����� ��������� ������ ��������    
    Color center() const;
    // ���������� ����� ����� ��������, �������� ��������� �� ������ ��������
    int farDotIndex() const;
    // ����� ���������� � ������� ������� � ������ ����� ������� �����,
    // ����������� ��������� �� ������ ������ ���������
    //void setCenterFromFarDot(std::vector<ColorsClaster> &clasters);
    bool isFixedCenter() const;
    Color dotByDotIndex(size_t index) const;

private:
    std::vector<Color> jcwDots_;
    Color center_;
    // ���� true - ������ ������� �������� �� ����� ����������� (��������� �������������)
    bool isFixedCenter_ = false;                        
};

}

