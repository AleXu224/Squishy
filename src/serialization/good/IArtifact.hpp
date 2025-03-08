#pragma once

#include "ISubstat.hpp"
#include "artifact/instance.hpp"
#include "artifact/set.hpp"
#include "cstdint"
#include "expected"
#include "string"
#include "vector"


namespace Serialization::Good {
	struct IArtifact {
		std::string setKey;
		std::string slotKey;
		uint8_t level;
		uint8_t rarity;
		std::string mainStatKey;
		std::string location;
		bool lock = false;
		std::vector<ISubstat> substats;

		static std::vector<IArtifact> fromInstance(const Artifact::Instance &weapon);

		std::expected<std::reference_wrapper<Artifact::Instance>, std::string> createInstance() const;

		std::expected<std::reference_wrapper<const Artifact::Set>, std::string> getData() const;

		std::expected<std::reference_wrapper<Artifact::Instance>, std::string> isAlreadyStored() const;

		void writeToInstance(Artifact::Instance &) const;
	};
}// namespace Serialization::Good