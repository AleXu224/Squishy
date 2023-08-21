#pragma once
#include "element.hpp"
#include "functional"
#include "stat.hpp"
#include "statSheet.hpp"
#include <array>
#include <functional>
#include <optional>
#include <string>
#include <utility>
#include <variant>
#include <vector>


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
		bool active{true};
		AttackType attackType = AttackType::Normal;
		std::optional<DMGElement> element{};
		struct StatMultiplier {
			AbilityScalingStat stat = AbilityScalingStat::ATK;
			float multiplier{0.0f};
		};
		std::array<StatMultiplier, 2> statMultipliers;
		struct Buffs {
			std::vector<std::function<float(const StatSheet &)>> BaseDmg{};
			std::vector<std::function<float(const StatSheet &)>> AdditiveDmg{};
			std::vector<std::function<float(const StatSheet &)>> Dmg{};
		};
		Buffs buffs;

		[[nodiscard]] inline DMGElement getDMGElement(const StatSheet &stats) const {
			if (!element.has_value() && (attackType == AttackType::Normal || attackType == AttackType::Charged || attackType == AttackType::Plunge)) {
				return DMGElement::Physical;
			}
			return element.value_or(static_cast<DMGElement>(stats.element));
		}

		// This should never be called in performance critical code
		// The only purpose of this function is to display the damage in the UI
		[[nodiscard]] inline float calculate(const StatSheet &sheet) const {
			const NodeFormula formula = bakeFormula(sheet);
			return formula(sheet);
		}
		
		// Perform all the necessary precomputations and return a function that calculates the damage
		// This needs to be refreshed every time a talent level or character level changes
		[[nodiscard]] inline NodeFormula bakeFormula(const StatSheet &sheet) const {
			const DMGElement element = getDMGElement(sheet);
			const auto elementStatsPtr = [&]() {
				switch (element) {
					case DMGElement::Anemo:
						return &StatSheet::Stats::Anemo;
					case DMGElement::Cryo:
						return &StatSheet::Stats::Cryo;
					case DMGElement::Dendro:
						return &StatSheet::Stats::Dendro;
					case DMGElement::Electro:
						return &StatSheet::Stats::Electro;
					case DMGElement::Geo:
						return &StatSheet::Stats::Geo;
					case DMGElement::Hydro:
						return &StatSheet::Stats::Hydro;
					case DMGElement::Pyro:
						return &StatSheet::Stats::Pyro;
					case DMGElement::Physical:
						return &StatSheet::Stats::Physical;
					default:
						std::unreachable();
				}
			}();
			const auto talentStatsPtr = [&]() {
				switch (attackType) {
					case AttackType::Normal:
						return &StatSheet::Stats::Normal;
					case AttackType::Charged:
						return &StatSheet::Stats::Charged;
					case AttackType::Plunge:
						return &StatSheet::Stats::Plunge;
					case AttackType::Skill:
						return &StatSheet::Stats::Skill;
					case AttackType::Burst:
						return &StatSheet::Stats::Burst;
					default:
						std::unreachable();
				}
			}();
			return [buffs = buffs,
					statMultipliers = statMultipliers,
					elementStatsPtr = elementStatsPtr,
					talentStatsPtr = talentStatsPtr](const StatSheet &sheet) -> float {
				const StatSheet::Stats::SkillStats &talentStats = std::invoke(talentStatsPtr, sheet.stats);
				const StatSheet::Stats::SkillStats &elementStats = std::invoke(elementStatsPtr, sheet.stats);

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
				const float totalSkillBaseDMGMultiplier = [&]() {
					float total = 1.f;
					for (const auto &multiplier: buffs.BaseDmg) {
						total += multiplier(sheet);
					}
					return total;
				}();
				const float totalSkillAdditiveDMG = [&]() {
					float total = 0.f;
					for (const auto &multiplier: buffs.AdditiveDmg) {
						total += multiplier(sheet);
					}
					return total;
				}();
				const float totalSkillDMG = [&]() {
					float total = 0.f;
					for (const auto &multiplier: buffs.Dmg) {
						total += multiplier(sheet);
					}
					return total;
				}();
				return (
					(totalBaseDMG * totalSkillBaseDMGMultiplier + totalAdditiveDMG + totalSkillAdditiveDMG) *
					(1.0f + totalDMG + totalSkillDMG) *
					(1.0f + std::min(1.0f, totalCritRate) * totalCritDMG) *
					(0.5f) *     // Enemy def
					(1.0f - 0.1f)// Enemy resistance
				);
			};
		};
	};

	struct InfoNode {
		std::string name;
		bool active{true};
		bool isPercentage = false;
		float value;
	};

	struct StatModifierNode {
		std::string name;
		bool active{true};
		StatModifier modifier;
	};

	using Node = std::variant<DmgNode, InfoNode, StatModifierNode>;

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