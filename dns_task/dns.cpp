#include <cstdint>
#include <cassert>
#include <vector>
#include <string>
#include <tuple>

using Result = std::pair<uint16_t, int>;

struct Subnet {
    Subnet ( const std::string & subnet );
    std::vector<std::string> m_Chunks;
    size_t                   m_Mask;
};

Subnet::Subnet ( const std::string & subnet ) {

}

struct ECS {

};

struct TrieNode {
    std::string             m_Val;
    std::vector<TrieNode *> m_Children;
};

struct Data {
    bool       Find   ( const ECS    & ecs, 
                        Result       & r );
    bool       Insert ( const Subnet & subnet, 
                        uint16_t       pop_id );

    TrieNode * m_TrieRoot = nullptr;
};

bool Data::Insert ( const Subnet & subnet, uint16_t pop_id ) {

}

Result Route ( Data & d, const ECS & ecs ) {

}

int main ( void ) {

    return 0;
}
