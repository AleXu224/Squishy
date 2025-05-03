#pragma once

#include "variant"
#include <vector>

namespace Reaction {
	struct None;
	struct Amplifying;
	struct Additive;
}// namespace Reaction

namespace Stats {
	struct State;
	struct Loadout;
	struct Team;
	struct Enemy;
}// namespace Stats

namespace Combo {
	struct Option;
}

namespace Formula {
	struct Context {
		const Stats::State &source;
		const Stats::State &active;
		const Stats::Team &team;
		const Stats::Enemy &enemy;
		const std::variant<const Reaction::None *, const Reaction::Amplifying *, const Reaction::Additive *> reaction{};
		std::vector<Combo::Option> *optionStore = nullptr;

		[[nodiscard]] Context withSource(const Stats::State &newSource) const {
			return {
				.source = newSource,
				.active = active,
				.team = team,
				.enemy = enemy,
				.reaction = reaction,
				.optionStore = optionStore,
			};
		}

		[[nodiscard]] Context withReaction(const std::variant<const Reaction::None *, const Reaction::Amplifying *, const Reaction::Additive *> &newReaction) const {
			return {
				.source = source,
				.active = active,
				.team = team,
				.enemy = enemy,
				.reaction = newReaction,
				.optionStore = optionStore,
			};
		}
	};
}// namespace Formula