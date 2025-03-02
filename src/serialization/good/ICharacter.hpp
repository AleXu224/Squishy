#pragma once

#include "character/instance.hpp"
#include "cstdint"
#include "glaze/glaze.hpp"// IWYU pragma: keep
#include <string>


namespace Serialization::Good {
	struct ICharacter {
		std::string key;
		uint8_t level;
		uint8_t constellation;
		uint8_t ascension;

		struct Talent {
			uint32_t auto_;
			uint32_t skill;
			uint32_t burst;
		} talent;

		ICharacter fromInstance(const Character::Instance &character);

		std::expected<Character::Instance, std::string> toInstance() const;
	};
}// namespace Serialization::Good

template<>
struct glz::meta<Serialization::Good::ICharacter::Talent> {
	using T = Serialization::Good::ICharacter::Talent;
	static constexpr auto value = object("auto", &T::auto_, &T::skill, &T::burst);
};