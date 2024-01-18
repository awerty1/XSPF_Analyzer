#include <iostream>
#include <limits>
#include "playlist_analyzer.h"

int main()
{
    size_t choice;
    
    while (true)
    {
        std::cout << "Выберите действие:\n"
                  << "1. Анализ сломанных путей до файла.\n"
                     "Анализирует все сломанные ссылки в плейлисте и составляет отчет.\n"
                  << "2. Простая замена путей до файла.\n"
                     "Если все файлы находятся в одной папке, то заменяет путь до папки.\n"
                  << "3. Умная замена путей до файла.\n"
                     "Ищет по названиям файлов в директории N и в подпапках этой директории, меняет путь.\n"
                  << "4. Другая функциональность.\n";
        
        
        std::cout << "Выш выбор: ";
        if (!(std::cin >> choice) || choice < 1 || choice > 5)
        {
            std::cout << "Неправильный выбор. Попробуйте снова.\n" << std::endl;
            std::cin.clear();  // Сбрасываем флаг ошибки ввода
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  // Очищаем буфер ввода
        }
        else
        {
            // Ввод корректный, выходим из цикла
            break;
        }
    }
    
    switch (choice)
    {
        case 1:
            std::cout << "Вы выбрали анализ плейлиста.\n";
            analyze_playlist(PLAY_LIST_PATH);
            break;
        case 2:
            std::cout << "Вы выбрали замену пути.\n";
            replace_path_simple(PLAY_LIST_PATH, NEW_PATH);
            break;
        case 3:
            replace_path_smart(PLAY_LIST_PATH, DIRECTORY_PATH);
            break;
        default:
            std::cout << "Неправильный выбор.\n";
            return 1;
    }
    
    return 0;
}
