#include <iostream>
#include <optional>
#include <cstdint>
#include <cassert>
#include <vector>
#include <memory>
#include <string>
#include <tuple>

using Result = std::pair<uint16_t, int>;

const size_t CHUNK_SIZE = 4;

/**
 @brief Returns repeated string
 @param str String to repeat
 @param nrepeats Number of repetitions of given string
 */
std::string StringRepeat ( const std::string & str, size_t nrepeats ) {
    std::string r = "";
    while ( nrepeats-- )
        r += str;
    return r;
}

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
            std::string chunk = subnet . substr ( start, pos - start );
            //Add ommited zeroes for more unified representation
            if ( chunk . size ( ) < CHUNK_SIZE )
                chunk = StringRepeat ( "0", CHUNK_SIZE - chunk . size ( ) ) + chunk;
            m_Chunks . push_back ( chunk );
            start = pos + 1;
        }
    }
    //Get mask from the remaining string
    m_Mask = std::stoi ( subnet . substr ( start + 1, subnet . size ( ) - start ) );
}

struct ECS {

};

struct TrieNode {
     TrieNode ( const std::string & val );
     //Overloaded ctor to store the pop value into optional
     TrieNode ( const std::string & val,
                uint16_t            pop );
    std::string             m_Val;
    std::optional<uint16_t> m_PoP;
    std::vector<TrieNode *> m_Children;
};

TrieNode::TrieNode ( const std::string & val )
: m_Val ( val )
{
}

TrieNode::TrieNode ( const std::string & val, uint16_t pop )
: m_Val ( val ),
  m_PoP ( pop )
{
}

struct Data {
     Data ( void );
    ~Data ( void );

    bool       Find   ( const ECS    & ecs, 
                        Result       & r );
    bool       Insert ( const Subnet & subnet, 
                        uint16_t       pop_id );

    std::shared_ptr<TrieNode> m_TrieRoot;
};

Data::Data ( void ) 
: m_TrieRoot ( std::make_shared<TrieNode> ( "" ) )
{}

bool Data::Insert ( const Subnet & subnet, uint16_t pop_id ) {
    return false;
}

bool Data::Find ( const ECS & ecs, Result & r ) {
    return false;
}

Result Route ( Data & d, const ECS & ecs ) {
    return { 0, 0 };
}

int main ( void ) {
    //Subnet a ( "2001:49f0:d0b8::/48" );
    //for ( const auto & x : a . m_Chunks )
    //    std::cout << x << std::endl;
    
    //Parse all routing data
    std::string subnet;
    uint16_t    pop;
    while ( std::cin >> std::ws >> subnet >> pop ) {
        Subnet a ( subnet );
        //std::cout << "Mask: " << a . m_Mask << std::endl;
        //for ( const auto & x : a . m_Chunks )
        //  std::cout << x << std::endl;
    }
    return 0;
}
