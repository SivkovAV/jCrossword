#pragma once

// (c) Copyright Aleksey Sivkov 2017
/*
    Класс реализует объект клетки таблицы задания.
*/

#include <color_index.hpp>

namespace jcw
{

class TaskCell
{
public:
    TaskCell();
    TaskCell(size_t value, ColorIndex color = ColorIndex());
    bool isEqual(const TaskCell &cell) const;
    size_t value() const;
    ColorIndex color() const;
    bool isChecked() const;

    void inc(); // value++
    void setValue(size_t value);
    void setColor(ColorIndex color);
    bool setChecked(bool isChecked);
    void invertChecking();

private:
    size_t value_;
    ColorIndex color_;
    bool isChecked_;
};

} // namespace jcw
