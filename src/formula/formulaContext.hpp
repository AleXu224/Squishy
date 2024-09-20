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
		const Stats::Loadout &target;
		const Stats::Team &team;
		const Stats::Enemy &enemy;
		const std::variant<const Reaction::None *, const Reaction::Amplifying *, const Reaction::Additive *> reaction{};

		[[nodiscard]] Context withSource(const Stats::Loadout &newSource) const {
			return {
				.source = newSource,
				.target = target,
				.team = team,
				.enemy = enemy,
			};
		}
	};
}// namespace Formula