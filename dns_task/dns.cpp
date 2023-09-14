#include <unordered_map>
#include <iostream>
#include <optional>
#include <cstdint>
#include <cassert>
#include <fstream>
#include <vector>
#include <memory>
#include <string>
#include <stack>
#include <tuple>

using Result = std::pair<uint16_t, int>;

const size_t CHUNK_SIZE = 4,
             BIT_COUNT  = 16,
             ARR_SIZE   = 2;

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

std::unordered_map<char, std::string> HEX_TO_BIN = {
    {'0', "0000"}, {'1', "0001"}, {'2', "0010"}, {'3', "0011"},
    {'4', "0100"}, {'5', "0101"}, {'6', "0110"}, {'7', "0111"},
    {'8', "1000"}, {'9', "1001"}, {'a', "1010"}, {'b', "1011"}, 
    {'c', "1100"}, {'d', "1101"}, {'e', "1110"}, {'f', "1111"}
};

std::string HexToBin ( const std::string & hex ) {
    std::string r;
    for ( const auto & c : hex )
        r += HEX_TO_BIN[c];
    return r;
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

    while ( pos != std::string::npos && mask ) {
        pos = subnet . find ( ":", start );
        if ( pos != std::string::npos ) {
            std::string chunk = subnet . substr ( start, pos - start );
            //Add ommited zeroes for more unified representation
            if ( chunk . size ( ) < CHUNK_SIZE )
                chunk = StringRepeat ( "0", CHUNK_SIZE - chunk . size ( ) ) + chunk;
            chunk = HexToBin ( chunk );
            //Cut address if remaining mask length is shorter then one chunk
            chunk = chunk . substr ( 0, mask > BIT_COUNT ? BIT_COUNT : mask );
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
 @brief Represents one node in a Trie.
        m_Pop holds value if the node happens to be a "terminating" node of some word
        m_Children holds pointers to node children
 */
struct TrieNode {
    TrieNode ( void );
    std::optional<Result>                  m_PoP;
    std::vector<std::shared_ptr<TrieNode>> m_Children;
};

TrieNode::TrieNode ( void )
: m_Children ( std::vector<std::shared_ptr<TrieNode>> ( ARR_SIZE, nullptr ) )
{}

using ATrieNode = std::shared_ptr<TrieNode>;
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

    size_t    m_Size;
    ATrieNode m_TrieRoot;
};

Data::Data ( void ) 
: m_TrieRoot ( std::make_shared<TrieNode> ( ) ),
  m_Size     ( 0 )
{}

bool Data::Insert ( const Subnet & subnet, uint16_t pop_id ) {
    ATrieNode curr = m_TrieRoot;
    size_t chunk_idx = 0;

    while ( chunk_idx != subnet . m_Bits . size ( ) ) {
        size_t idx = subnet[chunk_idx] - '0';
        if ( ! curr -> m_Children [idx] ) {
            m_Size++;
            curr = curr -> m_Children[idx] = std::make_shared<TrieNode> ( );
        }
        else curr = curr -> m_Children[idx];
        chunk_idx++;
    }

    //Set PoP id
    curr -> m_PoP = { pop_id, subnet . m_Mask };

    return true;
}

bool Data::Find ( const Subnet & subnet, Result & r ) {
    ATrieNode curr = m_TrieRoot;
    std::stack<ATrieNode> nodeStack;
    nodeStack . push ( m_TrieRoot );
    size_t chunk_idx = 0;

    //Try to traverse the trie and save all visited nodes onto a stack for future backtracking.
    while ( chunk_idx != subnet . m_Bits . size ( ) ) {
        auto f = nodeStack . top ( );
        auto node = f -> m_Children[subnet[chunk_idx] - '0'];
        
        if ( node ) nodeStack . push ( node );
        else {
            auto pop = f -> m_PoP;
            if ( pop ) {
                r = * pop;
                return true;
            }
            break;
        }
        chunk_idx++;
    }

    //No suitable subnet was found on the first pass, backtrack in attempt to find one.
    while ( ! nodeStack . empty ( ) ) {
        auto f = nodeStack . top ( );
        auto pop = f -> m_PoP;
        if ( pop ) {
            r = * pop;
            return true;
        }
        nodeStack . pop ( );
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
    assert ( r . first == 174 && r . second == 48 );

    r = Route ( d, Subnet ( "2402:8100:257d:0321::/56" ) );
    assert ( r . first == 215 && r . second == 48 );

    r = Route ( d, Subnet ( "2402:8100:2575::/50" ) );
    assert ( r . first == 215 && r . second == 47 );

    r = Route ( d, Subnet ( "2a01:4b40:6000:0001::/56" ) );
    assert ( r . first == 51 && r . second == 37 );

    r = Route ( d, Subnet ( "2409:8915:2480:1000::/56" ) );
    assert ( r . first == 236 && r . second == 44 );

    r = Route ( d, Subnet ( "2409:8915:2449:1000::/56" ) );
    assert ( r . first == 236 && r . second == 45 );
    
    //Doesn't belong to any subnet
    r = Route ( d, Subnet ( "2409:8915:2480:1000::/8" ) );
    assert ( ! r . first && ! r . second );

    //Similiar addresses test
    r = Route ( d, Subnet ( "2a04:2e00:1234::/36" ) );
    assert ( r . first == 79 && r . second == 32 );

    r = Route ( d, Subnet ( "2a04:2e01:0101::/36" ) );
    assert ( r . first == 79 && r . second == 29 );

    r = Route ( d, Subnet ( "2a04:2e06:0101::/36" ) );
    assert ( r . first == 202 && r . second == 32 );

    ////Little REPL for testing
    //std::cout << "> ";
    //while ( std::cin >> std::ws >> subnet ) {
    //    r = Route ( d, Subnet ( subnet ) );
    //    std::cout << "> ";
    //}
        
    return 0;
}
