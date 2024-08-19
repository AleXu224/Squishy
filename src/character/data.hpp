#pragma once

#include "conditional/conditional.hpp"
#include "key.hpp"
#include "node/node.hpp"
#include "stats/character.hpp"
#include "stats/characterBase.hpp"
#include "string_view"


namespace Character {
	struct Instance;

	struct Data {
		struct Setup {
			Stats::ModsSheet mods{};
			Node::CharacterList nodes{};
		};

		const Key key;
		const std::string_view name;
		const Stats::CharacterBase baseStats;
		const Conditional::CharacterList conds{};
		const std::function<Setup(void)> setup;

		const Setup data = [](const std::function<Setup(void)> &setup) {
			return setup();
		}(setup);

		void getConds(Conditional::CharacterMap &conditionals, Stats::Character &stats) const {
			Conditional::mapConditionals(
				conditionals,
				conds
			);
		}
	};
}// namespace Character

#include "formula/formula.hpp"// IWYU pragma: export
