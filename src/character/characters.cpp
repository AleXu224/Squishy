#include "characters.hpp"

#include "character/characters/Cyno.hpp"
#include "character/characters/Furina.hpp"


extern void Character::initCharacters() {
	list = {
		{Character::Datas::cyno.key, Character::Datas::cyno},
		{Character::Datas::furina.key, Character::Datas::furina}
	};
}
