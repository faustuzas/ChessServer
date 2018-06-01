#include "ClientConnection.hpp"

void* ClientConnection::convert_string_to_void_ptr(const std::string &str)
{
    const char* char_ptr = str.c_str();
    
    return const_cast< char* > ( char_ptr );
}

ClientConnection::ClientConnection(const std::string &ip, int32_t port)
{
    _ip = ip;
    _port = port;
}

ClientConnection::ClientConnection(const TCPsocket &conn)
{
    _tcp_socket = conn;
    _is_connected = true;
}

bool ClientConnection::setup_ip_address()
{
    int success = SDLNet_ResolveHost( &_ip_address, _ip.c_str(), _port );
    
    if ( success == -1 )
    {
        std::cout << "[CLIENT] Failed to resolve host " << _ip << " : " << _port << std::endl;
        return false;
    }
    
    std::cout  << "[CLIENT] Host resolved" << std::endl;
    return true;
}

void ClientConnection::send(std::string str)
{
    void* message_data = convert_string_to_void_ptr(str);
    int message_size = static_cast< int > ( str.length() );
    
    int bytes_sent = SDLNet_TCP_Send( _tcp_socket,  message_data,  message_size);
    
    if ( bytes_sent < message_size )
    {
        std::cout << "\tSend failed : " << SDLNet_GetError() << std::endl;
    }
}

std::string ClientConnection::read_message()
{
    char buffer[_buffer_size ];
    memset( buffer, 0, _buffer_size );
    
    int byteCount = SDLNet_TCP_Recv( _tcp_socket, buffer, _buffer_size );
    std::string received("");
    
    if ( byteCount > 0 )
    {
        buffer[byteCount] = '\0';
        received = buffer;
        
        if ( byteCount >= _buffer_size )
            std::cout << "[CLIENT] Too much data received : " << byteCount
            << "\tbuffer size : " << _buffer_size
            << "\n\tMaybe increase buffer size?" << std::endl;
    }
    else if ( byteCount == 0 )
    {
        _is_connected = false;
    }
    
    else if ( byteCount < 0 )
    {
        _is_connected = false;
        std::cout << "[CLIENT] Read Failed, terminating connection : " << SDLNet_GetError()  << std::endl;
    }
    
    return received;
}

bool ClientConnection::check_for_activity() const
{
    int i = SDLNet_SocketReady( _tcp_socket );
    return  i != 0;
}

void ClientConnection::add_to_socket_set(SDLNet_SocketSet* socket_set)
{
    SDLNet_TCP_AddSocket(*socket_set, _tcp_socket);
}

void ClientConnection::close()
{
    _is_connected = false;
    std::cout << "[CLIENT] Closing connection.." << std::endl;
    SDLNet_TCP_Close( _tcp_socket );
}

bool ClientConnection::is_connected() const
{
    return _is_connected;
}
