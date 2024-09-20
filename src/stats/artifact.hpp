#pragma once

#include "artifact/key.hpp"
#include "artifact/slot.hpp"
#include "option/option.hpp"
#include "stats/artifactSheet.hpp"


namespace Artifact {
	struct Set;
}

namespace Stats {
	struct Artifact {
		// The 4 piece set equipped if it exists
		std::optional<std::reference_wrapper<const ::Artifact::Set>> set{};
		std::optional<std::reference_wrapper<Option::ArtifactMap>> currentOptions{};
		std::unordered_map<::Artifact::SetKey, Option::ArtifactMap> options{};
		ArtifactSheet sheet{};

		struct Slotted {
			std::optional<::Artifact::InstanceKey> flower;
			std::optional<::Artifact::InstanceKey> plume;
			std::optional<::Artifact::InstanceKey> sands;
			std::optional<::Artifact::InstanceKey> goblet;
			std::optional<::Artifact::InstanceKey> circlet;

			[[nodiscard]] static auto getMembers() {
				return std::array{
					&Slotted::flower,
					&Slotted::plume,
					&Slotted::sands,
					&Slotted::goblet,
					&Slotted::circlet,
				};
			}
		};

		Slotted equipped{};

		static inline std::vector subStats{
			Stat::hp,
			Stat::hp_,
			Stat::atk,
			Stat::atk_,
			Stat::def,
			Stat::def_,
			Stat::er,
			Stat::em,
			Stat::cr,
			Stat::cd,
		};

		static inline std::vector flower{
			Stat::hp,
		};
		static inline std::vector plume{
			Stat::atk,
		};
		static inline std::vector sands{
			Stat::hp_,
			Stat::atk_,
			Stat::def_,
			Stat::er,
			Stat::em,
		};
		static inline std::vector goblet{
			Stat::hp_,
			Stat::atk_,
			Stat::def_,
			Stat::em,
			Stat::pyroDmg,
			Stat::hydroDmg,
			Stat::cryoDmg,
			Stat::electroDmg,
			Stat::dendroDmg,
			Stat::anemoDmg,
			Stat::geoDmg,
			Stat::physicalDmg,
		};
		static inline std::vector circlet{
			Stat::hp_,
			Stat::atk_,
			Stat::def_,
			Stat::em,
			Stat::cr,
			Stat::cd,
			Stat::hb,
		};

		[[nodiscard]] static const std::vector<Stat> &bySlot(::Artifact::Slot slot) {
			switch (slot) {
				case ::Artifact::Slot::flower:
					return flower;
				case ::Artifact::Slot::plume:
					return plume;
				case ::Artifact::Slot::sands:
					return sands;
				case ::Artifact::Slot::goblet:
					return goblet;
				case ::Artifact::Slot::circlet:
					return circlet;
			}
		}
	};
}// namespace Stats