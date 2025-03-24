#include <iostream>
#include <cstdlib>
#include <chrono>
#include <filesystem> 
using namespace std;
string version = "2.0.0";
std::string Get_Active_Time(){
    auto current_time = std::chrono::system_clock::now();
    std::time_t current_time_t = std::chrono::system_clock::to_time_t(current_time);
    return std::ctime(&current_time_t);
}
int main(){
    namespace fs = std::filesystem;
    
    fs::path currentDir = fs::current_path();
    
    std::string command = "sudo cp -r " + currentDir.string() + "/build" + "/* /Gamer_OS/kernel/modules/";
    std::cout << "LOG: DEV_INSTALLER VER " << version  << " installing compilied dep! " << Get_Active_Time << std::endl;
    system(command.c_str());
}