#pragma once

#include "print"

enum class Talent {
	normal,
	charged,
	plunge,
	skill,
	burst,
	passive1,
	passive2,
	constellation1,
	constellation2,
	constellation3,
	constellation4,
	constellation5,
	constellation6,
};

struct Talents {
	uint8_t normal;
	uint8_t skill;
	uint8_t burst;

	[[nodiscard]] uint8_t getLevel(const Talent &t) const {
		switch(t) {
			case Talent::normal:
				return normal;
				break;
			case Talent::skill:
				return skill;
				break;
			case Talent::burst:
				return burst;
				break;
			default:
				std::println("Warning: asked for a talent that isn't supported {}", static_cast<int>(t));
				return normal;
		}
	}
};
