#pragma once

#include "cstdint"
#include "expected"
#include "string"
#include "weapon/instance.hpp"


namespace Serialization::Good {
	struct IWeapon {
		std::string key;
		uint8_t level;
		uint8_t ascension;
		uint8_t refinement;
		std::string location;
		bool lock = false;

		std::vector<IWeapon> fromInstance(const Weapon::Instance &weapon);

		std::expected<std::reference_wrapper<Weapon::Instance>, std::string> createInstance() const;

		std::expected<std::reference_wrapper<const Weapon::Data>, std::string> getData() const;

		std::expected<std::reference_wrapper<Weapon::Instance>, std::string> isAlreadyStored() const;

		void writeToInstance(Weapon::Instance &) const;
	};
}// namespace Serialization::Good