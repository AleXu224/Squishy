#pragma once

#include "data.hpp"

#include "unordered_map"

namespace Character {
	inline std::unordered_map<Character::DataKey, const Character::Data &> list;

	extern void initCharacters();
}// namespace Character
