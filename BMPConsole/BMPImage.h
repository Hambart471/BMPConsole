#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <iostream>

//
// Описание бинарных заголовков BMP (упакованные структуры).
//
#pragma pack(push,1)
struct BMPFileHeader {
    uint16_t bfType;       // Тип файла: должно быть 'BM' = 0x4D42
    uint32_t bfSize;       // Общий размер файла в байтах
    uint16_t bfReserved1;  // Зарезервировано, должно быть 0
    uint16_t bfReserved2;  // Зарезервировано, должно быть 0
    uint32_t bfOffBits;    // Смещение от начала файла до пиксельных данных
};

struct BMPInfoHeader {
    uint32_t biSize;          // Размер этого заголовка (40 байт)
    int32_t  biWidth;         // Ширина изображения в пикселях
    int32_t  biHeight;        // Высота изображения в пикселях (если >0 — снизу вверх)
    uint16_t biPlanes;        // Число цветовых плоскостей, всегда 1
    uint16_t biBitCount;      // Глубина цвета (24 или 32 бита)
    uint32_t biCompression;   // Тип сжатия (0 = BI_RGB, без сжатия)
    uint32_t biSizeImage;     // Размер растровых данных (может быть 0)
    int32_t  biXPelsPerMeter; // Разрешение по X (пикселей на метр)
    int32_t  biYPelsPerMeter; // Разрешение по Y
    uint32_t biClrUsed;       // Число используемых цветов (0 = все)
    uint32_t biClrImportant;  // Важных цветов (0 = все)
};
#pragma pack(pop)

class BMPImage {
public:
    BMPImage();
    ~BMPImage();

    // Загрузить BMP из файла. Возвращает true при успехе.
    bool load(const std::string& filename);

    // Сохранить BMP в файл. Возвращает true при успехе.
    bool save(const std::string& filename) const;

    // Отобразить в консоли: '#' для черного, ' ' (пробел) для белого
    void display() const;

    // Нарисовать линию между двумя точками (алгоритм Брезенхема)
    void drawLine(int x1, int y1, int x2, int y2);

    // Получить размеры
    int getWidth()  const { return infoHeader.biWidth; }
    int getHeight() const { return std::abs(infoHeader.biHeight); }

private:
    BMPFileHeader fileHeader;
    BMPInfoHeader infoHeader;
    std::vector<uint8_t> data; // Буфер для растровых данных
    int rowSize;               // Выравненный размер одной строки в байтах

    // Возвращает указатель на начало пикселя (x,y) в буфере data
    uint8_t* pixel(int x, int y) const;
};