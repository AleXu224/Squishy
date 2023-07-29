#include "characterStore.hpp"
#include "Characters/cyno.hpp"

using namespace Squishy;

const std::unordered_map<std::string, ICharacterData> CharacterStore::charactersData{
    {"Cyno", Cyno},
};
std::vector<std::unique_ptr<Character>> CharacterStore::characters{};
