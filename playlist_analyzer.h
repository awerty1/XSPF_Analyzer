#ifndef PLAYLIST_ANALYZER_H
#define PLAYLIST_ANALYZER_H

#include <string>
#include <vector>
#include "config.h"
#include "config_example.h"

struct Play_list_link
{
    std::string path_to_file;
};

// Функция для анализа плейлиста
void analyzePlaylist(const std::string& play_list_path);

#endif // PLAYLIST_ANALYZER_H