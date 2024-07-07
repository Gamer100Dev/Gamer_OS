#include <iostream>
#include <chrono>
#include <cstdlib>
#include <iomanip>
#include <vector>
#include <cstring>
#include <unistd.h>
#include <pwd.h>

std::string getCurrentUser() {
    const char* user = getenv("USER");
    if (user == nullptr) {
        struct passwd* pw = getpwuid(getuid());
        if (pw) {
            user = pw->pw_name;
        }
    }
    return user ? user : "unknown";
}
std::string Get_Active_Time() {
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    std::tm local_tm = *std::localtime(&now_c);
    std::ostringstream oss;
    oss << std::put_time(&local_tm, "%c");
    return oss.str();
}
struct Command {
    std::string name;
    std::string description;
    std::string usage;
};

void printCommands(const std::vector<Command>& commands) {

    std::cout << std::left << std::setw(15) << "Command" 
              << std::setw(30) << "Description" 
              << "Usage" << std::endl;
    std::cout << std::string(70, '-') << std::endl;


    for (const auto& command : commands) {
        std::cout << std::left << std::setw(15) << command.name 
                  << std::setw(30) << command.description 
                  << command.usage << std::endl;
    }
}
int print(std::string text){
    std::cout << "LOG: Launchd "+Get_Active_Time()+": " << text << std::endl;
    return 0;
}
std::string trim(const std::string& str) {
    size_t end = str.find_last_not_of(" \t\n\r\f\v");
    return (end == std::string::npos) ? "" : str.substr(0, end + 1);
}

int main(int argc, char* argv[]) {
        std::vector<Command> commands = {
        {"--run", "Run a process", "--run -path <path>"},
        {"--kill", "Kill a process", "--kill <process_name>"},
        {"--help", "Display help", "--help"}
    };

    if (argc == 1) {  
       printCommands(commands);
    }
   for (int i = 1; i < argc; ++i) {
            if (std::string(argv[i]) == "--run") {
                if (i + 1 < argc) {
                    std::string file_path = argv[i + 1];
                    system(file_path.c_str());
                    print("Launching >> "+file_path+" from user: " + std::string(getCurrentUser()));
                } else {
                    print("Error: No file path provided for --run");
                }
                break;
            }
            if (std::string(argv[i]) == "--kill"){
                if (i + 1 < argc) {
                    std::string file_path = argv[i + 1];
                    std::string command = "pkill " + file_path;
                    system(command.c_str());
                    print("Closing executable of  >> "+file_path+" from user: " + std::string(getCurrentUser()));
                } else {
                    print("Error: No file path provided for --kill");
                }
        }
        if (std::string(argv[i]) == "--help"){
            printCommands(commands);
        }
       return 0;
   }
}