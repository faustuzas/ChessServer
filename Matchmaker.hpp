#ifndef Machmaker_hpp
#define Machmaker_hpp

#include <stdio.h>
#include <map>
#include <string>
#include "ClientConnection.hpp"
#include <time.h>
#include <algorithm>

template <class T>
struct MatchPair {
    std::shared_ptr<T> player1;
    std::shared_ptr<T> player2;
    bool player1_turn;
    
    MatchPair() :
        player1(nullptr),
        player2(nullptr)
    {
        
    }
};

class Matchmaker {
private:
    static const int HASH_LENGHT = 6;
    
    std::map<std::string, MatchPair<ClientConnection>*> _pairs;
    std::shared_ptr<ClientConnection> _waiting_player;
    
    std::string get_hash() const;
    static void send_hash_and_color_to_players(MatchPair<ClientConnection>* pair, std::string& hash);
public:
    Matchmaker();
    ~Matchmaker();
    void add_to_waiting(std::shared_ptr<ClientConnection>& connection);
    void send_to(std::string& msg) const;
};

#endif
