#include "ServerConnection.hpp"

ServerConnection::ServerConnection(int32_t port)
{
    _port = port;
}

bool ServerConnection::setup_ip_address( )
{
    std::cout << "[SERVER] Resolving host : " << _port << std::endl;
    
    int success = SDLNet_ResolveHost( &_ip_address, nullptr, _port);
    
    if ( success == -1 )
    {
        std::cout << "[SERVER] Failed to open port : " << _port << std::endl;
        
        return false;
    }
    
    std::cout << "[SERVER] Host resolved" << std::endl;
    return true;
}

bool ServerConnection::open_port_for_listening( )
{
    _tcp_socket = SDLNet_TCP_Open(&_ip_address);
    
    if (_tcp_socket == nullptr )
    {
        std::cout << "[SERVER] Failed to open port : " << _port
        <<" \n\tError : " << SDLNet_GetError()
        << std::endl;
        
        return false;
    }
    
    std::cout << "[SERVER] Success! Port opened for listening!\n";
    
    return true;
}

std::shared_ptr<ClientConnection> ServerConnection::try_to_accept_connection()
{
    if (!can_accept_connection())
        return nullptr;
    
    TCPsocket new_socket = SDLNet_TCP_Accept(_tcp_socket);
    
    if (new_socket == nullptr )
    {
        std::cout << "[SERVER] Cannot accept TCP connection : " << SDLNet_GetError() << std::endl;
        return nullptr;
    }
    
    std::shared_ptr<ClientConnection> connection(new ClientConnection(new_socket));
    
    std::cout << "Connection accepted!\n";
    
    return connection;
}

bool ServerConnection::check_for_activity() const
{
    return SDLNet_SocketReady(_tcp_socket) != 0;
}

void ServerConnection::add_to_socket_set(SDLNet_SocketSet* socket_set)
{
    SDLNet_TCP_AddSocket(*socket_set, _tcp_socket);
}

void ServerConnection::close()
{
    std::cout << "[SERVER] Closing connection.." << std::endl;
    
    SDLNet_TCP_Close(_tcp_socket);
}

bool ServerConnection::can_accept_connection()
{
    if (_tcp_socket == nullptr )
    {
        std::cout <<  "[SERVER] Can't accept, TCP Socket is null" << std::endl;
        return false;
    }
    
    bool can_accept = check_for_activity();
    
    return can_accept;
}
