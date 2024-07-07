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

const std::string dir = "/Gamer_OS/kernel/modules/";
const std::string Package_Commandi = "sudo pkg install "; // One of the things that I was talking about.
const std::string launchd = "/Gamer_OS/kernel/modules/launchd.so";

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

    return 0;
}
