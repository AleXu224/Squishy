#include "characters.hpp"

#include "character/characters/Cyno.hpp"
#include "character/characters/Furina.hpp"
#include "character/characters/Nahida.hpp"
#include "character/characters/Xilonen.hpp"


extern void Character::initCharacters() {
	list = {
		{Character::Datas::cyno.key, Character::Datas::cyno},
		{Character::Datas::furina.key, Character::Datas::furina},
		{Character::Datas::nahida.key, Character::Datas::nahida},
		{Character::Datas::xilonen.key, Character::Datas::xilonen},
	};
}
