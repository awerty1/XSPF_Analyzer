#include <iostream>
#include <limits>
#include "playlist_analyzer.h"

int main()
{
    size_t choice;
    
    while (true)
    {
        std::cout << "Выберите действие:\n"
                  << "1. Анализ сломанных путей до файла\n"
                  << "2. Замена путей до файла. "
                     "Если все файлы находятся в 1ой папке, то заменится путь до папки.\n"
                  << "3. Другая функциональность\n";
        
        std::cout << "Выш выбор: ";
        if (!(std::cin >> choice) || choice < 1 || choice > 4)
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
            analyze_playlist(play_list_path);
            break;
        case 2:
            std::cout << "Вы выбрали замену пути.\n";
            replace_path_simple(play_list_path, new_path);
            break;
        case 3:
            replace_path_smart();
            break;
        default:
            std::cout << "Неправильный выбор.\n";
            return 1;
    }
    
    return 0;
}
