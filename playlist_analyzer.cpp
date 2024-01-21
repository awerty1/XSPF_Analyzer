#include <iostream>
#include <fstream>
#include <filesystem>
#include <algorithm>

#include "playlist_analyzer.h"

// для препроцессора, чтобы можно было вызвать get_filename в analyze_playlist
static std::string get_filename(const std::string&);
static std::pair<std::string, std::string> get_file_path_and_filename_without_extension(const std::string&);
static std::string generate_multiplied_separator_line(const std::string&, size_t);
static std::vector<play_list_path> read_playlist(const std::string&);



static std::string generate_multiplied_separator_line(const std::string& separator, size_t length)
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
    std::vector<play_list_path> play_list_paths;
    
    // Шаг 1: Чтение ссылок из плейлиста
    std::ifstream file(play_list_path_);
    if (!file.is_open())
    {
        //throw std::runtime_error("Ошибка открытия файла: " + play_list_path_);
        std::cerr << "Ошибка открытия файла: " << play_list_path_ << std::endl;
        return;
    }
    
    std::string line;
    // размеры указанны не 0, на случай, если что-то сломается
    const size_t MAX_PATH_LENGTH = 30;
    size_t max_length = MAX_PATH_LENGTH; // Переменная для хранения размера самой длинной ссылки
    size_t min_length = MAX_PATH_LENGTH;; // Переменная для хранения размера самой короткой ссылки
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
            
            play_list_path link;
            link.path_to_file = path;
            play_list_paths.push_back(link);
            
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
    std::ofstream output_file(SAVED_FILE_PATH);
    if (!output_file)
    {
        std::cerr << "Ошибка создания файла." << std::endl;
        return;
    }
    
    // Выравниваем сепараторы по длине самой большой ссылки
    
    
    const std::string SEPARATOR_LINE_EQUALS = "=";
    // 18 символов не записывается в файл ""
    const size_t EXTRA_CHARACTERS = 18;
    //size_t without_msg_eighteen = 18;
    size_t len_with_eighteen_max = max_length + EXTRA_CHARACTERS;
    size_t len_without_eighteen_max = max_length;
    std::string multiplied_separator_line_equals =
            generate_multiplied_separator_line(SEPARATOR_LINE_EQUALS, len_with_eighteen_max);
    std::string multiplied_separator_line_equals_without_eighteen =
            generate_multiplied_separator_line(SEPARATOR_LINE_EQUALS, len_without_eighteen_max);
    
    // Выравниваем сепараторы по длине самой короткой ссылки
    std::string separator_line_hash = "#";
    size_t len_with_eighteen_min = min_length / 2 + EXTRA_CHARACTERS;
    std::string multiplied_separator_line_hash =
            generate_multiplied_separator_line(separator_line_hash, len_with_eighteen_min);
    
    
    std::cout << multiplied_separator_line_equals << std::endl << std::endl;
    output_file << multiplied_separator_line_equals_without_eighteen << std::endl << std::endl;
    
    const std::string play_list_filename = get_filename(play_list_path_);
    std::cout << multiplied_separator_line_hash << " Сломанные ссылки в файле \"" << play_list_filename
              << "\": " << multiplied_separator_line_hash << std::endl;
    output_file << multiplied_separator_line_hash << " Сломанные ссылки в файле \"" << play_list_filename
                << "\": " << multiplied_separator_line_hash << std::endl;
    
    for (const auto& link : play_list_paths)
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
    
    std::cout << multiplied_separator_line_hash << " Рабочие ссылки в файле " << "\"" << play_list_filename
              << "\": " << multiplied_separator_line_hash << std::endl;
    output_file << multiplied_separator_line_hash << " Рабочие ссылки в файле " << "\"" << play_list_filename
                << "\": " << multiplied_separator_line_hash << std::endl;
    
    for (const auto& link : play_list_paths)
    {
        std::ifstream test_file(link.path_to_file);
        if (test_file)
        {
            std::cout << "Рабочая ссылка: " << link.path_to_file << std::endl;
            output_file << link.path_to_file << std::endl;
        }
        
        test_file.close();
    }
    
    std::cout << std::endl << multiplied_separator_line_equals << std::endl << std::endl;
    output_file << std::endl << multiplied_separator_line_equals_without_eighteen << std::endl << std::endl;
    
    output_file.close();
}

void replace_path_simple(const std::string& play_list_path_, const std::string& new_path_)
{
    // 1. Копируем файл плейлиста и дописываем в него новую ссылку
    std::pair<std::string, std::string> file_info = get_file_path_and_filename_without_extension(play_list_path_);
    const std::string directory = file_info.first;
    const std::string name_of_file_without_ext = file_info.second;
    const std::string dir_plus_file_without_ext = directory + "/playlists_edited/" + name_of_file_without_ext;
    const std::string new_path_for_output = dir_plus_file_without_ext + std::string("_siedited.xspf");
    
    //std::cout << "dir_plus_file_with_ext" << dir_plus_file_with_ext;
    std::ifstream input(play_list_path_);
    std::ofstream output(new_path_for_output);
    
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
    
    std::cout << "Простая замена ссылок завершена, плейлист находится по пути \"" << new_path_for_output << "\"" << std::endl;
    std::cout <<"Новый путь: " << "\"" << new_path_ << "\"" << std::endl;
}

void replace_path_smart(const std::string& play_list_path_, const std::string& directory_path,
                        const std::string& not_found_path)
{
    std::pair<std::string, std::string> file_info = get_file_path_and_filename_without_extension(play_list_path_);
    const std::string directory = file_info.first;
    const std::string name_of_file_without_ext = file_info.second;
    const std::string dir_plus_file_with_ext = directory + "/playlists_edited/" + name_of_file_without_ext;
    
    static const std::string new_playlist_path = dir_plus_file_with_ext + "_smedited.xspf";
    
    std::ifstream src_file(play_list_path_);
    std::ofstream dest_file(new_playlist_path);
    
    if (!src_file.is_open() || !dest_file.is_open())
    {
        std::filesystem::remove(new_playlist_path + ".tmp");
        throw std::runtime_error("Ошибка при открытии файлов");
    }
    
    dest_file << src_file.rdbuf();
    
    src_file.close();
    dest_file.close();
    
    try
    {
        std::ifstream playlist_file(new_playlist_path);
        std::ofstream not_found_file(not_found_path);
        std::ofstream temp_file(new_playlist_path + ".tmp");
        
        if (!playlist_file.is_open() || !not_found_file.is_open() || !temp_file.is_open())
        {
            std::filesystem::remove(new_playlist_path + ".tmp");
            throw std::runtime_error("Ошибка при открытии файлов");
        }
        
        std::string line;
        
        while (std::getline(playlist_file, line))
        {
            if (line.find("<location>") != std::string::npos && line.find("</location>") != std::string::npos)
            {
                std::string location = line.substr(line.find("<location>") + 10);
                location = location.substr(0, location.find("</location>"));
                
                std::string file_name = location.substr(location.find_last_of('/') + 1);
                
                std::string percent_replacement = " ";
                size_t percent_pos = file_name.find("%20");
                
                while (percent_pos != std::string::npos)
                {
                    file_name.replace(percent_pos, 3, percent_replacement);
                    percent_pos = file_name.find("%20", percent_pos + percent_replacement.length());
                }
                
                std::string file_path;
                bool found = false;
                
                for (const auto& entry : std::filesystem::recursive_directory_iterator(directory_path))
                {
                    if (!entry.is_directory() && entry.path().filename() == file_name)
                    {
                        file_path = entry.path().string();
                        found = true;
                        break;
                    }
                }
                
                if (found)
                {
                    std::string new_location = "file:///" + file_path;
                    line.replace(line.find("<location>") + 10, location.length(), new_location);
                }
                else
                {
                    not_found_file << file_name << std::endl;
                }
            }
            
            temp_file << line << std::endl;
        }
        
        playlist_file.close();
        not_found_file.close();
        temp_file.close();
        
        std::filesystem::remove(new_playlist_path);
        std::filesystem::rename(new_playlist_path + ".tmp", new_playlist_path);
        
        std::cout << "Умная замена ссылок завершена, плейлист находится по пути \""<< new_playlist_path << "\"" << std::endl;
    }
    catch (const std::filesystem::filesystem_error& ex)
    {
        std::cout << "Произошла ошибка файловой системы: " << ex.what() << std::endl;
        
        // Удаление временного файла
        std::filesystem::remove(new_playlist_path + ".tmp");
    }
    catch (const std::exception& ex)
    {
        std::cout << "Произошла непредвиденная ошибка: " << ex.what() << std::endl;
        
        // Удаление временного файла
        std::filesystem::remove(new_playlist_path + ".tmp");
    }
}

// Статическая функция для получения имени файла без расширения name
static std::pair<std::string, std::string> get_file_path_and_filename_without_extension(const std::string& path)
{
    std::filesystem::path file_path(path);
    std::string directory = file_path.parent_path().string();
    std::string  filename_without_ext = file_path.stem().string();
    
    return std::make_pair(directory, filename_without_ext);
}

// Статическая функция для получения имени файла с расширением name.xspf
static std::string get_filename(const std::string& path)
{
    std::filesystem::path file_path(path);
    return file_path.filename().string();
}

// Cтатическая функция для чтения плейлиста
static std::vector<play_list_path> read_playlist(const std::string& play_list_path_)
{
    std::ifstream file(play_list_path_);
    std::vector<play_list_path> play_list;
    std::string line;
    
    while (std::getline(file, line))
    {
        if (!line.empty() && line.find("<location>") != std::string::npos)
        {
            std::string path = line.substr(line.find("<location>") + 10, line.find("</location>") - line.find("<location>") - 10);
            play_list.emplace_back(play_list_path{path});
//            play_list_path play_list_item;
//            play_list_item.path_to_file = path;
//            play_list.push_back(play_list_item);
        }
    }
    
    return play_list;
}

// Функция, которая находит разницу между файлами, например между name.xspf и name2.xspf
void checkin_xspf_diff(const std::string& play_list_path_, const std::string& second_play_list_path,
                       const std::string& xspf_diff_path)
{
    std::vector<play_list_path> play_list_paths1 = read_playlist(play_list_path_);
    std::vector<play_list_path> play_list_paths2 = read_playlist(second_play_list_path);
    
    std::ofstream diff_file(xspf_diff_path);
    if (!diff_file)
    {
        std::cerr << "Ошибка при создании файла xspf_diff.txt" << std::endl;
        return;
    }
    
    const std::string play_list_filename = get_filename(play_list_path_);
    const std::string second_play_list_filename = get_filename(second_play_list_path);
    
    diff_file << "Разница между файлами " << play_list_filename
              << " и " << second_play_list_filename << ":" << std::endl;
    
    size_t i = 1;
    diff_file << second_play_list_filename << ":" << std::endl;
    for (const auto& path : play_list_paths2)
    {
        bool found_diff = false;
        for (const auto& p : play_list_paths1)
        {
            if (p.path_to_file == path.path_to_file)
            {
                found_diff = true;
                break;
            }
        }
        
        if (!found_diff)
        {
            diff_file << i << "." << path.path_to_file << std::endl;
            i++;
        }
    }
    
    // Обновление значения i перед началом второго цикла
    i = 1;
    diff_file << play_list_filename << ":" << std::endl;
    for (const auto& path : play_list_paths1)
    {
        bool found_diff = false;
        for (const auto& p : play_list_paths2)
        {
            if (p.path_to_file == path.path_to_file)
            {
                found_diff = true;
                break;
            }
        }
        
        if (!found_diff)
        {
            diff_file << i << "." << path.path_to_file << std::endl;
            i++;
        }
    }
    
    std::cout << "Простая замена ссылок завершена, файл \"xspf_diff.txt\" находится по пути \""
              << xspf_diff_path << "\"" << std::endl;
    //std::cout << "Файл xspf_diff.txt успешно создан." << std::endl;
}

void create_new_file_with_diff(const std::string& play_list_path)
{
    std::cout << "Not realized! " + play_list_path << std::endl;
}