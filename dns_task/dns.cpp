#include <unordered_map>
#include <iostream>
#include <optional>
#include <cstdint>
#include <cassert>
#include <fstream>
#include <vector>
#include <memory>
#include <string>
#include <tuple>

using Result = std::pair<uint16_t, int>;

const size_t CHUNK_SIZE = 4,
             BIT_COUNT  = 4;

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//Helper functions
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
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
/**
 @brief
 */
struct Subnet {
                Subnet      ( const  std::string & subnet );
    std::string operator [] ( size_t idx ) const;
    std::vector<std::string> m_Chunks;
    size_t                   m_Mask;
};

Subnet::Subnet ( const std::string & subnet ) {
    size_t start = 0, 
           pos   = 0,
           mask  = 0;
    //Get mask from the string
    pos = subnet . find ( "/" );
    mask = m_Mask = std::stoi ( subnet . substr ( pos + 1, subnet . size ( ) - pos ) );
    mask /= BIT_COUNT;

    while ( pos != std::string::npos && mask ) {
        pos = subnet . find ( ":", start );
        if ( pos != std::string::npos ) {
            std::string chunk = subnet . substr ( start, pos - start );
            //Add ommited zeroes for more unified representation
            if ( chunk . size ( ) < CHUNK_SIZE )
                chunk = StringRepeat ( "0", CHUNK_SIZE - chunk . size ( ) ) + chunk;
            chunk = chunk . substr ( 0, mask > CHUNK_SIZE ? CHUNK_SIZE : mask );
            m_Chunks . push_back ( chunk );
            start = pos + 1;
            mask -= chunk . size ( );
        }
    }
}

std::string Subnet::operator [] ( size_t idx ) const {
    return m_Chunks[idx];
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
/**
 @brief
 */
struct TrieNode {
     TrieNode ( void );
    std::optional<uint16_t>                                    m_PoP;
    std::unordered_map<std::string, std::shared_ptr<TrieNode>> m_Children;
};

TrieNode::TrieNode ( void )
{
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
/**
 @brief
 */
struct Data {
     Data ( void );
    
    bool       Find   ( const Subnet & subnet, 
                        Result       & r );
    bool       Insert ( const Subnet & subnet, 
                        uint16_t       pop_id );

    std::shared_ptr<TrieNode> m_TrieRoot;
};

Data::Data ( void ) 
: m_TrieRoot ( std::make_shared<TrieNode> ( ) )
{}

bool Data::Insert ( const Subnet & subnet, uint16_t pop_id ) {
    std::shared_ptr<TrieNode> curr = m_TrieRoot;
    size_t chunk_idx = 0;

    while ( chunk_idx != subnet . m_Chunks . size ( ) ) {
        if ( ! curr -> m_Children . count ( subnet[chunk_idx] ) )
            curr = curr -> m_Children[subnet[chunk_idx]] = std::make_shared<TrieNode> ( );
        else curr = curr -> m_Children[subnet[chunk_idx]];
        chunk_idx++;
    }

    //Set PoP id
    curr -> m_PoP = pop_id;

    return true;
}

bool Data::Find ( const Subnet & subnet, Result & r ) {
    std::shared_ptr<TrieNode> curr = m_TrieRoot;
    size_t chunk_idx = 0;

    while ( chunk_idx != subnet . m_Chunks . size ( ) ) {
        if ( ! curr -> m_Children . count ( subnet[chunk_idx] ) ) {
            auto pop = curr -> m_PoP;
            //Return PoP id for the most specific subnet
            if ( pop ) {
                r = { *pop, 0 };
                return true;
            }
            //No subnet matches
            return false;
        }
        else curr = curr -> m_Children[subnet[chunk_idx]];
        chunk_idx++;
    }

    return true;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

Result Route ( Data & d, const Subnet & ecs ) {
    Result r = { 0, 0 };
    d . Find ( ecs, r );
    std::cout << "PoP => " << r . first << std::endl;
    return r;
}

int main ( void ) {
    Data d;
    
    //Subnet a ( "2001:49f0:d0b8::/48" );
    //Subnet b ( "2409:8904:3490::/44" );
    //Subnet c ( "2409:8915:2480::/44" );
    //d . Insert ( a, 174 );
    //d . Insert ( c, 0 );
    //for ( const auto & x : a . m_Chunks )
    //    std::cout << x << std::endl;

    //Parse all routing data
    std::ifstream ifs ( "routing-data.txt" );
    std::string subnet;
    uint16_t    pop;
    while ( ifs >> std::ws >> subnet >> pop ) {
        Subnet a ( subnet );
        d . Insert ( a, pop );
        //std::cout << "Mask: " << a . m_Mask << std::endl;
        //for ( const auto & x : a . m_Chunks )
        //  std::cout << x << std::endl;
    }

    ifs . close ( );
    std::cout << "Data Parsed\n" << std::endl;

    Result r;

    r = Route ( d, Subnet ( "2001:49f0:d0b8:8a00::/56" ) );
    assert ( r . first == 174 );

    r = Route ( d, Subnet ( "2402:8100:257d:0321::/56" ) );
    assert ( r . first == 215 );

    r = Route ( d, Subnet ( "2a01:4b40:6000:0001::/56" ) );
    assert ( r . first == 51 );

    return 0;
}
