// (c) Copyright Aleksey Sivkov 2017

#include <painter.hpp>
#include <palette.hpp>
#include <QImage>
#include <QPainter>
#include <QPen>
#include <QCoreApplication>
#include <generators.hpp>

using namespace jcw;

void drawBackground(QPainter &painter, const Palette &palette,
    const GameLine &game, const TaskLine &task, bool isHorizontal,
    size_t lineNumber, size_t cellsOffset, bool isInverceBackground)
{
    const size_t cellsCount = task.size() + game.size();
    const size_t penWidth = painter.pen().width();
    const size_t rectSideSize = Painter::cellSize + 2 * Painter::borderSize - penWidth;
    const size_t xCellBegin = penWidth / 2 + cellsOffset * Painter::meanCellSize;
    const size_t yCellBegin = penWidth / 2 + lineNumber * Painter::meanCellSize;
    for (size_t i = 0; i < cellsCount; i++)
    {
        if (i >= task.size())
        {
            const bool isPrimaryColor = ((i - task.size()) / Painter::groupCellsCount) % 2 == 0;
            const size_t index = isPrimaryColor == isInverceBackground ? 1 : 0;
            painter.setBrush(QBrush(palette.back[index]));
        }
        const size_t xCell = xCellBegin + i * Painter::meanCellSize;
        const size_t x = isHorizontal ? xCell : yCellBegin;
        const size_t y = isHorizontal ? yCellBegin : xCell;
        painter.drawRect(x, y, rectSideSize, rectSideSize);
    }
}

void drawTaskCells(QPainter &painter, const Palette &palette,
    const GameLine &game, const TaskLine &task, bool isHorizontal,
    size_t lineNumber, size_t cellsOffset, bool isInverceBackground)
{
    const size_t cellsCount = task.size() + game.size();
    const size_t penWidth = painter.pen().width();
    const size_t rectSideSize = Painter::cellSize + 2 * Painter::borderSize - penWidth;
    const size_t xCellBegin = penWidth / 2 + cellsOffset * Painter::meanCellSize;
    const size_t yCellBegin = penWidth / 2 + lineNumber * Painter::meanCellSize;

    painter.setFont(QFont("Times", (Painter::cellSize - 2) / 2, QFont::Bold));
    for (size_t i = 0; i < task.size(); i++)
    {
        const auto value =
            QString::fromStdString(std::to_string(task[i].value()));

        // надпись
        const size_t xCell = xCellBegin + i * Painter::meanCellSize;
        const size_t yCell = yCellBegin + round(penWidth / 2.);
        const size_t x = isHorizontal ? xCell + 1 : yCell;
        const size_t y = isHorizontal ? yCell - 1 : xCell;
        painter.drawText(x, y, rectSideSize, rectSideSize, Qt::AlignCenter, value);

        // крест (отметка о завершении отрисовки)
        if (task[i].isChecked())
        {
            const size_t x = xCellBegin + i * Painter::meanCellSize;
            const size_t y = yCellBegin;
            const size_t x2 = x + rectSideSize;
            const size_t y2 = y + rectSideSize;
            if (isHorizontal)
            {
                painter.drawLine(x, y, x2, y2);
                painter.drawLine(x, y2, x2, y);
            }
            else
            {
                painter.drawLine(y, x, y2, x2);
                painter.drawLine(y2, x, y, x2);
            }
        }
    }

    painter.setFont(QFont("Times", (Painter::cellSize - 2) / 2, QFont::Bold));
    for (size_t i = 0; i < task.size(); i++)
    {
        const auto value =
            QString::fromStdString(std::to_string(task[i].value()));

        // надпись
        const size_t xCell = xCellBegin + i * Painter::meanCellSize;
        const size_t yCell = yCellBegin + round(penWidth / 2.);
        const size_t x = isHorizontal ? xCell + 1 : yCell;
        const size_t y = isHorizontal ? yCell - 1 : xCell;
        painter.drawText(x, y, rectSideSize, rectSideSize, Qt::AlignCenter, value);

        // крест
        if (task[i].isChecked())
        {
            const size_t x = xCellBegin + i * Painter::meanCellSize;
            const size_t y = yCellBegin;
            const size_t x2 = x + rectSideSize;
            const size_t y2 = y + rectSideSize;
            if (isHorizontal)
            {
                painter.drawLine(x, y, x2, y2);
                painter.drawLine(x, y2, x2, y);
            }
            else
            {
                painter.drawLine(y, x, y2, x2);
                painter.drawLine(y2, x, y, x2);
            }
        }
    }
}

void drawGameCells(QPainter &painter, const Palette &palette,
    const GameLine &game, const TaskLine &task, bool isHorizontal,
    size_t lineNumber, size_t cellsOffset, bool isInverceBackground)
{
    const size_t cellsCount = task.size() + game.size();
    const size_t penWidth = painter.pen().width();
    const size_t rectSideSize = Painter::cellSize + 2 * Painter::borderSize - penWidth;
    const size_t xCellBegin = penWidth / 2 + cellsOffset * Painter::meanCellSize;
    const size_t yCellBegin = penWidth / 2 + lineNumber * Painter::meanCellSize;

    const size_t xShift = task.size() * Painter::meanCellSize + xCellBegin;
    for (size_t i = 0; i < game.size(); i++)
    {
        const size_t x = xShift + i * Painter::meanCellSize;
        const size_t y = yCellBegin;
        if (game[i].isPainted())
        {
            const auto color = game[i].color();
            const auto colorIndex = palette.main.size() > color ? color : 0;
            const auto paletteColor = palette.main[colorIndex];
            painter.setBrush(QBrush(paletteColor));
            if (isHorizontal)
            {
                painter.drawRect(x, y, rectSideSize, rectSideSize);
            }
            else
            {
                painter.drawRect(y, x, rectSideSize, rectSideSize);
            }
            painter.setBrush(QBrush(palette.back[0]));
        }
        else if (game[i].isEmpty())
        {
            const size_t x2 = x + rectSideSize;
            const size_t y2 = y + rectSideSize;
            if (isHorizontal)
            {
                painter.drawLine(x, y, x2, y2);
                painter.drawLine(x, y2, x2, y);
            }
            else
            {
                painter.drawLine(y, x, y2, x2);
                painter.drawLine(y2, x, y, x2);
            }
        }
    }
}

void drawLines(QPainter &painter, const Palette &palette,
    const GameLine &game, const TaskLine &task, bool isHorizontal,
    size_t lineNumber, size_t cellsOffset, bool isInverceBackground)
{
    const size_t penWidth = painter.pen().width();
    const size_t rectSideSize = Painter::cellSize + 2 * Painter::borderSize - penWidth;
    const size_t xCellBegin = penWidth / 2 + cellsOffset * Painter::meanCellSize;
    const size_t yCellBegin = penWidth / 2 + lineNumber * Painter::meanCellSize;

    const size_t count = (game.size() > 0 ? (game.size() - 1)
        / Painter::groupCellsCount : 0) + 1;
    for (size_t i = 0; i < count; i++)
    {
        const size_t cellNumber = Painter::groupCellsCount * i + task.size();
        const size_t pixelWidth = cellNumber * Painter::meanCellSize;
        const size_t x = pixelWidth + xCellBegin - penWidth;
        const size_t y = yCellBegin;
        if (isHorizontal)
        {
            painter.drawLine(x, 0, x, y + rectSideSize);
        }
        else
        {
            painter.drawLine(0, x, y + rectSideSize, x);
        }
    }
}

void drawGameLine(QImage &image, const Palette &palette,
    const GameLine &game, const TaskLine &task, bool isHorizontal = true,
    size_t lineNumber = 0, size_t cellsOffset = 0,
    bool isInverceBackground = false)
{
    QPainter painter(&image);
    painter.setBrush(QBrush(palette.taskBack));
    painter.setPen(QPen(QBrush(palette.lines), Painter::borderSize,
        Qt::PenStyle::SolidLine));

    // общий фон
    drawBackground(painter, palette, game, task,
        isHorizontal, lineNumber, cellsOffset, isInverceBackground);

    // клетки задания
    drawTaskCells(painter, palette, game, task,
        isHorizontal, lineNumber, cellsOffset, isInverceBackground);

    // игровые клетки
    drawGameCells(painter, palette, game, task,
        isHorizontal, lineNumber, cellsOffset, isInverceBackground);

    // разделительные линии
    drawLines(painter, palette, game, task,
        isHorizontal, lineNumber, cellsOffset, isInverceBackground);
}

void drawBorders(QImage& image, const Palette &palette,
    const size_t gameWidth, const size_t gameHeight, 
    const size_t leftTaskMaxLenght, const size_t topTaskMaxLenght)
{
    QPainter painter(&image);
    painter.setBrush(QBrush(palette.taskBack));
    painter.setPen(QPen(QBrush(palette.lines), Painter::borderSize,
        Qt::PenStyle::SolidLine));
    const auto HalfOfPenWidth = painter.pen().width() / 2;

    const size_t verticalLinesCount =
        (gameWidth > 0 ? (gameWidth - 1) / Painter::groupCellsCount : 0) + 1;
    const size_t horizontalLinesCount =
        (gameHeight > 0 ? (gameHeight - 1) / Painter::groupCellsCount : 0) + 1;

    const auto height = image.height() - 1;
    const auto width = image.width() - 1;
    for (size_t i = 0; i < verticalLinesCount; i++)
    {
        const size_t firstCellIndex = Painter::groupCellsCount * i + leftTaskMaxLenght;
        const size_t x = firstCellIndex * Painter::meanCellSize - HalfOfPenWidth;
        painter.drawLine(x, 0, x, height);
        painter.drawLine(x + 1, 0, x + 1, height);
    }
    
    for (size_t i = 0; i < horizontalLinesCount; i++)
    {
        const size_t firstCellIndex = Painter::groupCellsCount * i + topTaskMaxLenght;
        const size_t y = firstCellIndex * Painter::meanCellSize - HalfOfPenWidth;
        painter.drawLine(0, y, width, y);
        painter.drawLine(0, y + 1, width, y + 1);
    }
}

Painter::Painter()
{
}

std::string Painter::draw(const GameLine &game, const TaskLine &task,
    std::string filename)
{
    checkFilename(filename);

    const auto allCellsCount = task.size() + game.size();
    const auto width = allCellsCount * (cellSize + borderSize) + borderSize;
    const auto height = cellSize + 2 * borderSize;

    const Palette palette(game.maxColorsCount());
    QImage image = QImage(width, height, QImage::Format::Format_ARGB32);

    drawGameLine(image, palette, game, task);

    bool result = image.save(QString::fromStdString(filename), "png");
    return filename;
}

void Painter::draw(const Crossword &crossword, std::string filename)
{
    checkFilename(filename);

    const auto game = crossword.game();
    const auto leftTask = crossword.leftTask();
    const auto topTask = crossword.topTask();
    const auto verticalTaskShift = crossword.topTaskHeight();
    const auto horizontalTaskShift = crossword.leftTaskWidth();
    const auto verticalCellsGameCellsCount = game.size();
    const auto horizontalGameCellsCount = game.size() > 0 ? game[0].size() : 0;
    const auto verticalCellsCount = 
        verticalTaskShift + verticalCellsGameCellsCount;
    const auto horizontalCellsCount =
        horizontalTaskShift + horizontalGameCellsCount;

    const auto width = horizontalCellsCount * meanCellSize + borderSize;
    const auto height =  verticalCellsCount * meanCellSize + borderSize;

    const Palette palette(crossword.maxColorsCount());
    QImage image = QImage(width, height, QImage::Format::Format_ARGB32);

    for (size_t index = 0; index < leftTask.size(); ++index)
    {
        size_t lineNumber = verticalTaskShift + index;
        bool isInverceBackground = index / groupCellsCount % 2;
        drawGameLine(image, palette,
            game[index], leftTask[index], true, lineNumber,
            horizontalTaskShift - leftTask[index].size(), isInverceBackground);
    }

    for (size_t index = 0; index < topTask.size(); ++index)
    {
        drawGameLine(image, palette,
            GameLine(), topTask[index], false, horizontalTaskShift + index,
            verticalTaskShift - topTask[index].size());
    }

    drawBorders(image, palette,
        horizontalGameCellsCount, verticalCellsGameCellsCount,
        horizontalTaskShift, verticalTaskShift);

    bool result = image.save(QString::fromStdString(filename), "png");
}

void Painter::draw(const Palette &palette, std::string filename)
{
    checkFilename(filename);

    const size_t backgroundColorsCount = palette.back.size();
    const size_t mainColorsCount = palette.main.size();
    const size_t width = std::max(backgroundColorsCount, mainColorsCount);
    // 2 строки, т.к. вторая строка вуализирует цвета фона пустых клеток;
    // 6 столбцов, т.к. тест должен охватить оба варианта цвета фона;
    Game game(2, std::max(6u, width ));
    for (size_t i = 0; i < palette.main.size(); i++)
    {
        game[0][i].setPainted((ColorIndex)i);
    }
    draw(Crossword(game), filename);
}

void Painter::checkFilename(std::string &filename)
{
    if (filename == "")
    {
        auto qFilename = QCoreApplication::applicationFilePath();
        const auto dotIndex = qFilename.lastIndexOf(".");
        const auto slashIndex = qFilename.lastIndexOf("/");
        if (dotIndex > 0 && (slashIndex < 0 || dotIndex > slashIndex))
        {
            qFilename = qFilename.left(dotIndex);
        }
        filename = (qFilename + ".png").toStdString();
    }
}