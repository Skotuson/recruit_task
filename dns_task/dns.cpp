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
             BIT_COUNT  = 4,
             ARR_SIZE   = 128;

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

std::string HexToBin ( const std::string & hex ) {
    
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
/**
 @brief
 */
struct Subnet {
         Subnet      ( const  std::string & subnet );
    char operator [] ( size_t               idx ) const;
    std::vector<char> m_Bits;
    size_t            m_Mask;
};

Subnet::Subnet ( const std::string & subnet ) {
    size_t start = 0, 
           pos   = 0,
           mask  = 0;
    //Get mask from the string
    pos = subnet . find ( "/" );
    mask = m_Mask = std::stoi ( subnet . substr ( pos + 1, subnet . size ( ) - pos ) );
    //TODO: fractional result
    mask /= BIT_COUNT;

    while ( pos != std::string::npos && mask ) {
        pos = subnet . find ( ":", start );
        if ( pos != std::string::npos ) {
            std::string chunk = subnet . substr ( start, pos - start );
            //Add ommited zeroes for more unified representation
            if ( chunk . size ( ) < CHUNK_SIZE )
                chunk = StringRepeat ( "0", CHUNK_SIZE - chunk . size ( ) ) + chunk;
            //Cut address if remaining mask length is shorter then one chunk
            chunk = chunk . substr ( 0, mask > CHUNK_SIZE ? CHUNK_SIZE : mask );
            for ( const auto & c : chunk )
                m_Bits . push_back ( c );
            start = pos + 1;
            mask -= chunk . size ( );
        }
    }
}

char Subnet::operator [] ( size_t idx ) const {
    return m_Bits[idx];
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
/**
 @brief
 */
struct TrieNode {
    TrieNode ( void );
    std::optional<Result>                  m_PoP;
    std::vector<std::shared_ptr<TrieNode>> m_Children;
};

TrieNode::TrieNode ( void )
: m_Children ( std::vector<std::shared_ptr<TrieNode>> ( ARR_SIZE, nullptr ) )
{}

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

    size_t                    m_Size;
    std::shared_ptr<TrieNode> m_TrieRoot;
};

Data::Data ( void ) 
: m_TrieRoot ( std::make_shared<TrieNode> ( ) ),
  m_Size     ( 0 )
{}

bool Data::Insert ( const Subnet & subnet, uint16_t pop_id ) {
    std::shared_ptr<TrieNode> curr = m_TrieRoot;
    size_t chunk_idx = 0;

    while ( chunk_idx != subnet . m_Bits . size ( ) ) {
        if ( ! curr -> m_Children [subnet[chunk_idx]] ) {
            m_Size++;
            curr = curr -> m_Children[subnet[chunk_idx]] = std::make_shared<TrieNode> ( );
        }
        else curr = curr -> m_Children[subnet[chunk_idx]];
        chunk_idx++;
    }

    //TODO: handle duplicit cases
    //e.g. 2a04:2e00::/29 79 and 2a04:2e00::/32 79
    //Result pop = { pop_id, subnet . m_Mask };
    //if ( ! curr -> m_PoP || curr -> m_PoP -> second > pop . second )
    //    curr -> m_PoP = pop;
    //Set PoP id
    curr -> m_PoP = { pop_id, subnet . m_Mask };

    return true;
}

bool Data::Find ( const Subnet & subnet, Result & r ) {
    std::shared_ptr<TrieNode> curr = m_TrieRoot;
    size_t chunk_idx = 0;

    while ( chunk_idx != subnet . m_Bits . size ( ) ) {
        if ( ! curr -> m_Children [subnet[chunk_idx]] ) {
            auto pop = curr -> m_PoP;
            //Return PoP id for the most specific subnet
            if ( pop ) {
                r = *pop;
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
    std::cout << "PoP => " << r . first << ", Scope prefix-length => " << r . second << std::endl;
    return r;
}

int main ( void ) {
    Data d;
    Result r;

    Subnet a ( "2a04:2e00::/29" );
    for ( const auto & x : a . m_Bits )
        std::cout << x << " " << std::endl;

    return 0;
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
    std::cout << "Data Parsed ( " << d . m_Size << " trie nodes )\n" << std::endl;

    r = Route ( d, Subnet ( "2001:49f0:d0b8:8a00::/56" ) );
    assert ( r . first == 174 );

    r = Route ( d, Subnet ( "2402:8100:257d:0321::/56" ) );
    assert ( r . first == 215 );

    r = Route ( d, Subnet ( "2a01:4b40:6000:0001::/56" ) );
    assert ( r . first == 51 );

    r = Route ( d, Subnet ( "2409:8915:2480:1000::/56" ) );
    assert ( r . first == 236 );
    
    r = Route ( d, Subnet ( "2409:8915:2480:1000::/50" ) );
    assert ( r . first == 236 && r . second == 44 );

    r = Route ( d, Subnet ( "2a04:2e00:1234::/36" ) );
    assert ( r . first == 79 && r . second == 32 );

    //Little REPL for testing
    std::cout << "> ";
    while ( std::cin >> std::ws >> subnet ) {
        r = Route ( d, Subnet ( subnet ) );
        std::cout << "> ";
    }
        

    return 0;
}
