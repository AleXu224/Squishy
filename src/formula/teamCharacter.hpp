#pragma once

#include "character/data.hpp"
#include "character/instance.hpp"
#include "fmt/core.h"
#include "intermediary.hpp"
#include "stats/loadout.hpp"
#include "stats/team.hpp"

namespace Formula {
	template<IntermediaryLike T>
	struct TeamCharacter {
		size_t index = 0;
		T formula;

		[[nodiscard]] inline std::string print(const Stats::Loadout &, const Stats::Team &team, Step prevStep) const {
			const auto &character = team.characters.at(index);
			if (!character.has_value()) return "";
			auto &stats = character->get().stats;
			return fmt::format(
				"{} {}",
				stats.character.data.name,
				formula.print(stats, team, prevStep)
			);
		}

		[[nodiscard]] inline float eval(const Stats::Loadout &, const Stats::Team &team) const {
			const auto &character = team.characters.at(index);
			if (!character.has_value()) return 0.f;
			return formula.eval(character->get().stats, team);
		}
	};
}// namespace Formula