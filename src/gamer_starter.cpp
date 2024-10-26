#include <iostream>
#include <thread>
#include <cstdlib>
#include <fstream>
#ifdef __FREEBSD__
std::string compositor = "/Gamer_OS/kernel/modules/gamer_compositor.so";
std::string window_manager = "/Gamer_OS/kernel/modules/startAQ.so";
#else // Assuming its linux
std::string compositor = "/home/gamerpc/Gamer_OS/build/gamer_compositor";
std::string window_manager = "/home/gamerpc/Gamer_OS/build/startAQ";
#endif
void print(std::string& text){
    std::cout << "LOG: " << text << std::endl;
}
int main(){
    std::string text = "Starting: " + compositor;
    print(text);
    std::string starter = compositor + " -s " + window_manager;
    system(starter.c_str());

}