#include "playlist_analyzer.h"
#include <iostream>
#include <fstream>

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