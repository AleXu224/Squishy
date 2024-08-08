#include "character.hpp"

#include "character/data.hpp"

Stats::Character::Character(const ::Character::Data &data)
	: data(data), base(data.baseStats), sheet{} {}
