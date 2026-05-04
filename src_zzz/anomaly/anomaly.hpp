#pragma once

#include "formula/agent.hpp"
#include "formula/enemy.hpp"
#include "formula/operators.hpp"
#include "misc/attribute.hpp"
#include "string_view"


namespace Anomaly {
	using namespace Formula::Operators;
	[[nodiscard]] static Formula::FloatNode makeAnomalyFormula(const Stats::Sheet<Formula::FloatNode>::_SkillValue &modifier, float multiplier, Misc::Attribute attribute) {
		auto baseDmg = Modifiers::combat().atk * multiplier;
		auto dmgMod = Modifiers::combat().fromAttribute(attribute).DMG + Modifiers::combat().all.DMG;
		auto anomalyMod = modifier.DMG;
		auto resMod = Formula::EnemyResMultiplier{.attackSource{}, .element = attribute};
		auto defMod = Formula::EnemyDefMultiplier{};
		// FIXME: stunMod, dmg taken
		auto apBonus = Modifiers::combat().ap / Formula::ConstantFlat{.value = 100.f};
		auto buffLevelMod = 1.f + (Formula::AgentLevel{} - 1.f) / Formula::ConstantFlat{.value = 59.f};

		return baseDmg
			 * (1.f + dmgMod)
			 * (1.f + anomalyMod)
			 * resMod
			 * defMod
			 * apBonus
			 * buffLevelMod;
	}

	struct Anomaly {
		std::string_view name;
		float multiplier{};
		Misc::Attribute attribute{};
		const Stats::Sheet<Formula::FloatNode>::_SkillValue &modifier;
		Formula::FloatNode formula = makeAnomalyFormula(modifier, multiplier, attribute);
	};

	namespace List {
		const static inline Anomaly burn = Anomaly{
			.name = "Burn",
			.multiplier = 0.5f,
			.attribute = Misc::Attribute::fire,
			.modifier = Modifiers::combat().burn,
		};
		const static inline Anomaly shock = Anomaly{
			.name = "Shock",
			.multiplier = 1.25f,
			.attribute = Misc::Attribute::electric,
			.modifier = Modifiers::combat().shock,
		};
		const static inline Anomaly shatter = Anomaly{
			.name = "Shatter",
			.multiplier = 5.f,
			.attribute = Misc::Attribute::ice,
			.modifier = Modifiers::combat().shatter,
		};
		const static inline Anomaly assault = Anomaly{
			.name = "Assault",
			.multiplier = 7.13f,
			.attribute = Misc::Attribute::physical,
			.modifier = Modifiers::combat().assault,
		};
		const static inline Anomaly corruption = Anomaly{
			.name = "Corruption",
			.multiplier = 0.625f,
			.attribute = Misc::Attribute::ether,
			.modifier = Modifiers::combat().corruption,
		};

		// All disorders are expected to have happened 5 seconds after the last anomaly
		// Maybe there could be a selectable timer in the future, but for now we just assume that it happens midway through the 10 second window
		const static inline Anomaly burnDisorder = Anomaly{
			.name = "Burn Disorder",
			.multiplier = 4.5f + 5.f * 2.f * 0.5f,
			.attribute = Misc::Attribute::fire,
			.modifier = Modifiers::combat().burn,
		};
		const static inline Anomaly shockDisorder = Anomaly{
			.name = "Shock Disorder",
			.multiplier = 4.5f + 5.f * 1.25f,
			.attribute = Misc::Attribute::electric,
			.modifier = Modifiers::combat().shock,
		};
		const static inline Anomaly frozenDisorder = Anomaly{
			.name = "Frozen Disorder",
			.multiplier = 4.5f + 5.f * 0.075f,
			.attribute = Misc::Attribute::ice,
			.modifier = Modifiers::combat().shatter,
		};
		const static inline Anomaly frozenFrostDisorder = Anomaly{
			.name = "Frozen Disorder (Frost)",
			.multiplier = 6.f + 5.f * 0.75f,
			.attribute = Misc::Attribute::ice,
			.modifier = Modifiers::combat().shatter,
		};
		const static inline Anomaly assaultDisorder = Anomaly{
			.name = "Assault Disorder",
			.multiplier = 4.5f + 5.f * 0.075f,
			.attribute = Misc::Attribute::physical,
			.modifier = Modifiers::combat().assault,
		};
		const static inline Anomaly corruptionDisorder = Anomaly{
			.name = "Corruption Disorder",
			.multiplier = 4.5f + 5.f * 2.f * 0.625f,
			.attribute = Misc::Attribute::ether,
			.modifier = Modifiers::combat().corruption,
		};
	}// namespace List
}// namespace Anomaly