#include <iostream>
#include <limits>
#include "playlist_analyzer.h"

int main()
{
    size_t choice;
    bool valid_choice = false;
    
    while (!valid_choice)
    {
        std::cout << "Выберите действие:\n"
                  << "1. Анализ сломанных путей до файла.\n"
                     "Анализирует все сломанные ссылки в плейлисте и составляет отчет.\n"
                  << "2. Простая замена путей до файла.\n"
                     "Если все файлы находятся в одной папке, то заменяет путь до папки.\n"
                  << "3. Умная замена путей до файла.\n"
                     "Ищет по названиям файлов в директории N и в подпапках этой директории, меняет путь.\n"
                  << "4. Проверка двух файлов XSPF на различия.\n"
                     "Программа может сравнивать два файла XSPF и находить различия между ними.\n"
                  << "5. Создать новый файл с невстречающимися файлами после сравнения.\n"
                     "Будет создан новый файл XSPF, который будет содержать файлы, "
                     "присутствующие в одной версии, но отсутствующие в другой.\n";
        
        
        std::cout << std::endl << "Выберите один из вариантов (1-5): ";
        if (!(std::cin >> choice) || choice < 1 || choice > 5)
        {
            std::cout << "Неправильный выбор. Попробуйте снова.\n" << std::endl;
            std::cin.clear();  // Сбрасываем флаг ошибки ввода
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  // Очищаем буфер ввода
        }
        else
        {
            // Ввод корректный, выходим из цикла
            valid_choice = true;
        }
    }
    
    //std::cout << "Выш выбор: " << choice << std::endl;
    switch (choice)
    {
        case 1:
            std::cout << "Вы выбрали " << choice << " \"анализ плейлиста\".\n";
            analyze_playlist(PLAY_LIST_PATH);
            break;
        case 2:
            std::cout << "Вы выбрали " << choice << " \"простую замену пути\".\n";
            replace_path_simple(PLAY_LIST_PATH, NEW_PATH);
            break;
        case 3:
            std::cout << "Вы выбрали " << choice << " \"умную замену путей\".\n";
            replace_path_smart(PLAY_LIST_PATH, DIRECTORY_PATH);
            break;
        case 4:
            std::cout << "Вы выбрали " << choice << " \"проверку 2х файлов на различия\".\n";
            checkin_xspf_diff(PLAY_LIST_PATH);
            break;
        case 5:
            std::cout << "Вы выбрали " << choice <<  " \"создание нового файла с различиями\".\n";
            create_new_file_with_diff(PLAY_LIST_PATH);
            break;
        default:
            std::cout << "Неправильный выбор!\n";
            return 1;
    }
    
    return 0;
}
