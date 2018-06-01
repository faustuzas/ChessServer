#pragma once

#include <iostream>
#include <vector>
#include <string>

#include <SDL2/SDL_net.h>

class ClientConnection
{
private:
    bool _is_connected;
    std::string _ip;
    int32_t _port;
    int32_t _wait_time = 60000;
    const int _buffer_size = 20;
    IPaddress _ip_address;
    TCPsocket _tcp_socket;
    
    void* convert_string_to_void_ptr(const std::string &str);
    
public:
    ClientConnection(const std::string &ip, int32_t port);
    ClientConnection(const TCPsocket &conn);
    bool setup_ip_address();
    void send(std::string str);
    std::string read_message();
    bool check_for_activity() const;
    void add_to_socket_set(SDLNet_SocketSet* socket_set);
    void remove_from_socket_set(SDLNet_SocketSet* socket_set);
    void close();
    bool is_connected() const;
    TCPsocket& socket() { return _tcp_socket; }
};

