#pragma once

#include "key.hpp"
#include "node/node.hpp"
#include "option/option.hpp"
#include "stats/character.hpp"
#include "stats/characterBase.hpp"
#include "string_view"


namespace Character {
	struct Data {
		struct Setup {
			Stats::ModsSheet mods{};
			Node::CharacterList nodes{};
		};

		const DataKey key;
		const std::string_view name;
		const Stats::CharacterBase baseStats;
		const Option::CharacterList opts{};
		const std::function<Setup(void)> setup;

		const Setup data = [](const std::function<Setup(void)> &setup) {
			return setup();
		}(setup);

		void getOpts(Option::CharacterMap &options) const {
			Option::mapOptions(
				options,
				opts
			);
		}
	};
}// namespace Character
