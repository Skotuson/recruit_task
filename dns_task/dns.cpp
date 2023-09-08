#include <cstdint>
#include <cassert>
#include <string>
#include <tuple>

using Result = std::pair<uint16_t, int>;

struct TrieNode {
    std::string             m_Val;
    std::vector<TrieNode *> m_Children;
};

struct Data {
    bool       Find   ( const ECS & ecs, 
                        Result    & r );
    bool       Insert ( const ECS & ecs );

    TrieNode * m_TrieRoot = nullptr;
};

struct ECS {
    ECS ( const std::string & ecs );
    std::vector<std::string> m_Subnets;
};

Result Route ( Data & d, const ECS & ecs ) {

}

int main ( void ) {

    return 0;
}
