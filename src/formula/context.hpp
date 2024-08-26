#pragma once

namespace Stats {
	struct Loadout;
	struct Team;
}// namespace Stats

namespace Formula {
	struct Context {
		const Stats::Loadout &source;
		const Stats::Loadout &target;
		const Stats::Team &team;

		[[nodiscard]] inline Context withSource(const Stats::Loadout &newSource) const {
			return {
				.source = newSource,
				.target = target,
				.team = team,
			};
		}
	};
}// namespace Formula