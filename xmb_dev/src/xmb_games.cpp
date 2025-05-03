#include "xmb_games.h"
#include <fstream>
#include <iostream>
#include <filesystem>
namespace fs = std::filesystem;

const std::string GAMES_DIR = "/Gamer_OS/Games";
const std::string launchd = "/Gamer_OS/kernel/modules/launchd.so"
xmb_games::xmb_games() {}

std::unordered_map<std::string, GameInfo> xmb_games::init_games() {
    games.clear();

    for (const auto& entry : fs::directory_iterator(GAMES_DIR)) {
        if (entry.is_directory()) {
            std::string subdir = entry.path().string();
            std::string gameFile = subdir + "/game.eg";

            if (fs::exists(gameFile)) {
                GameInfo info;
                if (parse_gameinfo(gameFile, info) == 0) {
                    std::string gameName = entry.path().filename().string();
                    games[gameName] = info;
                    std::cout << "Loaded: " << gameName << "\n";
                }
            }
        }
    }

    return games;
}

int xmb_games::parse_gameinfo(const std::string& filePath, GameInfo& info) {
    std::ifstream in(filePath, std::ios::binary);
    if (!in) {
        std::cerr << "Failed to open: " << filePath << "\n";
        return -1;
    }

    auto readString = [&](std::string& outStr) {
        uint32_t len;
        in.read(reinterpret_cast<char*>(&len), sizeof(len));
        if (len > 2048) return false; // avoid malicious sizes
        outStr.resize(len);
        in.read(&outStr[0], len);
        return true;
    };

    return readString(info.title) &&
           readString(info.binary_path) &&
           readString(info.binary_icon) &&
           readString(info.author) ? 0 : -1;
}

int xmb_games::launch_game(const std::string& gameTitle) {
    if (games.find(gameTitle) != games.end()) {
        std::string path = games[gameTitle].binary_path;
        std::cout << "Launching: " << gameTitle << " from " << path << "\n";
    //    bool GAME_LAUNCH = system(std::string(launchd + "--run" + path).c_str());
        // TODO:
        // Call compositor for the game to launch on + sound (pulseaudio or pipewire)
        return 0;
    } else {
        std::cerr << "Game not found: " << gameTitle << "\n";
        return -1;
    }
}

int xmb_games::add_new_title(const std::string& title, const GameInfo& info) {
    games[title] = info;
    std::cout << "Added new title: " << title << "\n";
    return 0;
}
