#pragma once

#include "formula/base.hpp"

#include "formula/attribute.hpp"
#include "modifiers/enemyFactory.hpp"// IWYU pragma: keep
#include "modifiers/total/total.hpp"

namespace Formula {
	struct NodeAttribute : Formula::FormulaBase<float> {
		Utils::JankyOptional<Misc::Attribute> attribute{};
		Misc::SkillStat skillStat;

		[[nodiscard]] Formula::FloatNode fold(const Formula::Context &context, const Formula::FoldArgs &args) const {
			return Stats::fromSkillStat(Stats::fromAttribute(Modifiers::combat(), Formula::getAttribute(attribute, context)), skillStat).fold(context, args);
		}

		[[nodiscard]] std::string print(const Formula::Context &context, Formula::Step) const {
			return Formula::Percentage(
				std::format(
					"{} {}",
					Utils::Stringify(Formula::getAttribute(attribute, context)),
					Utils::Stringify(skillStat)
				),
				eval(context), Utils::isPercentage(skillStat)
			);
		}

		[[nodiscard]] float eval(const Formula::Context &context) const {
			return Stats::fromSkillStat(Stats::fromAttribute(Modifiers::combat(), Formula::getAttribute(attribute, context)), skillStat).eval(context);
		}
	};

	struct NodeSkill : Formula::FormulaBase<float> {
		Utils::JankyOptional<Misc::AttackSource> source{};
		Misc::SkillStat skillStat;

		[[nodiscard]] Formula::FloatNode fold(const Formula::Context &context, const Formula::FoldArgs &args) const {
			if (!source.has_value()) return Formula::Constant{.value = 0.f};
			return Stats::fromAttackSource(Modifiers::combat(), source.value(), skillStat).fold(context, args);
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
			return Stats::fromAttackSource(Modifiers::combat(), source.value(), skillStat).eval(context);
		}
	};

	struct NodeAttributeCheck : Formula::FormulaBase<float> {
		Modifiers::SheetMemberIdentifier identifier;
		Modifiers::EnemyMember<Stats::Sheet<Formula::FloatNode>::_EnemySheet> stat;
		Utils::JankyOptional<Misc::Attribute> attribute{};

		[[nodiscard]] Formula::FloatNode fold(const Formula::Context &context, const Formula::FoldArgs &args) const {
			return stat.resolve(Stats::fromAttribute(Modifiers::combat(), Formula::getAttribute(attribute, context)).enemy).fold(context, args);
		}

		[[nodiscard]] std::string print(const Formula::Context &context, Formula::Step) const {
			return Formula::Percentage(
				std::format(
					"{} {}",
					Utils::Stringify(Formula::getAttribute(attribute, context)),
					identifier.getName()
				),
				eval(context), identifier.isPercentage()
			);
		}

		[[nodiscard]] float eval(const Formula::Context &context) const {
			return stat.resolve(Stats::fromAttribute(Modifiers::combat(), Formula::getAttribute(attribute, context)).enemy).eval(context);
		}
	};

	struct NodeAttributeCheckDynamic : Formula::FormulaBase<float> {
		Modifiers::SheetMemberIdentifier identifier;
		Modifiers::EnemyMember<Stats::Sheet<Formula::FloatNode>::_EnemySheet> stat;
		Formula::NodeType<Misc::Attribute> attribute{};

		[[nodiscard]] Formula::FloatNode fold(const Formula::Context &context, const Formula::FoldArgs &args) const {
			auto attributeFolded = attribute.fold(context, args);
			if (attributeFolded.getType() == Type::constant) {
				return NodeAttributeCheck{
					.identifier = identifier,
					.stat = stat,
					.attribute = attributeFolded.getConstantValue(),
				}
					.fold(context, args);
			}

			return *this;
		}

		[[nodiscard]] std::string print(const Formula::Context &context, Formula::Step) const {
			return Formula::Percentage(
				std::format(
					"{} {}",
					Utils::Stringify(Formula::getAttribute(attribute.eval(context), context)),
					identifier.getName()
				),
				eval(context), identifier.isPercentage()
			);
		}

		[[nodiscard]] float eval(const Formula::Context &context) const {
			return stat.resolve(Stats::fromAttribute(Modifiers::combat(), Formula::getAttribute(attribute.eval(context), context)).enemy).eval(context);
		}
	};

	struct NodeSkillCheck : Formula::FormulaBase<float> {
		Modifiers::SheetMemberIdentifier identifier;
		Modifiers::EnemyMember<Stats::Sheet<Formula::FloatNode>::_EnemySheet> stat;
		Utils::JankyOptional<Misc::AttackSource> source{};

		[[nodiscard]] Formula::FloatNode fold(const Formula::Context &context, const Formula::FoldArgs &args) const {
			if (!source.has_value()) return Formula::Constant{.value = 0.f};
			return stat.resolve(Stats::fromAttackSource(Modifiers::combat(), source.value()).enemy).fold(context, args);
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
			return stat.resolve(Stats::fromAttackSource(Modifiers::combat(), source.value()).enemy).eval(context);
		}
	};

	[[nodiscard]] inline Stats::Sheet<FloatNode>::_EnemySheet getAttributeModifier(Utils::JankyOptional<Misc::Attribute> attribute) {
		return Modifiers::enemyFactory<Formula::FloatNode, Formula::NodeAttributeCheck>(     //
			Modifiers::EnemyNameFactory{},                                                   //
			Modifiers::EnemyPointerFactory<Stats::Sheet<Formula::FloatNode>::_EnemySheet>{}, //
			Modifiers::constantEnemyFactory<Utils::JankyOptional<Misc::Attribute>>(attribute)//
		);
	}

	[[nodiscard]] inline Stats::Sheet<FloatNode>::_EnemySheet getAttributeModifierDynamic(NodeType<Misc::Attribute> attribute) {
		return Modifiers::enemyFactory<Formula::FloatNode, Formula::NodeAttributeCheckDynamic>(//
			Modifiers::EnemyNameFactory{},                                                     //
			Modifiers::EnemyPointerFactory<Stats::Sheet<Formula::FloatNode>::_EnemySheet>{},   //
			Modifiers::constantEnemyFactory<NodeType<Misc::Attribute>>(attribute)              //
		);
	}

	[[nodiscard]] inline Stats::Sheet<FloatNode>::_EnemySheet getSourceModifier(Utils::JankyOptional<Misc::AttackSource> source) {
		return Modifiers::enemyFactory<Formula::FloatNode, Formula::NodeSkillCheck>(         //
			Modifiers::EnemyNameFactory{},                                                   //
			Modifiers::EnemyPointerFactory<Stats::Sheet<Formula::FloatNode>::_EnemySheet>{}, //
			Modifiers::constantEnemyFactory<Utils::JankyOptional<Misc::AttackSource>>(source)//
		);
	}
}// namespace Formula