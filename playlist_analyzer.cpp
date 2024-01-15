#include "playlist_analyzer.h"
#include <iostream>
#include <fstream>

void analyzePlaylist(const std::string& play_list_path_)
{
    std::vector<Play_list_link> play_list_links;
    
    // Шаг 1: Чтение ссылок из плейлиста
    std::ifstream file(play_list_path_);
    if (!file)
    {
        std::cerr << "Ошибка открытия файла: " << play_list_path_ << std::endl;
        return;
    }
    
    std::string line;
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
            
            Play_list_link link;
            link.path_to_file = path;
            play_list_links.push_back(link);
        }
    }
    
    file.close();
    
    // Шаг 3: Проверка ссылок
    //std::string saved_file_path = "/home/rastyle/CLionProjects/XSPF_Analyzer/broken_links.txt";
    std::ofstream output_file(saved_file_path);
    if (!output_file)
    {
        std::cerr << "Ошибка создания файла." << std::endl;
        return;
    }
    
    std::cout << "#### Сломанные ссылки: ####" << std::endl;
    output_file << "#### Сломанные ссылки: ####" << std::endl;
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
    
    std::cout << "====================" << std::endl;
    output_file << "====================" << std::endl;
    
    std::cout << "#### Рабочие ссылки: ####" << std::endl;
    output_file << "#### Рабочие ссылки: ####" << std::endl;
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