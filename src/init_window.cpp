#include <iostream>
#include <chrono>
#include <thread>
#include <cstdlib>
//#include <pkg.h>
using namespace std;
const std::string gamer_starter = "/Gamer_OS/kernel/modules/gamer_compositor.so -s /Gamer_OS/kernel/modules/startAQ.so";
const std::string gamer_services = "/Gamer_OS/kernel/modules/System_Services.so";
//bool is_package_installed(const char* pkgname) {
 //   struct pkgdb *db = nullptr;
  //  struct pkg *pkg = nullptr;
  //  bool found = false;
//

  //  if (pkgdb_open_all(&db, PKGDB_REMOTE) != EPKG_OK) {
  //      std::cerr << "Failed to open package database.\n";
      //  return false;
 //   }

   //
  //  if (pkgdb_query(db, pkgname, MATCH_EXACT) == EPKG_OK) {
   //     if (pkgdb_it_next(pkgdb_it(db), &pkg, PKG_LOAD_BASIC) == EPKG_OK) {
  //          found = true;
   //     }
  //  }

  
   // pkgdb_close(db);
  //  return found;
///}

//void Get_If_Seatd_Is_Setup(){
   // bool Get_SeatD_ISP = is_package_installed("seatd");
  //  if (Get_SeatD_ISP == true){
   //     return true;
   /// } else {
        // Sets up seatd
     //   bool Installer_F1_S = system(std::string("sudo pkg install seatd").c_str());
     //   if (Installer_F1_S == 0){
      //      std::cout << "LOG: Setup: 50%" << std::endl;
      //      bool SYSRC_ADDED_SEAT = system(std::string("sudo sysrc seatd_enable=" + '"YES').c_str());
      //      if (SYSRC_ADDED_SEAT == 0){
      //          std::cout << "LOG: Setup: 100% " << std::endl;
      ///      }
       //     system(std::string("sudo shutdown -r now").c_str());
      //  }
   // }
//}
std::string Get_Active_Time() {
    auto current_time = std::chrono::system_clock::now();
    std::time_t current_time_t = std::chrono::system_clock::to_time_t(current_time);
    return std::ctime(&current_time_t);
}
void init_drivers() {
    std::cout << "LOG: Setting up drivers " << Get_Active_Time() << std::endl;
    bool driver_intel = system("sudo kldload i915kms.ko");
    if (!driver_intel){
        // Assuming its either a virtual matchine
        // TODO Add logic for other firmwares!
        std::cout << "LOG: This matchine does not have any intel graphics chip on board. Continuing.." << std::endl;
    }
    bool virtual_matchine_DRV = system("kldload vboxvideo");
    if (virtual_matchine_DRV){
        std::cout << "LOG: Success!" << std::endl;
    } else if (virtual_matchine_DRV == 1 ){
        std::cout << "LOG: This matchine is not a virtual matchine, and the hardware itself for it is not present! Continuing.." << std::endl;
    }
  //  Get_If_Seatd_Is_Setup();
}

void Start_DE() {
    std::cout << "LOG: Launching /Gamer_OS/kernel/modules/startAQ.so " << Get_Active_Time() << std::endl;
    system("/Gamer_OS/kernel/modules/startAQ.so");
    init_drivers();
}

//void StartX() { // For FreeBSD
/// std::cout << //"LOG: Launching XServer at: " << Get_Active_Time() << std::endl;
    
  //  // Example custom startx command with options
  //  std::string command = "startx -- -dpi 96 -depth 24";
   // int result = system(command.c_str());
 //   if (result == 1){
 //       // There is no .xinitrc
   
 //   }
  //  std::string System_services_init = "/Gamer_OS/kernel/modules/System_Services.so";
  //  system(System_services_init.c_str());
  //  init_drivers();
//}
int main() {
    std::cout << "LOG: Welcome to Gamer OS! Time is " << Get_Active_Time() << std::endl;
  //  system("touch /root/.xinitrc");
 //   system("echo /Gamer_OS/kernel/modules/startAQ.so >> /root/.xinitrc");
    init_drivers();
    system(gamer_starter.c_str());
    //system(gamer_services.c_str());
   
 //   StartX(); Scrapped due to the lack of it within the compositor
    return 0;
}
