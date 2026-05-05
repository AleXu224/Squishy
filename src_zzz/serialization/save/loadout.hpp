#pragma once

#include "disc/key.hpp"
#include "engine/key.hpp"
#include "misc/specialty.hpp"
#include "stats/disc.hpp"
#include "stats/stat.hpp"


#include "glaze/glaze.hpp"// IWYU pragma: keep

namespace Serialization::Save {
	struct DiscSlotted {
		::Disc::InstanceKey one;
		::Disc::InstanceKey two;
		::Disc::InstanceKey three;
		::Disc::InstanceKey four;
		::Disc::InstanceKey five;
		::Disc::InstanceKey six;

		static DiscSlotted fromInstance(const ::Stats::Disc::Slotted &);
		::Stats::Disc::Slotted toInstance() const;
	};

	struct DiscTC {
		::Stats::Disc::Theorycraft::Set set1;
		::Disc::SetKey set2;

		struct MainStat {
			Stat stat;
			uint8_t level = 15;
			uint8_t rarity = 5;
		};
		std::array<MainStat, 6> mainStats{
			MainStat{Stat::hp},
			MainStat{Stat::atk},
			MainStat{Stat::def},
			MainStat{Stat::atk_},
			MainStat{Stat::atk_},
			MainStat{Stat::atk_},
		};

		struct RollCount {
			::Stat stat;
			uint8_t count;
		};
		std::vector<RollCount> subStats;

		static DiscTC fromInstance(const ::Stats::Disc::Theorycraft &);
		::Stats::Disc::Theorycraft toInstance() const;
	};

	using DiscTypes = std::variant<DiscSlotted, DiscTC>;

	struct Loadout {
		::Engine::InstanceKey engineInstanceKey;
		DiscTypes disc;

		static Loadout fromInstance(const ::Stats::Loadout &);
		::Stats::Loadout toInstance(Misc::Specialty specialty) const;
	};
}// namespace Serialization::Save

template<>
struct glz::meta<Serialization::Save::DiscSlotted> {
	using T = Serialization::Save::DiscSlotted;
	static constexpr auto value = object(&T::one, &T::two, &T::three, &T::four, &T::five, &T::six);
};
template<>
struct glz::meta<Serialization::Save::DiscTC> {
	using T = Serialization::Save::DiscTC;
	static constexpr auto value = object(&T::set1, &T::set2, &T::mainStats, &T::subStats);
};
template<>
struct glz::meta<Serialization::Save::DiscTypes> {
	static constexpr std::string_view tag = "type";
};