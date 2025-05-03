#pragma once

#include "stats/artifact.hpp"
#include "stats/character.hpp"
#include "stats/weapon.hpp"
#include "weapon/key.hpp"

namespace Stats {
	struct Loadout {
		::Weapon::InstanceKey weaponInstanceKey;
		Stats::Weapon *weapon;
		Stats::Artifact artifact;

		void init(Option::TypesMap &options);

		void swapWeapon(::Weapon::InstanceKey);
	};

	struct State {
		Stats::Character stats;
		std::optional<uint32_t> loadoutIndex = std::nullopt;
		Option::TypesMap options;
		Loadout equippedLoadout;
		std::vector<Loadout> loadouts{};

		void init();

		[[nodiscard]] inline Loadout &loadoutByIndex(const std::optional<uint32_t> &index) {
			if (!index.has_value()) return equippedLoadout;
			return loadouts.at(index.value());
		}
		[[nodiscard]] inline const Loadout &loadoutByIndex(const std::optional<uint32_t> &index) const {
			if (!index.has_value()) return equippedLoadout;
			return loadouts.at(index.value());
		}
		[[nodiscard]] inline Loadout &loadout() {
			return loadoutByIndex(loadoutIndex);
		}
		[[nodiscard]] inline const Loadout &loadout() const {
			return loadoutByIndex(loadoutIndex);
		}
	};
}// namespace Stats