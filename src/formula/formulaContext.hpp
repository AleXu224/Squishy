#pragma once

#include "variant"

namespace Reaction {
	struct None;
	struct Amplifying;
	struct Additive;
}// namespace Reaction

namespace Stats {
	struct Loadout;
	struct Team;
	struct Enemy;
}// namespace Stats

namespace Formula {
	struct Context {
		const Stats::Loadout &source;
		const Stats::Loadout &active;
		const Stats::Team &team;
		const Stats::Enemy &enemy;
		const std::variant<const Reaction::None *, const Reaction::Amplifying *, const Reaction::Additive *> reaction{};

		[[nodiscard]] Context withSource(const Stats::Loadout &newSource) const {
			return {
				.source = newSource,
				.active = active,
				.team = team,
				.enemy = enemy,
				.reaction = reaction,
			};
		}

		[[nodiscard]] Context withReaction(const std::variant<const Reaction::None *, const Reaction::Amplifying *, const Reaction::Additive *> &newReaction) const {
			return {
				.source = source,
				.active = active,
				.team = team,
				.enemy = enemy,
				.reaction = newReaction,
			};
		}
	};
}// namespace Formula