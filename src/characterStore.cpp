#include "characterStore.hpp"
#include "Characters/cyno.hpp"
#include "Weapons/Staff_of_the_Scarlet_Sands.hpp"

using namespace Squishy;

const std::unordered_map<std::string, ICharacterData> CharacterStore::charactersData{
    {"Cyno", Cyno},
};
const std::unordered_map<std::string, IWeaponData> CharacterStore::weaponsData{
    {"Staff of the Scarlet Sands", Staff_of_the_Scarlet_Sands},
};
std::vector<std::unique_ptr<Character>> CharacterStore::characters{};
