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
void print(std::string& string){
    std::cout << "LOG: " << string << std::endl;
}
int main(){
    namespace fs = std::filesystem;
    
    fs::path currentDir = fs::current_path();
    
    std::string command = "sudo cp -r " + currentDir.string() + "/* /Gamer_OS/kernel/modules/";
    std::cout << "LOG: DEV_INSTALLER VER " << version  << " installing compilied dep! " << Get_Active_Time << std::endl;
    bool INSTALLER_F1 = system(command.c_str());
    if (INSTALLER_F1 == 1){
        bool CHECK_GAMEROS_SUBFS_EXIST = system(std::string("find /Gamer_OS").c_str());
        if (CHECK_GAMEROS_SUBFS_EXIST == 0){
            print(std::string("SUB_SYSTEM_KERNEL_FOLD FOUND! 0"));
        } else {CHECK_GAMEROS_SUBFS_EXIST == 1}{
            bool System_Creation_F1 = system(std::string("sudo mkdir -p /Gamer_OS/kernel/modules && sudo mkdir -p /Gamer_OS/kernel/assets && sudo mkdir -p /Gamer_OS/kernel/Applications").c_str());
            if (System_Creation_F1 == 0){
                print(string("OS_F1: 0"));
            } // Made the sub folders
        }
    }
    // TODO: Curl and get any packages: Future Assets,Sound 
    
}