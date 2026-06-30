#pragma once

#include "formula/base.hpp"

#include "formula/elemental.hpp"
#include "modifiers/enemyFactory.hpp"// IWYU pragma: keep
#include "modifiers/total/total.hpp"

namespace Formula {
	struct NodeElement : Formula::FormulaBase<float> {
		Utils::JankyOptional<Misc::Element> element{};
		Utils::JankyOptional<Misc::AttackSource> source{};
		Misc::SkillStat skillStat;

		[[nodiscard]] Formula::FloatNode fold(const Formula::Context &context, const Formula::FoldArgs &args) const {
			return Stats::fromSkillStat(Stats::fromElement(Modifiers::total(), Formula::getElement(source, element, context)), skillStat).fold(context, args);
		}

		[[nodiscard]] std::string print(const Formula::Context &context, Formula::Step) const {
			return Formula::Percentage(
				std::format(
					"{} {}",
					Utils::Stringify(Formula::getElement(source, element, context)),
					Utils::Stringify(skillStat)
				),
				eval(context), Utils::isPercentage(skillStat)
			);
		}

		[[nodiscard]] float eval(const Formula::Context &context) const {
			return Stats::fromSkillStat(Stats::fromElement(Modifiers::total(), Formula::getElement(source, element, context)), skillStat).eval(context);
		}
	};

	struct NodeElementSimple : Formula::FormulaBase<float> {
		Misc::Element element{};
		Misc::SkillStat skillStat;

		[[nodiscard]] Formula::FloatNode fold(const Formula::Context &context, const Formula::FoldArgs &args) const {
			return Stats::fromSkillStat(Stats::fromElement(Modifiers::total(), element), skillStat).fold(context, args);
		}

		[[nodiscard]] std::string print(const Formula::Context &context, Formula::Step) const {
			return Formula::Percentage(
				std::format(
					"{} {}",
					Utils::Stringify(element),
					Utils::Stringify(skillStat)
				),
				eval(context), Utils::isPercentage(skillStat)
			);
		}

		[[nodiscard]] float eval(const Formula::Context &context) const {
			return Stats::fromSkillStat(Stats::fromElement(Modifiers::total(), element), skillStat).eval(context);
		}
	};

	struct NodeSkill : Formula::FormulaBase<float> {
		Utils::JankyOptional<Misc::AttackSource> source{};
		Misc::SkillStat skillStat;

		[[nodiscard]] Formula::FloatNode fold(const Formula::Context &context, const Formula::FoldArgs &args) const {
			if (!source.has_value()) return Formula::Constant{.value = 0.f};
			return Stats::fromAttackSource(Modifiers::total(), source.value(), skillStat).fold(context, args);
		}

		[[nodiscard]] std::string print(const Formula::Context &context, Formula::Step) const {
			return Formula::Percentage(
				std::format(
					"{} {}",
					Utils::Stringify(source),
					Utils::Stringify(skillStat)
				),
				eval(context), Utils::isPercentage(skillStat)
			);
		}

		[[nodiscard]] float eval(const Formula::Context &context) const {
			if (!source.has_value()) return 0.f;
			return Stats::fromAttackSource(Modifiers::total(), source.value(), skillStat).eval(context);
		}
	};

	struct NodeDirectLunar : Formula::FormulaBase<float> {
		Misc::LunarDamageType damageType{};
		Misc::SkillStat skillStat{};

		[[nodiscard]] Formula::FloatNode fold(const Formula::Context &context, const Formula::FoldArgs &args) const {
			return Stats::fromSkillStat(Stats::fromLunarDamageType(Modifiers::total(), damageType), skillStat).fold(context, args);
		}

		[[nodiscard]] std::string print(const Formula::Context &context, Formula::Step) const {
			return Formula::Percentage(std::format("{} {}", Utils::Stringify(damageType), Utils::Stringify(skillStat)), eval(context), Utils::isPercentage(skillStat));
		}

		[[nodiscard]] float eval(const Formula::Context &context) const {
			return Stats::fromSkillStat(Stats::fromLunarDamageType(Modifiers::total(), damageType), skillStat).eval(context);
		}
	};

	struct NodeElementCheck : Formula::FormulaBase<float> {
		Modifiers::SheetMemberIdentifier identifier;
		Modifiers::EnemyMember<Stats::Sheet<Formula::FloatNode>::_EnemySheet> stat;
		Utils::JankyOptional<Misc::AttackSource> source{};
		Utils::JankyOptional<Misc::Element> element{};

		[[nodiscard]] Formula::FloatNode fold(const Formula::Context &context, const Formula::FoldArgs &args) const {
			return stat.resolve(Stats::fromElement(Modifiers::total(), Formula::getElement(source, element, context)).enemy).fold(context, args);
		}

		[[nodiscard]] std::string print(const Formula::Context &context, Formula::Step) const {
			return Formula::Percentage(
				std::format(
					"{} {}",
					Utils::Stringify(Formula::getElement(source, element, context)),
					identifier.getName()
				),
				eval(context), identifier.isPercentage()
			);
		}

		[[nodiscard]] float eval(const Formula::Context &context) const {
			return stat.resolve(Stats::fromElement(Modifiers::total(), Formula::getElement(source, element, context)).enemy).eval(context);
		}
	};

	struct NodeElementCheckSimple : Formula::FormulaBase<float> {
		Modifiers::SheetMemberIdentifier identifier;
		Modifiers::EnemyMember<Stats::Sheet<Formula::FloatNode>::_EnemySheet> stat;
		Misc::Element element{};

		[[nodiscard]] Formula::FloatNode fold(const Formula::Context &context, const Formula::FoldArgs &args) const {
			return stat.resolve(Stats::fromElement(Modifiers::total(), element).enemy).fold(context, args);
		}

		[[nodiscard]] std::string print(const Formula::Context &context, Formula::Step) const {
			return Formula::Percentage(
				std::format(
					"{} {}",
					Utils::Stringify(element),
					identifier.getName()
				),
				eval(context), identifier.isPercentage()
			);
		}

		[[nodiscard]] float eval(const Formula::Context &context) const {
			return stat.resolve(Stats::fromElement(Modifiers::total(), element).enemy).eval(context);
		}
	};

	struct NodeElementCheckDynamic : Formula::FormulaBase<float> {
		Modifiers::SheetMemberIdentifier identifier;
		Modifiers::EnemyMember<Stats::Sheet<Formula::FloatNode>::_EnemySheet> stat;
		Formula::NodeType<Misc::AttackSource> source{};
		Formula::NodeType<Misc::Element> element{};

		[[nodiscard]] Formula::FloatNode fold(const Formula::Context &context, const Formula::FoldArgs &args) const {
			auto sourceFolded = source.fold(context, args);
			auto elementFolded = element.fold(context, args);
			if (elementFolded.getType() == Type::constant && sourceFolded.getType() == Type::constant) {
				return NodeElementCheck{
					.identifier = identifier,
					.stat = stat,
					.source = sourceFolded.getConstantValue(),
					.element = elementFolded.getConstantValue(),
				}
					.fold(context, args);
			}

			return *this;
		}

		[[nodiscard]] std::string print(const Formula::Context &context, Formula::Step) const {
			return Formula::Percentage(
				std::format(
					"{} {}",
					Utils::Stringify(Formula::getElement(source.eval(context), element.eval(context), context)),
					identifier.getName()
				),
				eval(context), identifier.isPercentage()
			);
		}

		[[nodiscard]] float eval(const Formula::Context &context) const {
			return stat.resolve(Stats::fromElement(Modifiers::total(), Formula::getElement(source.eval(context), element.eval(context), context)).enemy).eval(context);
		}
	};

	struct NodeSkillCheck : Formula::FormulaBase<float> {
		Modifiers::SheetMemberIdentifier identifier;
		Modifiers::EnemyMember<Stats::Sheet<Formula::FloatNode>::_EnemySheet> stat;
		Utils::JankyOptional<Misc::AttackSource> source{};

		[[nodiscard]] Formula::FloatNode fold(const Formula::Context &context, const Formula::FoldArgs &args) const {
			if (!source.has_value()) return Formula::Constant{.value = 0.f};
			return stat.resolve(Stats::fromAttackSource(Modifiers::total(), source.value()).enemy).fold(context, args);
		}

		[[nodiscard]] std::string print(const Formula::Context &context, Formula::Step) const {
			return Formula::Percentage(
				std::format(
					"{} {}",
					Utils::Stringify(source),
					identifier.getName()
				),
				eval(context), identifier.isPercentage()
			);
		}

		[[nodiscard]] float eval(const Formula::Context &context) const {
			if (!source.has_value()) return 0.f;
			return stat.resolve(Stats::fromAttackSource(Modifiers::total(), source.value()).enemy).eval(context);
		}
	};

	struct NodeDirectLunarCheck : Formula::FormulaBase<float> {
		Modifiers::SheetMemberIdentifier identifier;
		Modifiers::EnemyMember<Stats::Sheet<Formula::FloatNode>::_EnemySheet> stat;
		Misc::LunarDamageType damageType{};

		[[nodiscard]] Formula::FloatNode fold(const Formula::Context &context, const Formula::FoldArgs &args) const {
			return stat.resolve(Stats::fromLunarDamageType(Modifiers::total(), damageType).enemy).fold(context, args);
		}

		[[nodiscard]] std::string print(const Formula::Context &context, Formula::Step) const {
			return Formula::Percentage(std::format("{} {}", Utils::Stringify(damageType), identifier.getName()), eval(context), identifier.isPercentage());
		}

		[[nodiscard]] float eval(const Formula::Context &context) const {
			return stat.resolve(Stats::fromLunarDamageType(Modifiers::total(), damageType).enemy).eval(context);
		}
	};

	[[nodiscard]] inline Stats::Sheet<FloatNode>::_EnemySheet getElementModifier(Utils::JankyOptional<Misc::AttackSource> source, Utils::JankyOptional<Misc::Element> element) {
		return Modifiers::enemyFactory<Formula::FloatNode, Formula::NodeElementCheck>(        //
			Modifiers::EnemyNameFactory{},                                                    //
			Modifiers::EnemyPointerFactory<Stats::Sheet<Formula::FloatNode>::_EnemySheet>{},  //
			Modifiers::constantEnemyFactory<Utils::JankyOptional<Misc::AttackSource>>(source),//
			Modifiers::constantEnemyFactory<Utils::JankyOptional<Misc::Element>>(element)     //
		);
	}

	[[nodiscard]] inline Stats::Sheet<FloatNode>::_EnemySheet getElementModifierSimple(Misc::Element element) {
		return Modifiers::enemyFactory<Formula::FloatNode, Formula::NodeElementCheckSimple>(//
			Modifiers::EnemyNameFactory{},                                                  //
			Modifiers::EnemyPointerFactory<Stats::Sheet<Formula::FloatNode>::_EnemySheet>{},//
			Modifiers::constantEnemyFactory<Misc::Element>(element)                         //
		);
	}

	[[nodiscard]] inline Stats::Sheet<FloatNode>::_EnemySheet getElementModifierDynamic(NodeType<Misc::AttackSource> source, NodeType<Misc::Element> element) {
		return Modifiers::enemyFactory<Formula::FloatNode, Formula::NodeElementCheckDynamic>(//
			Modifiers::EnemyNameFactory{},                                                   //
			Modifiers::EnemyPointerFactory<Stats::Sheet<Formula::FloatNode>::_EnemySheet>{}, //
			Modifiers::constantEnemyFactory<NodeType<Misc::AttackSource>>(source),           //
			Modifiers::constantEnemyFactory<NodeType<Misc::Element>>(element)                //
		);
	}

	[[nodiscard]] inline Stats::Sheet<FloatNode>::_EnemySheet getSourceModifier(Utils::JankyOptional<Misc::AttackSource> source) {
		return Modifiers::enemyFactory<Formula::FloatNode, Formula::NodeSkillCheck>(         //
			Modifiers::EnemyNameFactory{},                                                   //
			Modifiers::EnemyPointerFactory<Stats::Sheet<Formula::FloatNode>::_EnemySheet>{}, //
			Modifiers::constantEnemyFactory<Utils::JankyOptional<Misc::AttackSource>>(source)//
		);
	}

	[[nodiscard]] inline Stats::Sheet<FloatNode>::_EnemySheet getLunarDamageModifier(Misc::LunarDamageType type) {
		return Modifiers::enemyFactory<Formula::FloatNode, Formula::NodeDirectLunarCheck>(  //
			Modifiers::EnemyNameFactory{},                                                  //
			Modifiers::EnemyPointerFactory<Stats::Sheet<Formula::FloatNode>::_EnemySheet>{},//
			Modifiers::constantEnemyFactory<Misc::LunarDamageType>(type)                    //
		);
	}
}// namespace Formula