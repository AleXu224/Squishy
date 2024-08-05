#pragma once

#include "functional"
#include "misc/attackSource.hpp"
#include "misc/element.hpp"
#include "stats/character.hpp"
#include "string_view"
#include <algorithm>
#include <optional>


namespace Node {
	[[nodiscard]] inline auto makeFormula(const Stat &scalingStat, const Talent &talent, const std::array<float, 15> &multipiers) {
		return [multipiers, scalingStat, talent](Stats::Character &stats) {
			return stats.sheet.fromStat(scalingStat).getTotal(stats.sheet) * multipiers.at(stats.sheet.talents.getLevel(talent));
		};
	}

	struct Atk {
		std::string_view name = "## NODE_NAME ##";
		std::optional<Misc::Element> attackElement = std::nullopt;
		Misc::AttackSource atkSource;
		Stats::Skill stats{};
		std::function<float(Stats::Character &stats)> formula;

		[[nodiscard]] Misc::Element getElement(const Stats::Character &stats) const {
			switch (atkSource) {
				case Misc::AttackSource::normal:
				case Misc::AttackSource::charged:
				case Misc::AttackSource::plunge:
					return attackElement.value_or(Misc::Element::physical);
				case Misc::AttackSource::skill:
				case Misc::AttackSource::burst:
					return attackElement.value_or(stats.base.element);
			}
		}

		float getTotal(const Stats::Character &stats, const Stats::Skill &elementStats, const Stats::Skill &talentStats, Stats::CharacterValue Stats::Skill::*stat) const {
			auto &sheet = stats.sheet;
			return std::invoke(stat, sheet.all).getTotal(sheet) +
				   std::invoke(stat, elementStats).getTotal(sheet) +
				   std::invoke(stat, talentStats).getTotal(sheet) +
				   std::invoke(stat, this->stats).getTotal(sheet);
		}

		float calculate(Stats::Character &stats) const {
			auto &sheet = stats.sheet;
			const auto element = getElement(stats);
			const auto elementStats = stats.sheet.fromElement(element);
			const auto talentStats = stats.sheet.fromAttackSource(atkSource);

			const auto totalDMG = getTotal(stats, elementStats, talentStats, &Stats::Skill::DMG);
			const auto totalAdditiveDMG = getTotal(stats, elementStats, talentStats, &Stats::Skill::additiveDMG);
			const auto totalMultiplicativeDMG = getTotal(stats, elementStats, talentStats, &Stats::Skill::multiplicativeDMG);
			const auto totalCritRate = getTotal(stats, elementStats, talentStats, &Stats::Skill::critRate);
			const auto totalCritDMG = getTotal(stats, elementStats, talentStats, &Stats::Skill::critDMG);

			return (
				((1.0f + totalMultiplicativeDMG) * formula(stats) + totalAdditiveDMG) *
				(1.0f + totalDMG) *
				(1.0f + std::clamp(sheet.cr.getTotal(sheet) + totalCritRate, 0.0f, 1.0f) * (sheet.cd.getTotal(sheet) + totalCritDMG)) *
				0.5f *       // Enemy def
				(1.0f - 0.1f)// Enemy dmg res
			);
		}
	};
}// namespace Node