#pragma once

#include "vector"

namespace Node {
	enum class CharacterSlot {
		normal,
		charged,
		plunge,
		skill,
		burst,
		passive1,
		passive2,
		constellation1,
		constellation2,
		constellation4,
		constellation6,
	};

	static inline std::vector<CharacterSlot> characterEntries{
		CharacterSlot::normal,
		CharacterSlot::charged,
		CharacterSlot::plunge,
		CharacterSlot::skill,
		CharacterSlot::burst,
		CharacterSlot::passive1,
		CharacterSlot::passive2,
		CharacterSlot::constellation1,
		CharacterSlot::constellation2,
		CharacterSlot::constellation4,
		CharacterSlot::constellation6,
	};
}// namespace Node