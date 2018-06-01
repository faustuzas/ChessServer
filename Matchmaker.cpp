#include "Matchmaker.hpp"

std::string Matchmaker::get_hash() const
{
    static const char alphanum[] =
    "0123456789"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz";
    
    std::string hash = "";
    
    for (int i = 0; i < HASH_LENGHT; ++i) {
        hash += alphanum[rand() % (sizeof(alphanum) - 1)];
    }
    
    std::random_shuffle(hash.begin(), hash.end());
    
    return hash;
}

Matchmaker::Matchmaker() :
    _waiting_player(nullptr)
{
    srand ((unsigned int)time(NULL));
}

void Matchmaker::add_to_waiting(std::shared_ptr<ClientConnection>& connection)
{
    if (_waiting_player == nullptr) {
        _waiting_player = connection;
    } else {
        MatchPair<ClientConnection>* pair = new MatchPair<ClientConnection>;
        std::string hash = get_hash();
        
        pair->player1 = _waiting_player;
        pair->player2 = connection;
        
        _pairs[hash] = pair;
        send_hash_and_color_to_players(pair, hash);
        
        std::cout << "Pair created. Hash - " << hash << std::endl;
        
        _waiting_player = nullptr;
    }
}

void Matchmaker::send_to(std::string& msg) const
{
    // Longer or equal that 1 message size
    if (msg.size() >= (1 + HASH_LENGHT + 2)) {
        while (msg.size() > 0) {
            std::string chunk = msg.substr(0, HASH_LENGHT + 3);
            msg = msg.substr(HASH_LENGHT + 3);
            
            if (chunk.size() == (1 + HASH_LENGHT + 2)) {
                char player = chunk.substr(0, 1)[0];
                std::string hash = chunk.substr(1, HASH_LENGHT);
                std::string data = chunk.substr(HASH_LENGHT + 1);
                
                auto pair = _pairs.find(hash)->second;
                
                if (player == '1') {
                    pair->player2->send(data);
                } else {
                    pair->player1->send(data);
                }
            }
        }
    }
}

void Matchmaker::send_hash_and_color_to_players(MatchPair<ClientConnection>* pair, std::string& hash)
{
    pair->player1->send("W" + hash);
    pair->player2->send("B" + hash);
}

Matchmaker::~Matchmaker()
{
    for (auto it = _pairs.begin(); it != _pairs.end(); ++it)
        delete it->second;
}
