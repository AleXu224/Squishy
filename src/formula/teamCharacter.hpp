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

		[[nodiscard]] inline std::string print(const Context &context, Step prevStep) const {
			const auto &character = context.team.characters.at(index);
			if (!character.has_value()) return "";
			auto &stats = character->get().stats;
			return fmt::format(
				"{} {}",
				stats.character.data.name,
				formula.print(context, prevStep)
			);
		}

		[[nodiscard]] inline float eval(const Context &context) const {
			const auto &character = context.team.characters.at(index);
			if (!character.has_value()) return 0.f;
			return formula.eval(context.withSource(character->get().stats));
		}
	};

	struct TeamInfusion {
		[[nodiscard]] static inline std::string print(const Context &context, Step) {
			auto elem = eval(context);
			if (elem.has_value()) {
				return fmt::format("{}", Utils::Stringify(elem.value()));
			}
			return "None";
		}

		[[nodiscard]] static inline Utils::JankyOptional<Misc::Element> eval(const Context &context) {
			const auto &character1 = context.team.characters.at(0);
			const auto &character2 = context.team.characters.at(1);
			const auto &character3 = context.team.characters.at(2);
			const auto &character4 = context.team.characters.at(3);
			auto getInfusion = [&](const std::optional<std::reference_wrapper<Character::Instance>> &character) -> Utils::JankyOptional<Misc::Element> {
				if (character.has_value() && character->get().stats.character.sheet.teamInfusion.hasValue()) return character->get().stats.character.sheet.teamInfusion.eval(context);
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

		[[nodiscard]] inline std::string print(const Context &context, Step) const {
			auto elem = eval(context);
			if (elem.has_value()) {
				return fmt::format("{}", Utils::Stringify(elem.value()));
			}
			return "None";
		}

		[[nodiscard]] inline Utils::JankyOptional<Misc::Element> eval(const Context &context) const {
			auto characterInfusion = val.eval(context);
			if (characterInfusion.has_value()) return characterInfusion;

			return context.team.infusion.eval(context);
		}
	};
}// namespace Formula