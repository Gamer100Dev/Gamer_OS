#include <iostream>
#include <chrono>
#include <X11/Xlib.h>
#include <X11/extensions/Xrandr.h>
#include <cstdlib>
// Launches and gets the systemr ready!

std::string Get_Active_Time(){
    auto current_time = std::chrono::system_clock::now();
    std::time_t current_time_t = std::chrono::system_clock::to_time_t(current_time);
    return std::ctime(&current_time_t);
}
void init_drivers(){
    std::cout << "LOG: Launching i915kms.ko " << Get_Active_Time << std::endl;
    system("kldload i915kms.ko");
}
void Start_DE(){
    std::cout << "LOG: Launching /Gamer_OS/kernel/modules/startAQ.so " << Get_Active_Time << std::endl;
    system("/Gamer_OS/kernel/modules/startAQ.so");
    init_drivers();
}
void StartX(){
    std::cout << "LOG: Launching XServer at: " << Get_Active_Time << std::endl;
    system("startx");
    Start_DE();
}

int main(){
    std::cout << "LOG: Welcome to Gamer OS! Time is " << Get_Active_Time() << std::endl;
    StartX();
}