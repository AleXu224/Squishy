#pragma once
#include "IWeaponData.hpp"
#include "character.hpp"
#include "memory"
#include <unordered_map>

namespace Squishy {
    struct CharacterStore {
        // Data
        static const std::unordered_map<std::string, ICharacterData> charactersData;
        static const std::unordered_map<std::string, IWeaponData> weaponsData;
        
        static std::vector<std::unique_ptr<Character>> characters;
    };
}