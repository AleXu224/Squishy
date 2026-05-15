#include "abloomNode.hpp"
#include "anomaly/list.hpp"
#include "formula/clamp.hpp"
#include "formula/operators.hpp"
#include "formula/teamAgent.hpp"
#include "misc/attribute.hpp"
#include "misc/skillStat.hpp"
#include "modifiers/total/total.hpp"


namespace Node {
	using namespace Formula::Operators;
	struct _NodeAttributeAbloom : Formula::FormulaBase<float> {
		Misc::Attribute attribute;
		Misc::SkillStat skillStat;

		[[nodiscard]] Formula::FloatNode fold(const Formula::Context &context, const Formula::FoldArgs &args) const {
			return Stats::fromSkillStat(Stats::fromAttribute(Modifiers::combat(), attribute), skillStat).fold(context, args);
		}

		[[nodiscard]] std::string print(const Formula::Context &context, Formula::Step) const {
			return Formula::Percentage(
				fmt::format(
					"{} {}",
					Utils::Stringify(attribute),
					Utils::Stringify(skillStat)
				),
				eval(context), Utils::isPercentage(skillStat)
			);
		}

		[[nodiscard]] float eval(const Formula::Context &context) const {
			return Stats::fromSkillStat(Stats::fromAttribute(Modifiers::combat(), attribute), skillStat).eval(context);
		}
	};

	[[nodiscard]] static constexpr auto _getTotalAbloom(
		Misc::Attribute attackAttribute,
		Misc::SkillStat skillStat,
		const auto &formula
	) {
		auto allAnomaly = Stats::fromSkillStat(Modifiers::combat().allAnomaly, skillStat);
		auto abloom = Stats::fromSkillStat(Modifiers::combat().abloom, skillStat);

		return allAnomaly + abloom + formula;
	}

	Formula::FloatNode AbloomNode::_getFormula(
		const Misc::Attribute &attribute,
		float multiplier,
		size_t index,
		const Formula::Modifier &modifier
	) {
		auto totalDMG = _getTotalAbloom(attribute, Misc::SkillStat::DMG, modifier.DMG);
		auto totalAdditiveDMG = _getTotalAbloom(attribute, Misc::SkillStat::additiveDMG, modifier.additiveDMG);
		// auto totalMultiplicativeDMG = _getTotalCustom(attribute, Misc::SkillStat::multiplicativeDMG, modifier.multiplicativeDMG);
		auto totalCritRate = Formula::Clamp({}, _getTotalAbloom(attribute, Misc::SkillStat::critRate, modifier.critRate), 0.f, 1.f);
		auto totalCritDMG = _getTotalAbloom(attribute, Misc::SkillStat::critDMG, modifier.critDMG);

		auto formula = Formula::TeamAgent{
						   .index = index,
						   .formula = Formula::Requires{
							   .requirement = Formula::IsAgentAttribute{.attribute = attribute},
							   .ret = Anomaly::List::fromAttribute(attribute).formulaAbloom,
						   },
					   } * (1.f + totalDMG)
					 + totalAdditiveDMG;
		auto crit = 1.0f + totalCritRate * totalCritDMG;

		return formula
			 * crit;
	}

}// namespace Node