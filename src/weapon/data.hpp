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
			Option::WeaponList opts{};
			Node::WeaponList nodes{};
		};

		DataKey key;
		std::string_view goodKey;
		std::string name;
		Stats::WeaponBase baseStats;
		std::function<Setup(void)> setup;

		mutable std::unique_ptr<const Setup> data = nullptr;

		void init() const {
			data = std::make_unique<Setup>(setup());
		}

		void getOpts(Option::TypesMap &options) const {
			Option::mapOptions(
				options,
				data->opts
			);
		}
	};
}// namespace Weapon
