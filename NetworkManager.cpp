#include "NetworkManager.hpp"

void NetworkManager::init()
{
    _client_socket_set = SDLNet_AllocSocketSet(10);
    _server_socket_set = SDLNet_AllocSocketSet(10);
}

void NetworkManager::add_server()
{
    std::shared_ptr<ServerConnection> server_con(new ServerConnection(_port++));
    
    server_con->setup_ip_address();
    server_con->open_port_for_listening();
    server_con->add_to_socket_set(&_server_socket_set);
    
    _server_connections.emplace_back(std::move(server_con));
}

std::vector<std::string> NetworkManager::get_all_messages()
{
    std::vector<std::string> message_list_copy = _message_list;
    
    _message_list.clear();
    return message_list_copy;
}

void NetworkManager::do_update()
{
    try_to_accept_connection();
    check_client_connections();
    
    SDL_Delay(50);
}

void NetworkManager::check_client_connections()
{
    int32_t count_active_connections = count_client_sockets_with_activity();
    if (count_active_connections == 0)
        return;
    
    int i = 0;
    while (count_active_connections != 0)
    {
        if (!_client_connections[i]->check_for_activity())
        {
            ++i;
            continue;
        }
        
        --count_active_connections;
        
        if ( !_client_connections[i]->is_connected() )
        {
            SDLNet_TCP_DelSocket(_client_socket_set, _client_connections[i]->socket());
            
            _client_connections.erase( std::begin(_client_connections) + i);
            ++i;
            
            std::cout << "Client has disconnected!\n";
            continue;
        }
        
        _message_list.push_back(_client_connections[i]->read_message() );
        
        if (count_active_connections == 0)
        {
            count_active_connections = count_client_sockets_with_activity();
            i = 0;
        }
    }
    
    send_messages_to_players();
}

void NetworkManager::send_messages_to_players()
{
    std::vector<std::string> messages = get_all_messages();
    
    for (auto& message : messages) {
        _matchmaker.send_to(message);
    }
}

int NetworkManager::count_connected_clients()
{
    return (int)_client_connections.size();
}

int NetworkManager::count_server_sockets_with_activity() const
{
    return SDLNet_CheckSockets(_server_socket_set, 5);
}

int NetworkManager::count_client_sockets_with_activity() const
{
    return SDLNet_CheckSockets(_client_socket_set, 5);
}

bool NetworkManager::try_to_accept_connection()
{
    int count = count_server_sockets_with_activity();
    
    int countFound = 0;
    
    for (auto& conn : _server_connections) {
        if ( countFound == count)
            break;
        
        if (!conn->can_accept_connection())
            continue;
        
        auto new_connection = conn->try_to_accept_connection();
        
        if (new_connection != nullptr)
        {
            new_connection->add_to_socket_set(&_client_socket_set);
            _matchmaker.add_to_waiting(new_connection);
            
            _client_connections.emplace_back(std::move(new_connection));
        }
    }
    
    return true;
}
