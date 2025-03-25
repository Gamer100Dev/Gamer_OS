#include <iostream>
#include <thread>
#include <cstdlib>
#include <fstream>

std::string compositor = "/Gamer_OS/kernel/modules/gamer_compositor.so";
std::string window_manager = "/Gamer_OS/kernel/modules/startAQ.so";
void print(std::string& text){
    std::cout << "LOG: " << text << std::endl;
}
int main(){
    std::string text = "Starting: " + compositor;
    print(text);
    std::string starter = compositor + " -s " + window_manager;
    system(starter.c_str());

}