#include <iostream>
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
    size_t start = 0, 
           pos = 0; 
    while ( pos != std::string::npos ) {
        pos = subnet . find ( ":", start );
        if ( pos != std::string::npos ) {
            m_Chunks . push_back ( subnet . substr ( start, pos - start ) );
            start = pos + 1;
        }
    }
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
    Subnet a ( "2001:49f0:d0b8::/48" );
    for ( const auto & x : a . m_Chunks )
        std::cout << x << std::endl;
    return 0;
}
