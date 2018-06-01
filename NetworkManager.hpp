#pragma once

#include <SDL2/SDL_net.h>
#include <vector>

#include "ServerConnection.hpp"
#include "ClientConnection.hpp"
#include "Matchmaker.hpp"

class NetworkManager
{
private:
    SDLNet_SocketSet _server_socket_set;
    SDLNet_SocketSet _client_socket_set;
    std::vector<std::string> _message_list;
    std::vector<std::shared_ptr<ServerConnection> > _server_connections;
    std::vector<std::shared_ptr<ClientConnection> > _client_connections;
    
    Matchmaker _matchmaker;
    
    int32_t _port = 5000;
    
    std::vector<std::string> get_all_messages();
    void check_client_connections();
    int count_connected_clients();
    int count_server_sockets_with_activity() const;
    int count_client_sockets_with_activity() const;
    bool try_to_accept_connection();
    
    void send_messages_to_players();
public:
    void init();
    void add_server();
    void do_update();
};
