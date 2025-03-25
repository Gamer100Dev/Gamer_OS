#include <iostream>
#include <chrono>
#include <thread>
#include <cstdlib>
using namespace std;
const std::string gamer_starter = "/Gamer_OS/kernel/modules/gamer_compositor -s /Gamer_OS/kernel/modules/startAQ.so";
const std::string gamer_services = "/Gamer_OS/kernel/modules/System_Services.so";
std::string Get_Active_Time() {
    auto current_time = std::chrono::system_clock::now();
    std::time_t current_time_t = std::chrono::system_clock::to_time_t(current_time);
    return std::ctime(&current_time_t);
}

void init_drivers() {
    std::cout << "LOG: Launching i915kms.ko " << Get_Active_Time() << std::endl;
    bool driver_intel = system("kldload i915kms.ko");
    if (!driver_intel){
        // Assuming its either a virtual matchine
        // TODO Add logic for other firmwares!

        bool virtual_matchine_DRV = system("kldload vboxvideo");
        if (virtual_matchine_DRV){
            std::cout << "LOG: Success!" << std::endl;
        }
    }
}

void Start_DE() {
    std::cout << "LOG: Launching /Gamer_OS/kernel/modules/startAQ.so " << Get_Active_Time() << std::endl;
    system("/Gamer_OS/kernel/modules/startAQ.so");
    init_drivers();
}

void StartX() { // For FreeBSD
    std::cout << "LOG: Launching XServer at: " << Get_Active_Time() << std::endl;
    
    // Example custom startx command with options
    std::string command = "startx -- -dpi 96 -depth 24";
    int result = system(command.c_str());
    if (result == 1){
        // There is no .xinitrc
   
    }
    std::string System_services_init = "/Gamer_OS/kernel/modules/System_Services.so";
    system(System_services_init.c_str());
    init_drivers();
}
int main() {
    std::cout << "LOG: Welcome to Gamer OS! Time is " << Get_Active_Time() << std::endl;
  //  system("touch /root/.xinitrc");
 //   system("echo /Gamer_OS/kernel/modules/startAQ.so >> /root/.xinitrc");
    init_drivers();
    system(gamer_services.c_str());
    system(gamer_starter.c_str());
 //   StartX(); Scrapped due to the lack of it within the compositor
    return 0;
}
