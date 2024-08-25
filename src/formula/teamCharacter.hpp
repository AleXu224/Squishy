#pragma once

#include "character/data.hpp"
#include "character/instance.hpp"
#include "fmt/core.h"
#include "intermediary.hpp"
#include "stats/loadout.hpp"
#include "stats/team.hpp"

namespace Formula {
	template<FloatFormula T>
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

	struct TeamInfusion {
		[[nodiscard]] static inline std::string print(const Stats::Loadout &stats, const Stats::Team &team, Step) {
			auto elem = eval(stats, team);
			if (elem.has_value()) {
				return fmt::format("{}", Utils::Stringify(elem.value()));
			}
			return "None";
		}

		[[nodiscard]] static inline Utils::JankyOptional<Misc::Element> eval(const Stats::Loadout &stats, const Stats::Team &team) {
			const auto &character1 = team.characters.at(0);
			const auto &character2 = team.characters.at(1);
			const auto &character3 = team.characters.at(2);
			const auto &character4 = team.characters.at(3);
			auto getInfusion = [&](const std::optional<std::reference_wrapper<Character::Instance>> &character) -> Utils::JankyOptional<Misc::Element> {
				if (character.has_value() && character->get().stats.character.sheet.teamInfusion.hasValue()) return character->get().stats.character.sheet.teamInfusion.eval(stats, team);
				return {};
			};
			auto infusion1 = getInfusion(character1);
			auto infusion2 = getInfusion(character2);
			auto infusion3 = getInfusion(character3);
			auto infusion4 = getInfusion(character4);
			if (infusion1.has_value()) return infusion1;
			if (infusion2.has_value()) return infusion2;
			if (infusion3.has_value()) return infusion3;
			if (infusion4.has_value()) return infusion4;
			return {};
		}
	};

	struct CharacterTeamInfusion {
		Formula::ElementNode val;

		[[nodiscard]] inline std::string print(const Stats::Loadout &stats, const Stats::Team &team, Step) const {
			auto elem = eval(stats, team);
			if (elem.has_value()) {
				return fmt::format("{}", Utils::Stringify(elem.value()));
			}
			return "None";
		}

		[[nodiscard]] inline Utils::JankyOptional<Misc::Element> eval(const Stats::Loadout &stats, const Stats::Team &team) const {
			auto characterInfusion = val.eval(stats, team);
			if (characterInfusion.has_value()) return characterInfusion;

			return team.infusion.eval(stats, team);
		}
	};
}// namespace Formula