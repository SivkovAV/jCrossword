#pragma once

// (c) Copyright Aleksey Sivkov 2017
/*
    Файл описывает тип "Номер Цвета".
Номер цвета ставится в соответствие с каким-либо реальным цветом
только в связке с объектом типа "Палитра" (Palette.hpp)
*/

#include "stdint.h"

namespace jcw
{

// тип "Номер цвета клетки";
typedef uint8_t ColorIndex;

} // namespace jcw