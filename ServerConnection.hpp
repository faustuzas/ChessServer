#pragma once

#include <string>
#include <memory>
#include <iostream>
#include <SDL2/SDL_net.h>

#include "ClientConnection.hpp"

class ServerConnection
{
private:
    int32_t _port;
    IPaddress _ip_address;
    TCPsocket _tcp_socket;
    
public:
    ServerConnection(int32_t port);
    bool setup_ip_address();
    bool open_port_for_listening();
    std::shared_ptr<ClientConnection> try_to_accept_connection();
    bool check_for_activity() const;
    void add_to_socket_set(SDLNet_SocketSet* socket_set);
    void close();
    bool can_accept_connection();
};
