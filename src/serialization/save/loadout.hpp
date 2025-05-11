#pragma once

#include "artifact/key.hpp"
#include "misc/weaponType.hpp"
#include "stats/artifact.hpp"
#include "stats/stat.hpp"
#include "weapon/key.hpp"


#include "glaze/glaze.hpp"// IWYU pragma: keep

namespace Serialization::Save {
	struct ArtifactSlotted {
		::Artifact::InstanceKey flower;
		::Artifact::InstanceKey plume;
		::Artifact::InstanceKey sands;
		::Artifact::InstanceKey goblet;
		::Artifact::InstanceKey circlet;

		static ArtifactSlotted fromInstance(const ::Stats::Artifact::Slotted &);
		::Stats::Artifact::Slotted toInstance() const;
	};

	struct ArtifactTC {
		::Stats::Artifact::Theorycraft::Set set1;
		::Artifact::SetKey set2;

		struct MainStat {
			Stat stat;
			uint8_t level = 20;
			uint8_t rarity = 5;
		};
		std::array<MainStat, 5> mainStats{
			MainStat{Stat::hp},
			MainStat{Stat::atk},
			MainStat{Stat::atk_},
			MainStat{Stat::atk_},
			MainStat{Stat::atk_},
		};

		struct RollCount {
			::Stat stat;
			uint8_t count;
		};
		std::vector<RollCount> subStats;

		static ArtifactTC fromInstance(const ::Stats::Artifact::Theorycraft &);
		::Stats::Artifact::Theorycraft toInstance() const;
	};

	using ArtifactTypes = std::variant<ArtifactSlotted, ArtifactTC>;

	struct Loadout {
		::Weapon::InstanceKey weaponInstanceKey;
		ArtifactTypes artifact;

		static Loadout fromInstance(const ::Stats::Loadout &);
		::Stats::Loadout toInstance(Misc::WeaponType weaponType) const;
	};
}// namespace Serialization::Save

template<>
struct glz::meta<Serialization::Save::ArtifactSlotted> {
	using T = Serialization::Save::ArtifactSlotted;
	static constexpr auto value = object(&T::flower, &T::plume, &T::sands, &T::goblet, &T::circlet);
};
template<>
struct glz::meta<Serialization::Save::ArtifactTC> {
	using T = Serialization::Save::ArtifactTC;
	static constexpr auto value = object(&T::mainStats, &T::subStats);
};
template<>
struct glz::meta<Serialization::Save::ArtifactTypes> {
	static constexpr std::string_view tag = "type";
};