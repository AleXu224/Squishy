#pragma once

#include "character/data.hpp"
#include "character/instance.hpp"
#include "fmt/core.h"
#include "formula/compiled/teamCharacter.hpp"
#include "formula/constant.hpp"
#include "formula/operators.hpp"
#include "intermediary.hpp"
#include "stats/loadout.hpp"
#include "stats/team.hpp"
#include <algorithm>


namespace Formula {
	template<FloatFormula T>
	struct TeamCharacter {
		size_t index = 0;
		T formula;

		[[nodiscard]] Compiled::FloatNode compile(const Context &context) const {
			const auto &character = context.team.characters.at(index);
			if (!character) return Compiled::ConstantFloat{.value = 0.f};
			return formula.compile(context.withSource(character->state));
		}

		[[nodiscard]] std::string print(const Context &context, Step prevStep) const {
			const auto &character = context.team.characters.at(index);
			if (!character) return "";
			auto &stats = character->state;
			return fmt::format(
				"{} {}",
				stats.stats.data.name,
				formula.print(context.withSource(stats), prevStep)
			);
		}

		[[nodiscard]] float eval(const Context &context) const {
			const auto &character = context.team.characters.at(index);
			if (!character) return 0.f;
			return formula.eval(context.withSource(character->state));
		}
	};

	struct TeamInfusion {
		[[nodiscard]] static Compiled::ElementNode compile(const Context &context) {
			return Compiled::ConstantElement{.value = eval(context)};
		}

		[[nodiscard]] static std::string print(const Context &context, Step) {
			auto elem = eval(context);
			if (elem.has_value()) {
				return fmt::format("{}", Utils::Stringify(elem.value()));
			}
			return "None";
		}

		[[nodiscard]] static Utils::JankyOptional<Misc::Element> eval(const Context &context) {
			const auto &character1 = context.team.characters.at(0);
			const auto &character2 = context.team.characters.at(1);
			const auto &character3 = context.team.characters.at(2);
			const auto &character4 = context.team.characters.at(3);
			auto getInfusion = [&](Character::Instance *character) -> Utils::JankyOptional<Misc::Element> {
				if (character && character->state.stats.sheet.teamInfusion.hasValue()) return character->state.stats.sheet.teamInfusion.eval(context);
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

	struct TeamCharacterCount {
		[[nodiscard]] Compiled::IntNode compile(const Context &context) const {
			return Compiled::ConstantInt{.value = eval(context)};
		}

		[[nodiscard]] std::string print(const Context &context, Step) const {
			return fmt::format("Team character count {}", eval(context));
		}

		[[nodiscard]] int32_t eval(const Context &context) const {
			uint32_t ret = 0;
			for (const auto &character: context.team.characters) {
				if (character) ret++;
			}

			return ret;
		}
	};

	struct CharacterTeamInfusion {
		Formula::ElementNode val;

		[[nodiscard]] Compiled::ElementNode compile(const Context &context) const {
			return Compiled::ConstantElement{.value = eval(context)};
		}

		[[nodiscard]] std::string print(const Context &context, Step) const {
			auto elem = eval(context);
			if (elem.has_value()) {
				return fmt::format("{}", Utils::Stringify(elem.value()));
			}
			return "None";
		}

		[[nodiscard]] Utils::JankyOptional<Misc::Element> eval(const Context &context) const {
			auto characterInfusion = val.eval(context);
			if (characterInfusion.has_value()) return characterInfusion;

			return context.team.infusion.eval(context);
		}
	};

	struct LunarDmgDistribution {
		Formula::FloatNode formula;

		[[nodiscard]] Compiled::FloatNode compile(const Context &context) const {
			return Compiled::LunarDmgDistribution{
				.char1 = TeamCharacter{.index = 0, .formula = formula}.compile(context),
				.char2 = TeamCharacter{.index = 1, .formula = formula}.compile(context),
				.char3 = TeamCharacter{.index = 2, .formula = formula}.compile(context),
				.char4 = TeamCharacter{.index = 3, .formula = formula}.compile(context),
			}
				.wrap();
		}

		[[nodiscard]] std::string print(const Context &context, Step step) const {
			std::array values{
				std::pair{
					TeamCharacter{.index = 0, .formula = formula},
					TeamCharacter{.index = 0, .formula = formula}.eval(context),
				},
				std::pair{
					TeamCharacter{.index = 1, .formula = formula},
					TeamCharacter{.index = 1, .formula = formula}.eval(context),
				},
				std::pair{
					TeamCharacter{.index = 2, .formula = formula},
					TeamCharacter{.index = 2, .formula = formula}.eval(context),
				},
				std::pair{
					TeamCharacter{.index = 3, .formula = formula},
					TeamCharacter{.index = 3, .formula = formula}.eval(context),
				},
			};

			std::sort(values.begin(), values.end(), [](auto &&val1, auto &&val2) {
				return val1.second > val2.second;
			});

			using namespace Formula::Operators;

			return (values.at(0).first
					+ values.at(1).first * Constant(0.5f)
					+ values.at(2).first * Constant(1.f / 12.f)
					+ values.at(3).first * Constant(1.f / 12.f))
				.print(context, step);
		}

		[[nodiscard]] float eval(const Context &context) const {
			std::array<float, 4> values{
				TeamCharacter{.index = 0, .formula = formula}.eval(context),
				TeamCharacter{.index = 1, .formula = formula}.eval(context),
				TeamCharacter{.index = 2, .formula = formula}.eval(context),
				TeamCharacter{.index = 3, .formula = formula}.eval(context),
			};

			std::sort(values.begin(), values.end(), std::greater<float>());

			return values.at(0) + values.at(1) * 0.5f + (values.at(2) + values.at(3)) * (1.f / 6.f);
		}
	};
}// namespace Formula