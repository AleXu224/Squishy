#pragma once
#include "element.hpp"
#include "functional"
#include "stat.hpp"
#include "statSheet.hpp"
#include "talent.hpp"
#include <array>
#include <optional>
#include <span>
#include <string>
#include <utility>
#include <variant>


namespace Squishy {
	using NodeFormula = std::function<float(const StatSheet &)>;
	enum class AttackType {
		Normal,
		Charged,
		Plunge,
		Skill,
		Burst,
	};

	struct DmgNode {
		std::string name;
		AttackType attackType = AttackType::Normal;
		std::optional<DMGElement> element{};
		struct StatMultiplier {
			AbilityScalingStat stat = AbilityScalingStat::ATK;
			float multiplier{0.0f};
		};
		std::array<StatMultiplier, 2> statMultipliers;

		[[nodiscard]] inline DMGElement getDMGElement(const StatSheet &stats) const {
			if (!element.has_value() && (attackType == AttackType::Normal || attackType == AttackType::Charged || attackType == AttackType::Plunge)) {
				return DMGElement::Physical;
			}
			return element.value_or(static_cast<DMGElement>(stats.element));
		}

		// Perform all the necessary precomputations and return a function that calculates the damage
		// This needs to be refreshed every time a talent level or character level changes
		[[nodiscard]] inline float calculate(const StatSheet &sheet) const {
			const StatSheet::Stats::SkillStats &talentStats = [&]() -> const StatSheet::Stats::SkillStats & {
				switch (attackType) {
					case AttackType::Normal:
						return sheet.stats.Normal;
					case AttackType::Charged:
						return sheet.stats.Charged;
					case AttackType::Plunge:
						return sheet.stats.Plunge;
					case AttackType::Skill:
						return sheet.stats.Skill;
					case AttackType::Burst:
						return sheet.stats.Burst;
					default:
						std::unreachable();
				}
			}();

			const StatSheet::Stats::SkillStats &elementStats = [&]() -> const StatSheet::Stats::SkillStats & {
				DMGElement elementUsed = getDMGElement(sheet);
				switch (elementUsed) {
					case DMGElement::Anemo:
						return sheet.stats.Anemo;
					case DMGElement::Cryo:
						return sheet.stats.Cryo;
					case DMGElement::Dendro:
						return sheet.stats.Dendro;
					case DMGElement::Electro:
						return sheet.stats.Electro;
					case DMGElement::Geo:
						return sheet.stats.Geo;
					case DMGElement::Hydro:
						return sheet.stats.Hydro;
					case DMGElement::Pyro:
						return sheet.stats.Pyro;
					case DMGElement::Physical:
						return sheet.stats.Physical;
					default:
						std::unreachable();
				}
			}();

			const StatSheet::Stats &stat = sheet.stats;
			const float totalAdditiveDMG = [&]() {
				return stat.All.AdditiveDMG.getTotal(sheet) + talentStats.AdditiveDMG.getTotal(sheet) + elementStats.AdditiveDMG.getTotal(sheet);
			}();
			const float totalDMG = [&]() {
				return stat.All.DMG.getTotal(sheet) + talentStats.DMG.getTotal(sheet) + elementStats.DMG.getTotal(sheet);
			}();
			const float totalCritRate = [&]() {
				return stat.All.CritRate.getTotal(sheet) + talentStats.CritRate.getTotal(sheet) + elementStats.CritRate.getTotal(sheet);
			}();
			const float totalCritDMG = [&]() {
				return stat.All.CritDMG.getTotal(sheet) + talentStats.CritDMG.getTotal(sheet) + elementStats.CritDMG.getTotal(sheet);
			}();
			const float totalBaseDMG = [&]() {
				float total = 0.0f;
				for (const auto &statMultiplier: statMultipliers) {
					if (statMultiplier.multiplier == 0.0f) {
						continue;
					}
					switch (statMultiplier.stat) {
						case AbilityScalingStat::HP:
							total += sheet.stats.HP.getTotal(sheet) * statMultiplier.multiplier;
							break;
						case AbilityScalingStat::ATK:
							total += sheet.stats.ATK.getTotal(sheet) * statMultiplier.multiplier;
							break;
						case AbilityScalingStat::DEF:
							total += sheet.stats.DEF.getTotal(sheet) * statMultiplier.multiplier;
							break;
						case AbilityScalingStat::EM:
							total += sheet.stats.EM.getTotal(sheet) * statMultiplier.multiplier;
							break;
						default:
							std::unreachable();
					}
				}
				return total;
			}();
			return (
				(totalBaseDMG + totalAdditiveDMG) *
				(1.0f + totalDMG) *
				(1.0f + std::min(1.0f, totalCritRate) * totalCritDMG) *
				(0.5f) *     // Enemy def
				(1.0f - 0.1f)// Enemy resistance
			);
		}
		[[nodiscard]] inline NodeFormula bakeFormula(const StatSheet &sheet) const {
			const DMGElement element = getDMGElement(sheet);
			return [=](const StatSheet &stats) -> float {
				const StatSheet::Stats::SkillStats &talentStats = [&]() -> const StatSheet::Stats::SkillStats & {
					switch (attackType) {
						case AttackType::Normal:
							return stats.stats.Normal;
						case AttackType::Charged:
							return stats.stats.Charged;
						case AttackType::Plunge:
							return stats.stats.Plunge;
						case AttackType::Skill:
							return stats.stats.Skill;
						case AttackType::Burst:
							return stats.stats.Burst;
						default:
							std::unreachable();
					}
				}();

				const StatSheet::Stats::SkillStats &elementStats = [&]() -> const StatSheet::Stats::SkillStats & {
					switch (element) {
						case DMGElement::Anemo:
							return stats.stats.Anemo;
						case DMGElement::Cryo:
							return stats.stats.Cryo;
						case DMGElement::Dendro:
							return stats.stats.Dendro;
						case DMGElement::Electro:
							return stats.stats.Electro;
						case DMGElement::Geo:
							return stats.stats.Geo;
						case DMGElement::Hydro:
							return stats.stats.Hydro;
						case DMGElement::Pyro:
							return stats.stats.Pyro;
						case DMGElement::Physical:
							return stats.stats.Physical;
						default:
							std::unreachable();
					}
				}();

				const StatSheet::Stats &stat = stats.stats;
				const float totalAdditiveDMG = [&]() {
					return stat.All.AdditiveDMG.getTotal(stats) + talentStats.AdditiveDMG.getTotal(stats) + elementStats.AdditiveDMG.getTotal(stats);
				}();
				const float totalDMG = [&]() {
					return stat.All.DMG.getTotal(stats) + talentStats.DMG.getTotal(stats) + elementStats.DMG.getTotal(stats);
				}();
				const float totalCritRate = [&]() {
					return stat.All.CritRate.getTotal(stats) + talentStats.CritRate.getTotal(stats) + elementStats.CritRate.getTotal(stats);
				}();
				const float totalCritDMG = [&]() {
					return stat.All.CritDMG.getTotal(stats) + talentStats.CritDMG.getTotal(stats) + elementStats.CritDMG.getTotal(stats);
				}();
				const float totalBaseDMG = [&]() {
					float total = 0.0f;
					for (const auto &statMultiplier: statMultipliers) {
						if (statMultiplier.multiplier == 0.0f) {
							continue;
						}
						switch (statMultiplier.stat) {
							case AbilityScalingStat::HP:
								total += stats.stats.HP.getTotal(stats) * statMultiplier.multiplier;
								break;
							case AbilityScalingStat::ATK:
								total += stats.stats.ATK.getTotal(stats) * statMultiplier.multiplier;
								break;
							case AbilityScalingStat::DEF:
								total += stats.stats.DEF.getTotal(stats) * statMultiplier.multiplier;
								break;
							case AbilityScalingStat::EM:
								total += stats.stats.EM.getTotal(stats) * statMultiplier.multiplier;
								break;
							default:
								std::unreachable();
						}
					}
					return total;
				}();
				return (
					(totalBaseDMG + totalAdditiveDMG) *
					(1.0f + totalDMG) *
					(1.0f + std::min(1.0f, totalCritRate) * totalCritDMG) *
					(0.5f) *     // Enemy def
					(1.0f - 0.1f)// Enemy resistance
				);
			};
		};
	};

	struct InfoNode {
		std::string name;
		bool isPercentage = false;
		float value;
	};

	using Node = std::variant<DmgNode, InfoNode>;

	struct Nodes {
		using NodesVec = std::vector<Node>;
		NodesVec normal{};
		NodesVec charged{};
		NodesVec plunge{};
		NodesVec skill{};
		NodesVec burst{};

		NodesVec passive1{};
		NodesVec passive2{};

		NodesVec constellation1{};
		NodesVec constellation2{};
		NodesVec constellation4{};
		NodesVec constellation6{};
	};
}// namespace Squishy