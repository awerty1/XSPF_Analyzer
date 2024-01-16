#include <iostream>
#include <fstream>
#include "playlist_analyzer.h"

std::string generate_multiplied_separator_line(const std::string& separator, size_t length)
{
    std::string multiplied_separator = separator;
    
    for (size_t i = 1; i < length; i++)
    {
        multiplied_separator += separator;
    }
    
    return multiplied_separator;
}


void analyze_playlist(const std::string& play_list_path_)
{
    std::vector<play_list_link> play_list_links;
    
    // Шаг 1: Чтение ссылок из плейлиста
    std::ifstream file(play_list_path_);
    if (!file)
    {
        std::cerr << "Ошибка открытия файла: " << play_list_path_ << std::endl;
        return;
    }
    
    std::string line;
    // размеры указанны не 0, на случай, если что-то сломается
    size_t max_length = 30; // Переменная для хранения размера самой длинной ссылки
    size_t min_length = 30; // Переменная для хранения размера самой короткой ссылки
    while (std::getline(file, line))
    {
        if (line.find("<location>") != std::string::npos)
        {
            size_t start_pos = line.find("<location>") + std::string("<location>").length();
            size_t end_pos = line.find("</location>");
            std::string path = line.substr(start_pos, end_pos - start_pos);
            
            // Шаг 2: Удаление "file:///" из начала пути
            if (path.find("file:///") != std::string::npos)
            {
                path = path.substr(std::string("file:///").length());
            }
            
            play_list_link link;
            link.path_to_file = path;
            play_list_links.push_back(link);
            
            // Обновление переменной max_length/min_length, если найдена более длинная/короткая ссылка
            if (path.length() > max_length)
            {
                max_length = path.length();
            }
            else
            {
                min_length = path.length();
            }
        }
    }
    
    file.close();
    
    // Шаг 3: Проверка ссылок
    std::ofstream output_file(saved_file_path);
    if (!output_file)
    {
        std::cerr << "Ошибка создания файла." << std::endl;
        return;
    }
    
    // Выравниваем сепараторы по длине самой большой ссылки
    std::string separator_line_equals = "=";
    size_t without_msg_eighteen = 18;
    size_t len_with_eighteen_max = max_length + without_msg_eighteen;
    size_t len_without_eighteen_max = max_length;
    std::string multiplied_separator_line_equals =
            generate_multiplied_separator_line(separator_line_equals, len_with_eighteen_max);
    std::string multiplied_separator_line_equals_without_eighteen =
            generate_multiplied_separator_line(separator_line_equals, len_without_eighteen_max);
    
    // Выравниваем сепараторы по длине самой короткой ссылки
    std::string separator_line_hash = "#";
    size_t len_with_eighteen_min = min_length / 2 + without_msg_eighteen;
    std::string multiplied_separator_line_hash =
            generate_multiplied_separator_line(separator_line_hash, len_with_eighteen_min);
    
    
    std::cout << multiplied_separator_line_equals << std::endl << std::endl;
    output_file << multiplied_separator_line_equals << std::endl << std::endl;
    std::cout << multiplied_separator_line_hash << " Сломанные ссылки: " << multiplied_separator_line_hash << std::endl;
    output_file << multiplied_separator_line_hash <<" Сломанные ссылки: "<< multiplied_separator_line_hash << std::endl;
    for (const auto& link : play_list_links)
    {
        std::ifstream test_file(link.path_to_file);
        if (!test_file)
        {
            std::cout << "Сломанная ссылка: " << link.path_to_file << std::endl;
            output_file << link.path_to_file << std::endl;
        }
        
        test_file.close();
    }
    
    std::cout << std::endl << multiplied_separator_line_equals << std::endl << std::endl;
    output_file << std::endl << multiplied_separator_line_equals_without_eighteen << std::endl << std::endl;
    
    std::cout << multiplied_separator_line_hash << " Рабочие ссылки: " << multiplied_separator_line_hash << std::endl;
    output_file << multiplied_separator_line_hash << " Рабочие ссылки: " << multiplied_separator_line_hash << std::endl;
    for (const auto& link : play_list_links)
    {
        std::ifstream test_file(link.path_to_file);
        if (test_file)
        {
            std::cout << "Рабочая ссылка: " << link.path_to_file << std::endl;
            output_file << link.path_to_file << std::endl;
        }
        
        test_file.close();
    }
    
    output_file.close();
}

void replace_path_simple(const std::string& play_list_path_, const std::string& new_path_)
{
    // 1. Копируем файл плейлиста и дописываем в него новую ссылку
    std::ifstream input(play_list_path_);
    std::ofstream output(play_list_path_ + std::string("_edited.xspf"));
    
    if (!input.is_open() || !output.is_open())
    {
        std::cerr << "Не удалось открыть файлы" << std::endl;
        return;
    }
    
    std::string line;
    while (getline(input, line))
    {
        // Шаг 3: Проверяем строки, содержащие <location>
        size_t start_pos = line.find("<location>");
        size_t end_pos = line.find("</location>");
        
        if (start_pos != std::string::npos && end_pos != std::string::npos)
        {
            // Шаг 4: Извлекаем путь до файла между <location> и </location>
            std::string path = line.substr(start_pos + 10, end_pos - (start_pos + 10));
            
            // Производим замену пути файла
            size_t file_start_pos = path.find("file:///");
            if (file_start_pos != std::string::npos)
            {
                size_t last_slash_pos = path.find_last_of('/');
                if (last_slash_pos != std::string::npos)
                {
                    std::string new_path_after_changed = path.substr(0, file_start_pos + 8) + new_path_ + path.substr(last_slash_pos);
                    line.replace(start_pos + 10, end_pos - (start_pos + 10), new_path_after_changed);
                }
            }
        }
        
        // Записываем обновленную строку в новый файл
        output << line << std::endl;
    }
    
    // Шаг 2: Закрываем файлы
    input.close();
    output.close();
    
    std::cout << "Простая замена ссылок завершена. Новый путь: " << "\"" << new_path_ << "\"" << std::endl;
}

void replace_path_smart()
{
    std::cout << "Not realised" << std::endl;
}