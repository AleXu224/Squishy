#pragma once

#include "functional"
#include "node/node.hpp"
#include "option/option.hpp"
#include "stats/sheet.hpp"
#include "stats/weaponBase.hpp"
#include "weapon/key.hpp"


namespace Weapon {
	struct Data {
		struct Setup {
			Stats::ModsSheet mods{};
			const Option::WeaponList opts{};
			Node::WeaponList nodes{};
		};

		const DataKey key;
		const std::string name;
		const Stats::WeaponBase baseStats;
		const std::function<Setup(void)> setup;

		const Setup data = [](const std::function<Setup(void)> &setup) {
			return setup();
		}(setup);

		void getOpts(Option::WeaponMap &options) const {
			Option::mapOptions(
				options,
				data.opts
			);
		}
	};
}// namespace Weapon
