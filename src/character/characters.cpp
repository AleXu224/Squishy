#include "characters.hpp"

#include "character/characters/Bennett.hpp"
#include "character/characters/Citlali.hpp"
#include "character/characters/Cyno.hpp"
#include "character/characters/Escoffier.hpp"
#include "character/characters/Furina.hpp"
#include "character/characters/KamisatoAyaka.hpp"
#include "character/characters/Mavuika.hpp"
#include "character/characters/Nahida.hpp"
#include "character/characters/Xilonen.hpp"


extern void Character::initCharacters() {
	list = {
		{Character::Datas::bennett.key, Character::Datas::bennett},
		{Character::Datas::citlali.key, Character::Datas::citlali},
		{Character::Datas::cyno.key, Character::Datas::cyno},
		{Character::Datas::escoffier.key, Character::Datas::escoffier},
		{Character::Datas::furina.key, Character::Datas::furina},
		{Character::Datas::kamisatoAyaka.key, Character::Datas::kamisatoAyaka},
		{Character::Datas::mavuika.key, Character::Datas::mavuika},
		{Character::Datas::nahida.key, Character::Datas::nahida},
		{Character::Datas::xilonen.key, Character::Datas::xilonen},
	};

	for (const auto &character: list) {
		character.second.init();
	}
}
