#pragma once

#include "formula/agent.hpp"
#include "formula/enemy.hpp"
#include "formula/operators.hpp"
#include "formula/requirement.hpp"
#include "formula/requires.hpp"
#include "misc/attribute.hpp"


namespace Anomaly {
	using namespace Formula::Operators;
	[[nodiscard]] inline Formula::FloatNode makeAnomalyFormula(const Stats::Sheet<Formula::FloatNode>::_SkillValue &modifier, float baseMultiplier, Misc::Attribute attribute, bool useAnomalyMod = true) {
		auto multiplier = baseMultiplier + modifier.additiveMultiplier + Modifiers::combat().allAnomaly.additiveMultiplier;
		auto baseDmg = Modifiers::combat().atk * (multiplier) + modifier.additiveDMG + Modifiers::combat().allAnomaly.additiveDMG;
		auto dmgMod = Modifiers::combat().fromAttribute(attribute).DMG + Modifiers::combat().all.DMG;
		auto anomalyMod = Formula::Requires{
			.requirement = Formula::ConstantBool{.value = useAnomalyMod},
			.ret = modifier.DMG + Modifiers::combat().allAnomaly.DMG,
		};
		auto resMod = Formula::IfElse{
			.requirement = Formula::ConstantBool{.value = useAnomalyMod},
			.trueVal = Formula::EnemyResMultiplier{.element = attribute},
			.elseVal = Formula::Constant{.value = 1.f},
		};
		auto defMod = Formula::EnemyDefMultiplier{};
		// FIXME: dmg taken
		auto stunMod = Formula::Requires{.requirement = Requirement::enemyStunned, .ret = Modifiers::enemy().stunMod};
		auto apBonus = Modifiers::combat().ap / Formula::ConstantFlat{.value = 100.f};
		auto buffLevelMod = 1.f + (Formula::AgentLevel{} - 1.f) / Formula::ConstantFlat{.value = 59.f};

		return baseDmg
			 * (1.f + dmgMod)
			 * (1.f + anomalyMod)
			 * resMod
			 * defMod
			 * apBonus
			 * buffLevelMod
			 * (1.f + stunMod);
	}
	[[nodiscard]] inline Formula::FloatNode makeDisorderFormula(const Stats::Sheet<Formula::FloatNode>::_SkillValue &modifier, float baseMultiplier, Misc::Attribute attribute) {
		auto multiplier = baseMultiplier + Modifiers::combat().disorder.additiveMultiplier + modifier.additiveMultiplier + Modifiers::combat().allAnomaly.additiveMultiplier;
		auto baseDmg = Modifiers::combat().atk * (multiplier) + modifier.additiveDMG + Modifiers::combat().disorder.additiveDMG;
		auto dmgMod = Modifiers::combat().fromAttribute(attribute).DMG + Modifiers::combat().all.DMG;
		auto anomalyMod = modifier.DMG + Modifiers::combat().disorder.DMG;
		auto resMod = Formula::EnemyResMultiplier{.element = attribute};
		auto defMod = Formula::EnemyDefMultiplier{};
		// FIXME: dmg taken
		auto stunMod = Formula::Requires{.requirement = Requirement::enemyStunned, .ret = Modifiers::enemy().stunMod};
		auto apBonus = Modifiers::combat().ap / Formula::ConstantFlat{.value = 100.f};
		auto buffLevelMod = 1.f + (Formula::AgentLevel{} - 1.f) / Formula::ConstantFlat{.value = 59.f};

		return baseDmg
			 * (1.f + dmgMod)
			 * (1.f + anomalyMod)
			 * resMod
			 * defMod
			 * apBonus
			 * buffLevelMod
			 * (1.f + stunMod);
	}
	[[nodiscard]] extern Formula::FloatNode makeVortexFormula(const Stats::Sheet<Formula::FloatNode>::_SkillValue &modifier, Formula::NodeType<Misc::Attribute> attribute, size_t index);

	[[nodiscard]] inline Stats::Sheet<Formula::FloatNode>::_SkillValue makeVortexModifier(const Stats::Sheet<Formula::FloatNode>::_SkillValue &modifier) {
		auto ret = modifier;
		ret.additiveMultiplier = modifier.additiveMultiplier + Modifiers::combat().vortex.additiveMultiplier;
		return ret;
	}

	struct Anomaly {
		Formula::NodeType<std::string> name;
		float multiplier{};
		Formula::NodeType<Misc::Attribute> attribute{};
		const Stats::Sheet<Formula::FloatNode>::_SkillValue &modifier;
		Formula::FloatNode formula = makeAnomalyFormula(modifier, multiplier, attribute.getConstantValue());
		Formula::FloatNode formulaAbloom = makeAnomalyFormula(modifier, multiplier, attribute.getConstantValue(), false);
		Formula::FloatNode formulaVortex = makeAnomalyFormula(makeVortexModifier(modifier), multiplier, attribute.getConstantValue(), false);
	};

	namespace List {
		const static inline Anomaly burn = Anomaly{
			.name = Formula::ConstantBase<std::string>{.value = "Burn"},
			.multiplier = 0.5f,
			.attribute = Formula::ConstantBase<Misc::Attribute>{.value = Misc::Attribute::fire},
			.modifier = Modifiers::combat().burn,
		};
		const static inline Anomaly shock = Anomaly{
			.name = Formula::ConstantBase<std::string>{.value = "Shock"},
			.multiplier = 1.25f,
			.attribute = Formula::ConstantBase<Misc::Attribute>{.value = Misc::Attribute::electric},
			.modifier = Modifiers::combat().shock,
		};
		const static inline Anomaly shatter = Anomaly{
			.name = Formula::ConstantBase<std::string>{.value = "Shatter"},
			.multiplier = 5.f,
			.attribute = Formula::ConstantBase<Misc::Attribute>{.value = Misc::Attribute::ice},
			.modifier = Modifiers::combat().shatter,
		};
		const static inline Anomaly assault = Anomaly{
			.name = Formula::ConstantBase<std::string>{.value = "Assault"},
			.multiplier = 7.13f,
			.attribute = Formula::ConstantBase<Misc::Attribute>{.value = Misc::Attribute::physical},
			.modifier = Modifiers::combat().assault,
		};
		const static inline Anomaly corruption = Anomaly{
			.name = Formula::ConstantBase<std::string>{.value = "Corruption"},
			.multiplier = 0.625f,
			.attribute = Formula::ConstantBase<Misc::Attribute>{.value = Misc::Attribute::ether},
			.modifier = Modifiers::combat().corruption,
		};
		const static inline Anomaly windswept = Anomaly{
			.name = Formula::ConstantBase<std::string>{.value = "Windswept"},
			.multiplier = 12.5f,
			.attribute = Formula::ConstantBase<Misc::Attribute>{.value = Misc::Attribute::wind},
			.modifier = Modifiers::combat().windswept,
		};

		// All disorders are expected to have happened 5 seconds after the last anomaly
		// Maybe there could be a selectable timer in the future, but for now we just assume that it happens midway through the 10 second window
		const static inline Anomaly burnDisorder = Anomaly{
			.name = Formula::ConstantBase<std::string>{.value = "Burn Disorder"},
			.multiplier = 4.5f + 5.f * 2.f * 0.5f,
			.attribute = Formula::ConstantBase<Misc::Attribute>{.value = Misc::Attribute::fire},
			.modifier = Modifiers::combat().burn,
			.formula = makeDisorderFormula(Modifiers::combat().burnDisorder, 4.5f + 5.f * 2.f * 0.5f, Misc::Attribute::fire)
		};
		const static inline Anomaly shockDisorder = Anomaly{
			.name = Formula::ConstantBase<std::string>{.value = "Shock Disorder"},
			.multiplier = 4.5f + 5.f * 1.25f,
			.attribute = Formula::ConstantBase<Misc::Attribute>{.value = Misc::Attribute::electric},
			.modifier = Modifiers::combat().shock,
			.formula = makeDisorderFormula(Modifiers::combat().shockDisorder, 4.5f + 5.f * 1.25f, Misc::Attribute::electric)
		};
		const static inline Anomaly frozenDisorder = Anomaly{
			.name = Formula::ConstantBase<std::string>{.value = "Frozen Disorder"},
			.multiplier = 4.5f + 5.f * 0.075f,
			.attribute = Formula::ConstantBase<Misc::Attribute>{.value = Misc::Attribute::ice},
			.modifier = Modifiers::combat().shatter,
			.formula = makeDisorderFormula(Modifiers::combat().frozenDisorder, 4.5f + 5.f * 0.075f, Misc::Attribute::ice)
		};
		const static inline Anomaly frozenFrostDisorder = Anomaly{
			.name = Formula::ConstantBase<std::string>{.value = "Frozen Disorder (Frost)"},
			.multiplier = 6.f + 5.f * 0.75f,
			.attribute = Formula::ConstantBase<Misc::Attribute>{.value = Misc::Attribute::ice},
			.modifier = Modifiers::combat().shatter,
			.formula = makeDisorderFormula(Modifiers::combat().frozenFrostDisorder, 6.f + 5.f * 0.75f, Misc::Attribute::ice)
		};
		const static inline Anomaly assaultDisorder = Anomaly{
			.name = Formula::ConstantBase<std::string>{.value = "Assault Disorder"},
			.multiplier = 4.5f + 5.f * 0.075f,
			.attribute = Formula::ConstantBase<Misc::Attribute>{.value = Misc::Attribute::physical},
			.modifier = Modifiers::combat().assault,
			.formula = makeDisorderFormula(Modifiers::combat().assaultDisorder, 4.5f + 5.f * 0.075f, Misc::Attribute::physical)
		};
		const static inline Anomaly corruptionDisorder = Anomaly{
			.name = Formula::ConstantBase<std::string>{.value = "Corruption Disorder"},
			.multiplier = 4.5f + 5.f * 2.f * 0.625f,
			.attribute = Formula::ConstantBase<Misc::Attribute>{.value = Misc::Attribute::ether},
			.modifier = Modifiers::combat().corruption,
			.formula = makeDisorderFormula(Modifiers::combat().corruptionDisorder, 4.5f + 5.f * 2.f * 0.625f, Misc::Attribute::ether)
		};
		const static inline Anomaly windsweptDisorder = Anomaly{
			.name = Formula::ConstantBase<std::string>{.value = "Windswept Disorder"},
			.multiplier = 1.0f,
			.attribute = Formula::ConstantBase<Misc::Attribute>{.value = Misc::Attribute::wind},
			.modifier = Modifiers::combat().windswept,
			.formula = makeDisorderFormula(Modifiers::combat().windsweptDisorder, 1.0f, Misc::Attribute::wind)
		};
	}// namespace List
}// namespace Anomaly