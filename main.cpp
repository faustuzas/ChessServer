#include <iostream>
#include <sstream>
#include <vector>
#include <string>

#include "NetworkManager.hpp"

int main( )
{
    std::cout
    << "\n================================================\n"
    << "Chess server"
    << "\n================================================\n";
    
    NetworkManager net_manager;
    
    net_manager.init();
    net_manager.add_server();
    
    try {
        while(true) {
            net_manager.do_update();
            SDL_Delay(200);
        }
    } catch (std::exception& e) {
        std::cout << e.what() << std::endl;
    }
    
    return 0;
}

