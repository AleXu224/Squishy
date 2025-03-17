#pragma once

#include "cstdint"
#include "utility"
#include "utils/stringify.hpp"
#include "vector"


namespace Node {
	enum class CharacterSlot : uint8_t {
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

	static inline std::vector<CharacterSlot> characterSlots{
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

namespace Utils {
	template<>
	constexpr std::string Stringify<>(const Node::CharacterSlot &characterSlot) {
		switch (characterSlot) {
			case Node::CharacterSlot::normal:
				return "Normal Attack";
			case Node::CharacterSlot::charged:
				return "Charged Attack";
			case Node::CharacterSlot::plunge:
				return "Plunge Attack";
			case Node::CharacterSlot::skill:
				return "Elemental Skill";
			case Node::CharacterSlot::burst:
				return "Elemental Burst";
			case Node::CharacterSlot::passive1:
				return "Passive 1";
			case Node::CharacterSlot::passive2:
				return "Passive 2";
			case Node::CharacterSlot::constellation1:
				return "Constellation 1";
			case Node::CharacterSlot::constellation2:
				return "Constellation 2";
			case Node::CharacterSlot::constellation4:
				return "Constellation 4";
			case Node::CharacterSlot::constellation6:
				return "Constellation 6";
		}
		std::unreachable();
	}
}// namespace Utils