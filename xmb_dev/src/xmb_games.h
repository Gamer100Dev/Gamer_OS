#pragma once

#include <string>
#include <vector>
#include <unordered_map>

struct GameInfo {
    std::string title;
    std::string binary_path;
    std::string binary_icon;
    std::string author;
};

class xmb_games {
private:
    std::unordered_map<std::string, GameInfo> games;

public:
    xmb_games();

    std::unordered_map<std::string, GameInfo> init_games();
    int parse_gameinfo(const std::string& filePath, GameInfo& info);
    int launch_game(const std::string& gameTitle);
    int add_new_title(const std::string& title, const GameInfo& info);
};
