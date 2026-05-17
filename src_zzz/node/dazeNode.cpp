#include "dazeNode.hpp"
#include "formula/attribute.hpp"
#include "formula/operators.hpp"
#include "misc/attribute.hpp"
#include "misc/skillStat.hpp"
#include "modifiers/total/total.hpp"


namespace Node {
	using namespace Formula::Operators;
	template<Misc::SkillStat skillStat>
	struct _NodeAttribute : Formula::FormulaBase<float> {
		Utils::JankyOptional<Misc::Attribute> attribute{};
		Utils::JankyOptional<Misc::AttackSource> source{};

		[[nodiscard]] Formula::FloatNode fold(const Formula::Context &context, const Formula::FoldArgs &args) const {
			return Stats::fromSkillStat(Stats::fromAttribute(Modifiers::combat(), Formula::getAttribute(source, attribute, context)), skillStat).fold(context, args);
		}

		[[nodiscard]] std::string print(const Formula::Context &context, Formula::Step) const {
			return Formula::Percentage(
				std::format(
					"{} {}",
					Utils::Stringify(Formula::getAttribute(source, attribute, context)),
					Utils::Stringify(skillStat)
				),
				eval(context), Utils::isPercentage(skillStat)
			);
		}

		[[nodiscard]] float eval(const Formula::Context &context) const {
			return Stats::fromSkillStat(Stats::fromAttribute(Modifiers::combat(), Formula::getAttribute(source, attribute, context)), skillStat).eval(context);
		}
	};

	template<Misc::SkillStat skillStat>
	struct _NodeSkill : Formula::FormulaBase<float> {
		Utils::JankyOptional<Misc::AttackSource> source{};

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

	template<Misc::SkillStat skillStat>
	[[nodiscard]] static constexpr auto _getTotal(
		Utils::JankyOptional<Misc::Attribute> attackAttribute,
		const Utils::JankyOptional<Misc::AttackSource> &atkSource,
		const auto &formula
	) {
		auto allStats = Stats::fromSkillStat(Modifiers::combat().all, skillStat);
		auto attributeStats = _NodeAttribute<skillStat>({}, attackAttribute, atkSource);
		auto skillStats = _NodeSkill<skillStat>({}, atkSource);

		return allStats + attributeStats + skillStats + formula;
	}

	Formula::FloatNode Daze::_getFormula(
		const Utils::JankyOptional<Misc::Attribute> &attribute,
		const Utils::JankyOptional<Misc::AttackSource> &source,
		const Formula::FloatNode &formula,
		const Formula::Modifier &modifier
	) {
		auto totalDaze = _getTotal<Misc::SkillStat::daze>(attribute, source, modifier.daze);

		auto dazeRes = 1.f - Modifiers::enemy().dazeRes;
		auto daze = 1.f + (Modifiers::enemy().daze + totalDaze);
		auto dazeTaken = 1.f + Modifiers::enemy().dazeTaken;

		return formula
			 * dazeRes
			 * daze
			 * dazeTaken;
	}

}// namespace Node