#pragma once

#include "conditional/conditional.hpp"
#include "functional"
#include "node/node.hpp"
#include "stats/weaponBase.hpp"
#include "weapon/key.hpp"
#include "stats/sheet.hpp"


namespace Weapon {
	struct Data {
		struct Setup {
			Stats::ModsSheet mods{};
			Node::WeaponList nodes{};
		};

		const Key key;
		const std::string name;
		const Stats::WeaponBase baseStats;
		const Conditional::WeaponList conds{};
		const std::function<Setup(void)> setup;

		const Setup data = [](const std::function<Setup(void)> &setup) {
			return setup();
		}(setup);

		void getConds(Conditional::WeaponMap &conditionals) const {
			Conditional::mapConditionals(
				conditionals,
				conds
			);
		}
	};
}// namespace Weapon
