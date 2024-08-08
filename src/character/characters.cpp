#include "characters.hpp"

#include "character/characters/Cyno.hpp"


extern void Character::initCharacters() {
	list = {
		{Character::Datas::cyno.key, Character::Datas::cyno}
	};

	for (auto &character: list) {
		character.second.nodes = character.second.getNodes();
	}
}
