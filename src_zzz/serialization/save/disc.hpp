#pragma once

#include "disc/instance.hpp"
#include "disc/key.hpp"
#include "disc/slot.hpp"
#include "stats/stat.hpp"
#include <optional>


namespace Serialization::Save {
	struct DiscSubStat {
		::Stat stat;
		bool activated = true;
		float value;
	};

	struct Disc {
		::Disc::InstanceKey instanceKey{};
		::Disc::SetKey setKey{};
		::Disc::Partition partition = ::Disc::Partition::one;
		::Stat mainStat = Stat::hp;
		std::array<std::optional<DiscSubStat>, 4> subStats{};
		uint8_t level{};
		uint8_t rarity = 5;

		static Disc fromInstance(const ::Disc::Instance &);
		::Disc::Instance toInstance() const;
	};
}// namespace Serialization::Save