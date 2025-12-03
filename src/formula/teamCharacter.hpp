#pragma once

#include "character/data.hpp"
#include "character/instance.hpp"
#include "fmt/core.h"
#include "formula/character.hpp"
#include "formula/clamp.hpp"
#include "formula/compiled/teamCharacter.hpp"
#include "formula/constant.hpp"
#include "formula/operators.hpp"
#include "intermediary.hpp"
#include "modifiers/total/total.hpp"
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
		[[nodiscard]] static Compiled::IntNode compile(const Context &context) {
			return Compiled::ConstantInt{.value = eval(context)};
		}

		[[nodiscard]] static std::string print(const Context &context, Step) {
			return fmt::format("Team character count {}", eval(context));
		}

		[[nodiscard]] static int32_t eval(const Context &context) {
			int32_t ret = 0;
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

	struct NonMoonsignCharacterBuff {
		[[nodiscard]] static Compiled::FloatNode compile(const Formula::Context &context) {
			using namespace Formula::Operators;

			if (context.source.stats.sheet.moonsignLevel.eval(context) < 1) {
				return Formula::Constant{0.0f}.compile(context);
			}

			Formula::NodeType<float> totalBuff = Formula::Constant{0.0f};

			switch (context.source.stats.base.element) {
				case Misc::Element::pyro:
				case Misc::Element::electro:
				case Misc::Element::cryo:
					totalBuff = totalBuff + (Modifiers::total().atk * Formula::Constant{0.009f});
					break;
				case Misc::Element::hydro:
					totalBuff = totalBuff + (Modifiers::total().hp * Formula::Constant{0.0006f});
					break;
				case Misc::Element::geo:
					totalBuff = totalBuff + (Modifiers::total().def * Formula::Constant{0.01f});
					break;
				case Misc::Element::anemo:
				case Misc::Element::dendro:
					totalBuff = totalBuff + (Modifiers::total().em * Formula::Constant{0.0225f});
					break;
				default:
					break;
			}

			return totalBuff.compile(context);
		}

		[[nodiscard]] static std::string print(const Formula::Context &context, Step) {
			return fmt::format("Non-Moonsign Character Buff: {}", eval(context));
		}

		[[nodiscard]] static float eval(const Formula::Context &context) {
			if (CharacterMoonsignLevel{}.eval(context) < 1) {
				return 0.0f;
			}

			float totalBuff = 0.0f;

			switch (context.source.stats.base.element) {
				case Misc::Element::pyro:
				case Misc::Element::electro:
				case Misc::Element::cryo:
					totalBuff += 0.009f * Modifiers::total().atk.eval(context);
					break;
				case Misc::Element::hydro:
					totalBuff += 0.0006f * Modifiers::total().hp.eval(context);
					break;
				case Misc::Element::geo:
					totalBuff += 0.01f * Modifiers::total().def.eval(context);
					break;
				case Misc::Element::anemo:
				case Misc::Element::dendro:
					totalBuff += 0.0225f * Modifiers::total().em.eval(context);
					break;
				default:
					break;
			}

			return totalBuff;
		}
	};

	struct NonMoonsignCharacterTeamBuff {
		[[nodiscard]] static Compiled::FloatNode compile(const Context &context) {
			using namespace Formula::Operators;
			auto totalBuff = TeamCharacter{.index = 0, .formula = NonMoonsignCharacterBuff{}}
						   + TeamCharacter{.index = 1, .formula = NonMoonsignCharacterBuff{}}
						   + TeamCharacter{.index = 2, .formula = NonMoonsignCharacterBuff{}}
						   + TeamCharacter{.index = 3, .formula = NonMoonsignCharacterBuff{}};

			Formula::Min ret{
				.val1 = totalBuff,
				.val2 = 0.36f,
			};
			return ret.compile(context);
		}

		[[nodiscard]] static std::string print(const Context &context, Step) {
			return fmt::format("Non-Moonsign Character Team Buff: {}", eval(context));
		}

		[[nodiscard]] static float eval(const Context &context) {
			float totalBuff = 0.f;
			static constexpr auto formula = NonMoonsignCharacterBuff{};

			for (const auto &character: context.team.characters) {
				if (!character) continue;
				totalBuff += formula.eval(context.withSource(character->state));
			}

			return std::min(totalBuff, 0.36f);
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

			return values.at(0)
				 + (values.at(1) * 0.5f)
				 + ((values.at(2) + values.at(3)) * (1.f / 12.f));
		}
	};
}// namespace Formula