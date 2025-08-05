#pragma once

#include "character/data.hpp"
#include "character/instance.hpp"
#include "fmt/core.h"
#include "formula/character.hpp"
#include "formula/compiled/teamCharacter.hpp"
#include "formula/constant.hpp"
#include "formula/operators.hpp"
#include "intermediary.hpp"
#include "stats/loadout.hpp"
#include "stats/team.hpp"
#include <algorithm>


namespace Formula {
	template<FormulaLike T>
	struct TeamCharacter {
		size_t index = 0;
		T formula;

		using RetType = FormulaType<T>;

		[[nodiscard]] Formula::Compiled::NodeType<RetType> compile(const Context &context) const {
			const auto &character = context.team.characters.at(index);
			if (!character) return Compiled::Constant<RetType>{.value = {}};
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

		[[nodiscard]] RetType eval(const Context &context) const {
			const auto &character = context.team.characters.at(index);
			if (!character) return {};
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

	struct TeamMoonsignLevel {
		[[nodiscard]] static Compiled::IntNode compile(const Context &context) {
			using namespace Formula::Operators;
			return (TeamCharacter{.index = 0, .formula = CharacterMoonsignLevel{}}
					+ TeamCharacter{.index = 1, .formula = CharacterMoonsignLevel{}}
					+ TeamCharacter{.index = 2, .formula = CharacterMoonsignLevel{}}
					+ TeamCharacter{.index = 3, .formula = CharacterMoonsignLevel{}})
				.compile(context);
		}

		[[nodiscard]] static std::string print(const Context &context, Step) {
			return fmt::format("Team Moonsign Level {}", eval(context));
		}

		[[nodiscard]] static int32_t eval(const Context &context) {
			const auto &character1 = context.team.characters.at(0);
			const auto &character2 = context.team.characters.at(1);
			const auto &character3 = context.team.characters.at(2);
			const auto &character4 = context.team.characters.at(3);

			int32_t ret = 0;
			if (character1) ret += character1->state.stats.sheet.moonsignLevel.eval(context.withSource(character1->state));
			if (character2) ret += character2->state.stats.sheet.moonsignLevel.eval(context.withSource(character2->state));
			if (character3) ret += character3->state.stats.sheet.moonsignLevel.eval(context.withSource(character3->state));
			if (character4) ret += character4->state.stats.sheet.moonsignLevel.eval(context.withSource(character4->state));

			return ret;
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