#pragma once

#include "character/data.hpp"
#include "character/instance.hpp"
#include "format"
#include "formula/base.hpp"
#include "formula/character.hpp"
#include "formula/clamp.hpp"
#include "formula/constant.hpp"
#include "formula/operators.hpp"
#include "modifiers/total/total.hpp"
#include "stats/loadout.hpp"
#include "stats/team.hpp"
#include <algorithm>


namespace Formula {
	template<FormulaLike T>
	struct TeamCharacter : FormulaBase<FormulaType<T>> {
		size_t index = 0;
		T formula;

		using RetType = FormulaType<T>;

		[[nodiscard]] NodeType<RetType> fold(const Context &context, const FoldArgs &args) const {
			const auto &character = context.team.characters.at(index);
			if (!character) return ConstantBase<RetType>{.value = {}};
			auto ret = formula.fold(context.withSource(character->state), args);
			auto type = ret.getType();
			if (type == Formula::Type::constant) {
				return ret;
			}
			return TeamCharacter<decltype(ret)>{
				.index = index,
				.formula = ret,
			};
		}

		[[nodiscard]] std::string print(const Context &context, Step prevStep) const {
			const auto &character = context.team.characters.at(index);
			if (!character) return "";
			auto &stats = character->state;
			return std::format(
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

	template<FormulaLike T>
	struct ActiveCharacter : FormulaBase<FormulaType<T>> {
		T formula;

		using RetType = FormulaType<T>;

		[[nodiscard]] NodeType<RetType> fold(const Context &context, const FoldArgs &args) const {
			const auto &character = context.team.characters.at(context.team.activeCharacterIndex);
			if (!character) return ConstantBase<RetType>{.value = {}};
			return formula.fold(context.withSource(character->state), args);
		}

		[[nodiscard]] std::string print(const Context &context, Step prevStep) const {
			const auto &character = context.team.characters.at(context.team.activeCharacterIndex);
			if (!character) return "";
			auto &stats = character->state;
			return std::format(
				"{} {}",
				stats.stats.data.name,
				formula.print(context.withSource(stats), prevStep)
			);
		}

		[[nodiscard]] RetType eval(const Context &context) const {
			const auto &character = context.team.characters.at(context.team.activeCharacterIndex);
			if (!character) return {};
			return formula.eval(context.withSource(character->state));
		}
	};

	template<FormulaLike T>
	struct PreviousCharacter : FormulaBase<FormulaType<T>> {
		T formula;

		using RetType = FormulaType<T>;

		[[nodiscard]] NodeType<RetType> fold(const Context &context, const FoldArgs &args) const {
			return formula.fold(context.withSource(context.prevSource), args);
		}

		[[nodiscard]] std::string print(const Context &context, Step prevStep) const {
			return std::format(
				"{} {}",
				context.prevSource.stats.data.name,
				formula.print(context.withSource(context.prevSource), prevStep)
			);
		}

		[[nodiscard]] RetType eval(const Context &context) const {
			return formula.eval(context.withSource(context.prevSource));
		}
	};

	template<ArithmeticFormula T>
	struct TeamEvalSum : FormulaBase<FormulaType<T>> {
		T formula;

		using RetType = FormulaType<T>;
		[[nodiscard]] NodeType<RetType> fold(const Context &context, const FoldArgs &args) const {
			using namespace Formula::Operators;
			return (TeamCharacter{.index = 0, .formula = formula}
					+ TeamCharacter{.index = 1, .formula = formula}
					+ TeamCharacter{.index = 2, .formula = formula}
					+ TeamCharacter{.index = 3, .formula = formula})
				.fold(context, args);
		}

		[[nodiscard]] std::string print(const Context &context, Step) {
			return std::format("{}", eval(context));
		}

		[[nodiscard]] RetType eval(const Context &context) {
			const auto &character1 = context.team.characters.at(0);
			const auto &character2 = context.team.characters.at(1);
			const auto &character3 = context.team.characters.at(2);
			const auto &character4 = context.team.characters.at(3);

			RetType ret = 0;
			if (character1) ret += formula.eval(context.withSource(character1->state));
			if (character2) ret += formula.eval(context.withSource(character2->state));
			if (character3) ret += formula.eval(context.withSource(character3->state));
			if (character4) ret += formula.eval(context.withSource(character4->state));

			return ret;
		}
	};

	struct TeamInfusion : FormulaBase<Utils::JankyOptional<Misc::Element>, Type::constant> {
		[[nodiscard]] static std::string print(const Context &context, Step) {
			auto elem = eval(context);
			if (elem.has_value()) {
				return std::format("{}", Utils::Stringify(elem.value()));
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

	struct TeamMoonsignLevel : FormulaBase<int32_t> {
		[[nodiscard]] static IntNode fold(const Context &context, const FoldArgs &args) {
			using namespace Formula::Operators;
			return (TeamCharacter{.index = 0, .formula = CharacterMoonsignLevel{}}
					+ TeamCharacter{.index = 1, .formula = CharacterMoonsignLevel{}}
					+ TeamCharacter{.index = 2, .formula = CharacterMoonsignLevel{}}
					+ TeamCharacter{.index = 3, .formula = CharacterMoonsignLevel{}})
				.fold(context, args);
		}

		[[nodiscard]] static std::string print(const Context &context, Step) {
			return std::format("Team Moonsign Level {}", eval(context));
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

	struct TeamCharacterCount : FormulaBase<int32_t, Type::constant> {
		[[nodiscard]] static std::string print(const Context &context, Step) {
			return std::format("Team character count {}", eval(context));
		}

		[[nodiscard]] static int32_t eval(const Context &context) {
			int32_t ret = 0;
			for (const auto &character: context.team.characters) {
				if (character) ret++;
			}

			return ret;
		}
	};

	struct CharacterTeamInfusion : FormulaBase<Utils::JankyOptional<Misc::Element>, Type::constant> {
		Formula::ElementNode val;

		[[nodiscard]] std::string print(const Context &context, Step) const {
			auto elem = eval(context);
			if (elem.has_value()) {
				return std::format("{}", Utils::Stringify(elem.value()));
			}
			return "None";
		}

		[[nodiscard]] Utils::JankyOptional<Misc::Element> eval(const Context &context) const {
			auto characterInfusion = val.eval(context);
			if (characterInfusion.has_value()) return characterInfusion;

			return context.team.infusion.eval(context);
		}
	};

	struct NonMoonsignCharacterBuff : FormulaBase<float> {
		[[nodiscard]] static FloatNode fold(const Formula::Context &context, const FoldArgs &args) {
			using namespace Formula::Operators;

			if (context.source.stats.sheet.moonsignLevel.eval(context) >= 1) {
				return Formula::Constant{.value = 0.0f}.fold(context, args);
			}

			Formula::NodeType<float> totalBuff = Formula::Constant{.value = 0.0f};

			switch (context.source.stats.base.element) {
				case Misc::Element::pyro:
				case Misc::Element::electro:
				case Misc::Element::cryo:
					totalBuff = totalBuff + ((Modifiers::total().atk / 100.f) * 0.009f);
					break;
				case Misc::Element::hydro:
					totalBuff = totalBuff + ((Modifiers::total().hp / 1000.f) * 0.0006f);
					break;
				case Misc::Element::geo:
					totalBuff = totalBuff + ((Modifiers::total().def / 100.f) * 0.01f);
					break;
				case Misc::Element::anemo:
				case Misc::Element::dendro:
					totalBuff = totalBuff + ((Modifiers::total().em / 100.f) * 0.0225f);
					break;
				default:
					break;
			}

			return totalBuff.fold(context, args);
		}

		[[nodiscard]] static std::string print(const Formula::Context &context, Step) {
			return std::format("Non-Moonsign Character Buff: {}", Percentage({}, eval(context), true));
		}

		[[nodiscard]] static float eval(const Formula::Context &context) {
			if (CharacterMoonsignLevel{}.eval(context) >= 1) {
				return 0.0f;
			}

			float totalBuff = 0.0f;

			switch (context.source.stats.base.element) {
				case Misc::Element::pyro:
				case Misc::Element::electro:
				case Misc::Element::cryo:
					totalBuff += 0.009f * (Modifiers::total().atk.eval(context) / 100.f);
					break;
				case Misc::Element::hydro:
					totalBuff += 0.0006f * (Modifiers::total().hp.eval(context) / 1000.f);
					break;
				case Misc::Element::geo:
					totalBuff += 0.01f * (Modifiers::total().def.eval(context) / 100.f);
					break;
				case Misc::Element::anemo:
				case Misc::Element::dendro:
					totalBuff += 0.0225f * (Modifiers::total().em.eval(context) / 100.f);
					break;
				default:
					break;
			}

			return totalBuff;
		}
	};

	struct NonMoonsignCharacterTeamBuff : FormulaBase<float> {
		[[nodiscard]] static FloatNode fold(const Context &context, const FoldArgs &args) {
			using namespace Formula::Operators;
			auto totalBuff = TeamCharacter{.index = 0, .formula = NonMoonsignCharacterBuff{}}
						   + TeamCharacter{.index = 1, .formula = NonMoonsignCharacterBuff{}}
						   + TeamCharacter{.index = 2, .formula = NonMoonsignCharacterBuff{}}
						   + TeamCharacter{.index = 3, .formula = NonMoonsignCharacterBuff{}};

			Formula::Min ret{
				.val1 = totalBuff,
				.val2 = Constant{.value = 0.36f},
			};
			return ret.fold(context, args);
		}

		[[nodiscard]] static std::string print(const Context &context, Step) {
			return Percentage("Non-Moonsign Character Team Buff: ", eval(context), true);
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

	struct ActiveCharacterFilter : FormulaBase<float> {
		Formula::FloatNode formula;

		[[nodiscard]] FloatNode fold(const Context &context, const FoldArgs &args) const {
			const auto &activeCharacter = context.team.characters.at(context.team.activeCharacterIndex);
			if (!activeCharacter) return ConstantFlat{.value = 0};
			if (context.source.instanceKey != activeCharacter->instanceKey) return ConstantFlat{.value = 0};
			return formula.fold(context, args);
		}

		[[nodiscard]] std::string print(const Context &context, Step step) const {
			const auto &activeCharacter = context.team.characters.at(context.team.activeCharacterIndex);
			if (!activeCharacter) return "";
			if (context.source.instanceKey != activeCharacter->instanceKey) return "";
			return formula.print(context, step);
		}

		[[nodiscard]] float eval(const Context &context) const {
			const auto &activeCharacter = context.team.characters.at(context.team.activeCharacterIndex);
			if (!activeCharacter) return {};
			if (context.source.instanceKey != activeCharacter->instanceKey) return {};
			return formula.eval(context);
		}
	};
}// namespace Formula