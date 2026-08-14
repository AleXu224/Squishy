#pragma once

#include "UI/elementToColor.hpp"
#include "formula/base.hpp"

#include "formula/elemental.hpp"
#include "modifiers/enemyFactory.hpp"// IWYU pragma: keep
#include "modifiers/total/total.hpp"

namespace Formula {
	struct NodeElement : Formula::FormulaBase<float> {
		Utils::JankyOptional<Misc::Element> element{};
		Utils::JankyOptional<Misc::AttackSource> source{};
		Misc::SkillStat skillStat;

		auto getFormula(const Formula::Context &context) const {
			return Stats::fromSkillStat(Stats::fromElement(Modifiers::total(), Formula::getElement(source, element, context)), skillStat);
		}

		[[nodiscard]] Formula::FloatNode fold(const Formula::Context &context, const Formula::FoldArgs &args) const {
			return getFormula(context).fold(context, args);
		}

		void print(Descriptor &descriptor, const Formula::Context &context, Formula::Step) const {
			auto element = Formula::getElement(source, this->element, context);
			descriptor.pushColor(Utils::elementToColor(element));
			getFormula(context).print(descriptor, context, Step::none);
		}

		[[nodiscard]] float eval(const Formula::Context &context) const {
			return getFormula(context).eval(context);
		}
	};

	struct NodeElementSimple : Formula::FormulaBase<float> {
		Misc::Element element{};
		Misc::SkillStat skillStat;

		auto getFormula() const {
			return Stats::fromSkillStat(Stats::fromElement(Modifiers::total(), element), skillStat);
		}

		[[nodiscard]] Formula::FloatNode fold(const Formula::Context &context, const Formula::FoldArgs &args) const {
			return getFormula().fold(context, args);
		}

		void print(Descriptor &descriptor, const Formula::Context &context, Formula::Step) const {
			descriptor.pushColor(Utils::elementToColor(element));
			getFormula().print(descriptor, context, Step::none);
		}

		[[nodiscard]] float eval(const Formula::Context &context) const {
			return getFormula().eval(context);
		}
	};

	struct NodeElementSimpleOptional : Formula::FormulaBase<float> {
		Utils::JankyOptional<Misc::Element> element{};
		Misc::SkillStat skillStat;

		auto getFormula(const Formula::Context &context) const {
			return Stats::fromSkillStat(Stats::fromElement(Modifiers::total(), element.value_or(context.source.stats.base.element)), skillStat);
		}

		[[nodiscard]] Formula::FloatNode fold(const Formula::Context &context, const Formula::FoldArgs &args) const {
			return getFormula(context).fold(context, args);
		}

		void print(Descriptor &descriptor, const Formula::Context &context, Formula::Step) const {
			descriptor.pushColor(Utils::elementToColor(element.value_or(context.source.stats.base.element)));
			getFormula(context).print(descriptor, context, Step::none);
		}

		[[nodiscard]] float eval(const Formula::Context &context) const {
			return getFormula(context).eval(context);
		}
	};

	struct NodeSkill : Formula::FormulaBase<float> {
		Utils::JankyOptional<Misc::AttackSource> source{};
		Misc::SkillStat skillStat;

		FloatNode getFormula() const {
			if (!source.has_value()) return Formula::Constant{.value = 0.f};
			return Stats::fromAttackSource(Modifiers::total(), source.value_or(Misc::AttackSource::normal), skillStat);
		}

		[[nodiscard]] Formula::FloatNode fold(const Formula::Context &context, const Formula::FoldArgs &args) const {
			return getFormula().fold(context, args);
		}

		void print(Descriptor &descriptor, const Formula::Context &context, Formula::Step) const {
			getFormula().print(descriptor, context, Step::none);
		}

		[[nodiscard]] float eval(const Formula::Context &context) const {
			return getFormula().eval(context);
		}
	};

	struct NodeDirectLunar : Formula::FormulaBase<float> {
		Misc::LunarDamageType damageType{};
		Misc::SkillStat skillStat{};

		auto getFormula(const Formula::Context &context) const {
			return Stats::fromSkillStat(Stats::fromLunarDamageType(Modifiers::total(), damageType), skillStat);
		}

		[[nodiscard]] Formula::FloatNode fold(const Formula::Context &context, const Formula::FoldArgs &args) const {
			return getFormula(context).fold(context, args);
		}

		void print(Descriptor &descriptor, const Formula::Context &context, Formula::Step) const {
			descriptor.pushColor(Utils::elementToColor(Misc::lunarDamageTypeToElement(damageType)));
			getFormula(context).print(descriptor, context, Step::none);
		}

		[[nodiscard]] float eval(const Formula::Context &context) const {
			return getFormula(context).eval(context);
		}
	};

	struct NodeDirectStellar : Formula::FormulaBase<float> {
		Utils::JankyOptional<Misc::Element> element{};
		Misc::StellarDamageType damageType{};
		Misc::SkillStat skillStat{};

		auto getFormula(const Formula::Context &context) const {
			return Stats::fromSkillStat(Stats::fromStellarDamageType(Modifiers::total(), damageType), skillStat);
		}

		[[nodiscard]] Formula::FloatNode fold(const Formula::Context &context, const Formula::FoldArgs &args) const {
			return getFormula(context).fold(context, args);
		}

		void print(Descriptor &descriptor, const Formula::Context &context, Formula::Step) const {
			descriptor.pushColor(Utils::elementToColor(element.value_or(context.source.stats.base.element)));
			getFormula(context).print(descriptor, context, Step::none);
		}

		[[nodiscard]] float eval(const Formula::Context &context) const {
			return getFormula(context).eval(context);
		}
	};

	struct NodeElementCheck : Formula::FormulaBase<float> {
		Modifiers::SheetMemberIdentifier identifier;
		Modifiers::EnemyMember<Stats::Sheet<Formula::FloatNode>::_EnemySheet> stat;
		Utils::JankyOptional<Misc::AttackSource> source{};
		Utils::JankyOptional<Misc::Element> element{};

		auto getFormula(const Formula::Context &context) const {
			return stat.resolve(Stats::fromElement(Modifiers::total(), Formula::getElement(source, element, context)).enemy);
		}

		[[nodiscard]] Formula::FloatNode fold(const Formula::Context &context, const Formula::FoldArgs &args) const {
			return getFormula(context).fold(context, args);
		}

		void print(Descriptor &descriptor, const Formula::Context &context, Formula::Step) const {
			auto element = Formula::getElement(source, this->element, context);
			descriptor.pushColor(Utils::elementToColor(element));
			getFormula(context).print(descriptor, context, Step::none);
		}

		[[nodiscard]] float eval(const Formula::Context &context) const {
			return getFormula(context).eval(context);
		}
	};

	struct NodeElementCheckSimple : Formula::FormulaBase<float> {
		Modifiers::SheetMemberIdentifier identifier;
		Modifiers::EnemyMember<Stats::Sheet<Formula::FloatNode>::_EnemySheet> stat;
		Misc::Element element{};

		auto getFormula() const {
			return stat.resolve(Stats::fromElement(Modifiers::total(), element).enemy);
		}

		[[nodiscard]] Formula::FloatNode fold(const Formula::Context &context, const Formula::FoldArgs &args) const {
			return getFormula().fold(context, args);
		}

		void print(Descriptor &descriptor, const Formula::Context &context, Formula::Step) const {
			descriptor.pushColor(Utils::elementToColor(element));
			getFormula().print(descriptor, context, Step::none);
		}

		[[nodiscard]] float eval(const Formula::Context &context) const {
			return getFormula().eval(context);
		}
	};

	struct NodeElementCheckSimpleOptional : Formula::FormulaBase<float> {
		Modifiers::SheetMemberIdentifier identifier;
		Modifiers::EnemyMember<Stats::Sheet<Formula::FloatNode>::_EnemySheet> stat;
		Utils::JankyOptional<Misc::Element> element{};

		auto getFormula(const Formula::Context &context) const {
			return stat.resolve(Stats::fromElement(Modifiers::total(), element.value_or(context.source.stats.base.element)).enemy);
		}

		[[nodiscard]] Formula::FloatNode fold(const Formula::Context &context, const Formula::FoldArgs &args) const {
			return getFormula(context).fold(context, args);
		}

		void print(Descriptor &descriptor, const Formula::Context &context, Formula::Step) const {
			descriptor.pushColor(Utils::elementToColor(element.value_or(context.source.stats.base.element)));
			getFormula(context).print(descriptor, context, Step::none);
		}

		[[nodiscard]] float eval(const Formula::Context &context) const {
			return getFormula(context).eval(context);
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

		void print(Descriptor &descriptor, const Formula::Context &context, Formula::Step) const {
			descriptor.pushColor(Utils::elementToColor(Formula::getElement(source.eval(context), this->element.eval(context), context)));
			stat.resolve(Stats::fromElement(Modifiers::total(), Formula::getElement(source.eval(context), element.eval(context), context)).enemy).print(descriptor, context, Step::none);
		}

		[[nodiscard]] float eval(const Formula::Context &context) const {
			return stat.resolve(Stats::fromElement(Modifiers::total(), Formula::getElement(source.eval(context), element.eval(context), context)).enemy).eval(context);
		}
	};

	struct NodeSkillCheck : Formula::FormulaBase<float> {
		Modifiers::SheetMemberIdentifier identifier;
		Modifiers::EnemyMember<Stats::Sheet<Formula::FloatNode>::_EnemySheet> stat;
		Utils::JankyOptional<Misc::AttackSource> source{};

		FloatNode getFormula(const Formula::Context &context) const {
			if (!source.has_value()) return Formula::Constant{.value = 0.f};
			return stat.resolve(Stats::fromAttackSource(Modifiers::total(), source.value()).enemy);
		}

		[[nodiscard]] Formula::FloatNode fold(const Formula::Context &context, const Formula::FoldArgs &args) const {
			return getFormula(context).fold(context, args);
		}

		void print(Descriptor &descriptor, const Formula::Context &context, Formula::Step) const {
			// descriptor.add(squi::RichText::Style{.font = squi::FontStore::defaultFontBold, .text = std::format("{} {}", Utils::Stringify(source), identifier.getName())});
			// descriptor.add(Formula::Percentage({}, eval(context), identifier.isPercentage()));
			getFormula(context).print(descriptor, context, Step::none);
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

		auto getFormula(const Formula::Context &context) const {
			return stat.resolve(Stats::fromLunarDamageType(Modifiers::total(), damageType).enemy);
		}

		[[nodiscard]] Formula::FloatNode fold(const Formula::Context &context, const Formula::FoldArgs &args) const {
			return getFormula(context).fold(context, args);
		}

		void print(Descriptor &descriptor, const Formula::Context &context, Formula::Step) const {
			descriptor.pushColor(Utils::elementToColor(Misc::lunarDamageTypeToElement(damageType)));
			getFormula(context).print(descriptor, context, Step::none);
		}

		[[nodiscard]] float eval(const Formula::Context &context) const {
			return stat.resolve(Stats::fromLunarDamageType(Modifiers::total(), damageType).enemy).eval(context);
		}
	};

	struct NodeDirectStellarCheck : Formula::FormulaBase<float> {
		Modifiers::SheetMemberIdentifier identifier;
		Modifiers::EnemyMember<Stats::Sheet<Formula::FloatNode>::_EnemySheet> stat;
		Utils::JankyOptional<Misc::Element> element{};
		Misc::StellarDamageType damageType{};

		auto getFormula(const Formula::Context &context) const {
			return stat.resolve(Stats::fromStellarDamageType(Modifiers::total(), damageType).enemy);
		}

		[[nodiscard]] Formula::FloatNode fold(const Formula::Context &context, const Formula::FoldArgs &args) const {
			return getFormula(context).fold(context, args);
		}

		void print(Descriptor &descriptor, const Formula::Context &context, Formula::Step) const {
			descriptor.pushColor(Utils::elementToColor(element.value_or(context.source.stats.base.element)));
			getFormula(context).print(descriptor, context, Step::none);
		}

		[[nodiscard]] float eval(const Formula::Context &context) const {
			return stat.resolve(Stats::fromStellarDamageType(Modifiers::total(), damageType).enemy).eval(context);
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

	[[nodiscard]] inline Stats::Sheet<FloatNode>::_EnemySheet getElementModifierSimpleOptional(Utils::JankyOptional<Misc::Element> element) {
		return Modifiers::enemyFactory<Formula::FloatNode, Formula::NodeElementCheckSimpleOptional>(//
			Modifiers::EnemyNameFactory{},                                                          //
			Modifiers::EnemyPointerFactory<Stats::Sheet<Formula::FloatNode>::_EnemySheet>{},        //
			Modifiers::constantEnemyFactory<Utils::JankyOptional<Misc::Element>>(element)           //
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

	[[nodiscard]] inline Stats::Sheet<FloatNode>::_EnemySheet getStellarDamageModifier(Utils::JankyOptional<Misc::Element> element, Misc::StellarDamageType type) {
		return Modifiers::enemyFactory<Formula::FloatNode, Formula::NodeDirectStellarCheck>(//
			Modifiers::EnemyNameFactory{},                                                  //
			Modifiers::EnemyPointerFactory<Stats::Sheet<Formula::FloatNode>::_EnemySheet>{},//
			Modifiers::constantEnemyFactory<Utils::JankyOptional<Misc::Element>>(element),  //
			Modifiers::constantEnemyFactory<Misc::StellarDamageType>(type)                  //
		);
	}
}// namespace Formula