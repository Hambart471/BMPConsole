#include "BMPImage.h"
#include <iostream>
#include <string>

int main() {
    std::string inFile, outFile;
    BMPImage img;

    // 1) Запрос имени входного BMP-файла
    std::cout << ">> Enter input BMP file name: ";
    std::getline(std::cin, inFile);

    if (!img.load(inFile)) {
        // Если не удалось загрузить — выходим с кодом ошибки
        return 1;
    }

    // 2) Вывод исходного изображения в консоль
    std::cout << "\n--- Original Image ---\n";
    img.display();

    // 3) Рисуем крест (две диагонали)
    int w = img.getWidth();
    int h = img.getHeight();
    img.drawLine(0, 0, w - 1, h - 1);
    img.drawLine(w - 1, 0, 0, h - 1);

    // 4) Вывод модифицированного изображения
    std::cout << "\n--- Modified Image ---\n";
    img.display();

    // 5) Запрос имени выходного BMP-файла и сохранение
    std::cout << "\n>> Enter output BMP file name: ";
    std::getline(std::cin, outFile);
    if (!img.save(outFile)) {
        return 2;
    }

    std::cout << "\nГотово! Изображение сохранено в " << outFile << "\n";
    return 0;
}