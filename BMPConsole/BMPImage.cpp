#include "BMPImage.h"
#include <fstream>

BMPImage::BMPImage() : rowSize(0) {}

BMPImage::~BMPImage() {
    // Вектор data автоматически освобождается
}

bool BMPImage::load(const std::string& filename) {
    std::ifstream in(filename, std::ios::binary);
    if (!in) {
        std::cerr << "Ошибка: не удалось открыть файл для чтения: " << filename << "\n";
        return false;
    }

    // --- Читаем заголовок файла ---
    in.read(reinterpret_cast<char*>(&fileHeader), sizeof(fileHeader));
    if (fileHeader.bfType != 0x4D42) {
        std::cerr << "Ошибка: файл не является BMP (отсутствует сигнатура 'BM')\n";
        return false;
    }

    // --- Читаем заголовок информации ---
    in.read(reinterpret_cast<char*>(&infoHeader), sizeof(infoHeader));
    if (infoHeader.biBitCount != 24 && infoHeader.biBitCount != 32) {
        std::cerr << "Ошибка: поддерживаются только 24- или 32-битные BMP. "
            << "В файле: " << infoHeader.biBitCount << " бит\n";
        return false;
    }

    // --- Вычисляем размер выровненной строки (padding до 4 байт) ---
    int bytesPerPixel = infoHeader.biBitCount / 8;
    rowSize = ((infoHeader.biWidth * bytesPerPixel + 3) / 4) * 4;

    // --- Выделяем буфер для данных и читаем ---
    int height = std::abs(infoHeader.biHeight);
    data.resize(rowSize * height);
    in.seekg(fileHeader.bfOffBits, std::ios::beg);
    in.read(reinterpret_cast<char*>(data.data()), data.size());

    in.close();
    return true;
}

bool BMPImage::save(const std::string& filename) const {
    std::ofstream out(filename, std::ios::binary);
    if (!out) {
        std::cerr << "Ошибка: не удалось открыть файл для записи: " << filename << "\n";
        return false;
    }

    // --- Записываем заголовки и данные  ---
    out.write(reinterpret_cast<const char*>(&fileHeader), sizeof(fileHeader));
    out.write(reinterpret_cast<const char*>(&infoHeader), sizeof(infoHeader));
    out.write(reinterpret_cast<const char*>(data.data()), data.size());

    out.close();
    return true;
}

uint8_t* BMPImage::pixel(int x, int y) const {
    // В BMP координата (0,0) — левый нижний угол, если biHeight>0
    int height = std::abs(infoHeader.biHeight);
    int yy = (infoHeader.biHeight > 0) ? (height - 1 - y) : y;
    int offset = yy * rowSize + x * (infoHeader.biBitCount / 8);
    return const_cast<uint8_t*>(data.data() + offset);
}

void BMPImage::display() const {
    int w = getWidth();
    int h = getHeight();
    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            uint8_t* p = pixel(x, y);
            uint8_t B = p[0], G = p[1], R = p[2];
            // Если пиксель чёрный
            if (R == 0 && G == 0 && B == 0)
                std::cout << '#';
            else
                std::cout << ' ';
        }
        std::cout << "\n";
    }
}

void BMPImage::drawLine(int x1, int y1, int x2, int y2) {
    // Алгоритм Брезенхема для рисования линии
    int dx = std::abs(x2 - x1), sx = (x1 < x2 ? 1 : -1);
    int dy = -std::abs(y2 - y1), sy = (y1 < y2 ? 1 : -1);
    int err = dx + dy;

    int x = x1, y = y1;
    while (true) {
        uint8_t* p = pixel(x, y);
        // Задаём чёрный цвет R=G=B=0
        p[0] = p[1] = p[2] = 0;
        if (infoHeader.biBitCount == 32) {
            p[3] = 255; // непрозрачность, если есть альфа
        }

        if (x == x2 && y == y2) break;
        int e2 = 2 * err;
        if (e2 >= dy) { err += dy; x += sx; }
        if (e2 <= dx) { err += dx; y += sy; }
    }
}