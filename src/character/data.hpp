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
			Option::CharacterList opts{};
			Node::CharacterList nodes{};
		};

		DataKey key;
		std::string_view goodKey;
		std::string_view name;
		Stats::CharacterBase baseStats;
		std::function<Setup(void)> setup;

		Setup data = [](const std::function<Setup(void)> &setup) {
			return setup();
		}(setup);

		void getOpts(Option::TypesMap &options) const {
			Option::mapOptions(
				options,
				data.opts
			);
		}
	};
}// namespace Character
