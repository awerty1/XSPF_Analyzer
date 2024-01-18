#ifndef PLAYLIST_ANALYZER_H
#define PLAYLIST_ANALYZER_H

#include <string>
#include <vector>
#include "config.h"
#include "config_example.h"

struct play_list_link
{
    std::string path_to_file;
};

// Функция для анализа плейлиста
static std::string generate_multiplied_separator_line(const std::string& separator, size_t length);
void analyze_playlist(const std::string& PLAY_LIST_PATH);
void replace_path_simple(const std::string& PLAY_LIST_PATH, const std::string& NEW_PATH);
void replace_path_smart(const std::string& PLAY_LIST_PATH, const std::string& DIRECTORY_PATH);


#endif // PLAYLIST_ANALYZER_H