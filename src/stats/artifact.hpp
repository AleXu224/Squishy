#pragma once

#include "artifact/key.hpp"
#include "artifact/set.hpp"
#include "artifact/slot.hpp"
#include "stats/artifactSheet.hpp"
#include "utility"


namespace Stats {
	struct ArtifactBonus {
		const ::Artifact::Set *setPtr;
		const ::Artifact::SetBonus *bonusPtr;
	};

	struct Artifact {
		// These slots contain the necessary data to correctly identify which modifiers should be applied
		// Two of them sould cover all possible combinations:
		// 2pc 2pc, 2pc 4pc (4pc has a separate slot), 2pc (one of the slots is left empty)
		std::optional<ArtifactBonus> bonus1{};
		std::optional<ArtifactBonus> bonus2{};

		ArtifactSheet sheet{};

		struct Slotted {
			::Artifact::InstanceKey flower;
			::Artifact::InstanceKey plume;
			::Artifact::InstanceKey sands;
			::Artifact::InstanceKey goblet;
			::Artifact::InstanceKey circlet;

			[[nodiscard]] static auto getMembers() {
				return std::array{
					&Slotted::flower,
					&Slotted::plume,
					&Slotted::sands,
					&Slotted::goblet,
					&Slotted::circlet,
				};
			}

			[[nodiscard]] auto &fromSlot(this auto &&self, ::Artifact::Slot slot) {
				switch (slot) {
					case ::Artifact::Slot::flower:
						return self.flower;
					case ::Artifact::Slot::plume:
						return self.plume;
					case ::Artifact::Slot::sands:
						return self.sands;
					case ::Artifact::Slot::goblet:
						return self.goblet;
					case ::Artifact::Slot::circlet:
						return self.circlet;
				}
				std::unreachable();
			}

			void unequipAll();
		};

		struct Theorycraft {
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
			uint8_t hp = 0;
			uint8_t hp_ = 0;
			uint8_t atk = 0;
			uint8_t atk_ = 0;
			uint8_t def = 0;
			uint8_t def_ = 0;
			uint8_t er = 0;
			uint8_t em = 0;
			uint8_t cr = 0;
			uint8_t cd = 0;

			[[nodiscard]] auto &fromStat(this auto &self, ::Stat stat) {
				switch (stat) {
					case Stat::hp:
						return self.hp;
					case Stat::hp_:
						return self.hp_;
					case Stat::atk:
						return self.atk;
					case Stat::atk_:
						return self.atk_;
					case Stat::def:
						return self.def;
					case Stat::def_:
						return self.def_;
					case Stat::er:
						return self.er;
					case Stat::em:
						return self.em;
					case Stat::cr:
						return self.cr;
					case Stat::cd:
						return self.cd;
					default:
						return self.hp;
				}
			}
		};

		std::variant<Slotted, Theorycraft> equipped{};

		Slotted &getSlotted();
		const Slotted &getSlotted() const;

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

		static inline std::vector mainStats{
			Stat::hp,
			Stat::atk,
			Stat::hp_,
			Stat::atk_,
			Stat::def_,
			Stat::er,
			Stat::em,
			Stat::pyroDmg,
			Stat::hydroDmg,
			Stat::cryoDmg,
			Stat::electroDmg,
			Stat::dendroDmg,
			Stat::anemoDmg,
			Stat::geoDmg,
			Stat::physicalDmg,
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
			std::unreachable();
		}

		void refreshStats();
	};
}// namespace Stats