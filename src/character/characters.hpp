#pragma once

#include "character/characters/Cyno.hpp"
#include "data.hpp"

#include "unordered_map"

namespace Character {
	inline std::unordered_map<Character::Key, const Character::Data &> list;

	inline void initCharacters() {
		list = {
			{Character::Datas::cyno.key, Character::Datas::cyno}
		};
	}
}// namespace Character
