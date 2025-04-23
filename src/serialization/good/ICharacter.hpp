#pragma once

#include "character/instance.hpp"
#include "cstdint"
#include "expected"
#include <string>


namespace Serialization::Good {
	struct ICharacter {
		std::string key;
		uint8_t level;
		uint8_t constellation;
		uint8_t ascension;

		struct Talent {
			int32_t auto_;
			int32_t skill;
			int32_t burst;
		} talent;

		static ICharacter fromInstance(const Character::Instance &character);

		std::expected<std::reference_wrapper<Character::Instance>, std::string> createInstance() const;

		std::expected<std::reference_wrapper<const Character::Data>, std::string> getData() const;

		std::expected<std::reference_wrapper<Character::Instance>, std::string> isAlreadyStored() const;

		void writeToInstance(Character::Instance &) const;
	};
}// namespace Serialization::Good
