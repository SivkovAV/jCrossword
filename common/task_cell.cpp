// (c) Copyright Aleksey Sivkov 2017

#include <task_cell.hpp>

using namespace jcw;

TaskCell::TaskCell()
    : value_(1)
    , color_(0)
    , isChecked_(false)
{
}

TaskCell::TaskCell(size_t value, ColorIndex color)
    : value_(value)
    , color_(color)
    , isChecked_(false)
{
}

bool TaskCell::isEqual(const TaskCell &cell) const
{
    const bool isEqualValue = cell.value() == value_;
    const bool isEqualColor = cell.color() == color_;
    const bool isEqualState = cell.isChecked() == isChecked_;
    return (isEqualValue && isEqualColor && isEqualState);
}

size_t TaskCell::value() const
{
    return value_;
}

ColorIndex TaskCell::color() const
{
    return color_;
}

bool TaskCell::isChecked() const
{
    return isChecked_;
}

void TaskCell::inc()
{
    value_++;
}

void TaskCell::setValue(size_t new_value)
{
    value_ = new_value;
}

void TaskCell::setColor(ColorIndex color)
{
    color_ = color;
}

bool TaskCell::setChecked(bool isChecked)
{
    if (isChecked_ == isChecked)
    {
        return false;
    }
    isChecked_ = isChecked;
    return true;
}

void TaskCell::invertChecking()
{
    isChecked_ = !isChecked_;
}