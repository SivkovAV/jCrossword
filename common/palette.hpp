#pragma once

// (c) Copyright Aleksey Sivkov 2017

#include <QColor>
#include <vector>

namespace jcw
{

class Palette
{
public:
    Palette(size_t mainColorsCount = 1u);
    void reset(size_t mainColorsCount = 1u);

    static std::string toString(QColor color);
    static QColor fromString(std::string string);

    std::vector<QColor> back; // ����� ����
    std::vector<QColor> main; // ����� ����������� ������
    QColor gameBack;          // ���� ���� ����������
    QColor taskBack;          // ���� ���� � ������� �������
    QColor taskText;          // ���� ������ � ������� �������
    QColor lines;             // ����� �����
};

} // namespace jcw