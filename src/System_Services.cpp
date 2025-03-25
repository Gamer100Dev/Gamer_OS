#include <iostream>
#include <chrono>
#include <cstdlib>
#include <filesystem>
#include <X11/Xlib.h>
#include <X11/extensions/Xrandr.h>
#include <iomanip>
#include <vector>
#include <cstring>
#include <unistd.h>
#include <pwd.h>
#include <string>
#include <array>

// Dev_Notes:
// This is closely packed to FreeBSD and may impact compatibility with other OSes such as linux so be very wary
// I will use ifdefs for FreeBSD but functionality may be reduced for other OSes
// Thank you for your reading!
// Coded by Gamer100_Dev
// FreeBSD License copyright

struct Users{
    std::string root; // Main Root user | System level operations
    // For user-level operations, goes under here if registered.
};
const std::string dir = "/Gamer_OS/kernel/modules/";
const std::string profile_dat = "/Gamer_OS/profile/user.dat";
const std::string Package_Commandi = "sudo pkg install "; // One of the things that I was talking about.
#ifdef __FREEBSD__
const std::string launchd = "/Gamer_OS/kernel/modules/launchd.so";
#else
    const std::string launchd = "/home/gamerpc/Gamer_OS/build/launchd"; // For linux
#endif

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

int print(const std::string& text) {
    std::cout << "LOG: System_Services " + Get_Active_Time() + ": " << text << std::endl;
    return 0;
}

struct Packages {
    std::string xorg = "xorg";
    std::string pulseaudio = "pulseaudio";
    std::string NetworkManager = "networkmgr";
    std::string qt5core = "qt5-core";
    std::string qt5widgets = "qt5-widgets";
    std::vector<std::string> packagesToInstall;
};
bool isPackageInstalled(const std::string& packageName) {
    std::string command = "pkg info " + packageName;

    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) {
        std::cerr << "Error executing command." << std::endl;
        return false;
    }

    char buffer[128];
    std::string result = "";
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        result += buffer;
    }

    pclose(pipe);

    return result.find(packageName) != std::string::npos;
}

void CheckIfPackagesExist(Packages& packages) {

    if (isPackageInstalled(packages.xorg)) {
        print(packages.xorg + " is installed.");
    } else {
        print(packages.xorg + " is NOT installed. Attempting to install...");
        packages.packagesToInstall.push_back(packages.xorg);
    }

   
    if (isPackageInstalled(packages.pulseaudio)) {
        print(packages.pulseaudio + " is installed.");
    } else {
        print(packages.pulseaudio + " is NOT installed. Attempting to install...");
        packages.packagesToInstall.push_back(packages.pulseaudio);
    }


    if (isPackageInstalled(packages.NetworkManager)) {
        print(packages.NetworkManager + " is installed.");
    } else {
        print(packages.NetworkManager + " is NOT installed. Attempting to install...");
        packages.packagesToInstall.push_back(packages.NetworkManager);
    }


    if (isPackageInstalled(packages.qt5core)) {
        print(packages.qt5core + " is installed.");
    } else {
        print(packages.qt5core + " is NOT installed. Attempting to install...");
        packages.packagesToInstall.push_back(packages.qt5core);
    }

  
    if (isPackageInstalled(packages.qt5widgets)) {
        print(packages.qt5widgets + " is installed.");
    } else {
        print(packages.qt5widgets + " is NOT installed. Attempting to install...");
        packages.packagesToInstall.push_back(packages.qt5widgets);
    }
}

void installMissingPackages(const Packages& packages) {
    for (const auto& package : packages.packagesToInstall) {
        std::string command = "sudo pkg install -y " + package;
        int result = system(command.c_str());
        if (result == 0) {
            print(package + " installed successfully.");
        } else {
            print("Failed to install " + package + ". Check logs for details.");
        }
    }
}
void launchPulseAudioDaemon() {

    if (system("pgrep pulseaudio > /dev/null") == 0) {
        std::cout << "PulseAudio daemon is already running." << std::endl;
        return;
    }


    int result = system("exec ./launchd.so --run pulseaudio --start");

    if (result == 0) {
        std::cout << "PulseAudio daemon started successfully." << std::endl;
    } else {
        std::cerr << "Failed to start PulseAudio daemon." << std::endl;
    }
}
void adjustVolume(int delta) {

    std::string command = "pactl set-sink-volume @DEFAULT_SINK@ ";
    command += std::to_string(delta) + "%";

    int result = system(command.c_str());

    if (result == 0) {
        std::cout << "Volume adjusted successfully." << std::endl;
    } else {
        std::cerr << "Failed to adjust volume." << std::endl;
    }
}

void startNetworkManagerService() {
    // Check if NetworkManager is already running
    if (system("exec pgrep NetworkManager > /dev/null") == 0) {
        std::cout << "NetworkManager service is already running." << std::endl;
        return;
    }

    // Attempt to start NetworkManager
    int result = system("sudo service NetworkManager onestart");

    if (result == 0) {
        std::cout << "NetworkManager service started successfully." << std::endl;
    } else {
        std::cerr << "Failed to start NetworkManager service." << std::endl;
        // This might be due to it not being enabled in rc.conf
        std::string command = "echo 'networkmgr_enable=YES' | sudo tee -a /etc/rc.conf";
        int command_R = system(command.c_str());
        if (command_R == 0) {
            std::cout << "Successfully added networkmgr_enable=YES to /etc/rc.conf. Network Manager should work after reboot or manual start." << std::endl;
        } else {
            std::cerr << "Failed to add networkmgr_enable=YES to /etc/rc.conf. Check if Network Manager is installed and executable is present." << std::endl;
        }
    }
}

int main() {
    print("System_Services is loading!");

    Packages packages;
    CheckIfPackagesExist(packages);
    if (!packages.packagesToInstall.empty()) {
        installMissingPackages(packages);
    }
    CheckIfPackagesExist(packages);
    std::cout << "Current user: " << getCurrentUser() << std::endl;
    std::cout << "Active time: " << Get_Active_Time() << std::endl;
    print("Launching PulseAudio Services");
    launchPulseAudioDaemon();
    int delta_P = 10; 
    int delta_N = -10; 
    // adjustVolume(delta); in theory
    print("Launching Networkmgr!");
    startNetworkManagerService();
    print("SYSTEM_READY! System is up!");
    print("Obtaining Graphics");
    bool Run_GI = system(std::string('dbus-launch "startx /Gamer_OS/kernel/modules/gamer_compositor.so -s .Gamer_OS/kernel/modules/startAQ.so"').c_str());
    if (Run_GI == 0 ){
        print("Launched Succesfullly! 0");
    } else {
        print("Error running the graphics interface");
    }
    return 0;
}
