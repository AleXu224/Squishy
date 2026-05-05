#pragma once

#include "ISubstat.hpp"
#include "disc/instance.hpp"
#include "disc/set.hpp"
#include "cstdint"
#include "expected"
#include "string"
#include "vector"


namespace Serialization::Zod {
	struct IDisc {
		std::string setKey;
		std::string slotKey;
		uint8_t level;
		std::string rarity;
		std::string mainStatKey;
		std::string location;
		bool lock = false;
		bool trash = false;
		std::vector<ISubstat> substats;

		static IDisc fromInstance(const Disc::Instance &disc);

		std::expected<std::reference_wrapper<Disc::Instance>, std::string> createInstance() const;

		std::expected<std::reference_wrapper<const Disc::Set>, std::string> getData() const;

		std::expected<std::reference_wrapper<Disc::Instance>, std::string> isAlreadyStored() const;

		void writeToInstance(Disc::Instance &) const;
	};
}// namespace Serialization::Zod