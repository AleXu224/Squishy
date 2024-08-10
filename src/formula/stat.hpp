#pragma once

#include "fmt/core.h"
#include "intermediary.hpp"
#include "stats/sheet.hpp"


namespace Formula {
	[[nodiscard]] consteval auto Stat(::Stat stat) {
		return Intermediary{
			.print = [stat](const Stats::Sheet &stats, Step) -> std::string {
				bool isPercentage = Stats::isPercentage(stat);
				return fmt::format(
					"{} {:.2f}{}",
					Utils::Stringify(stat),
					stats.character.sheet.fromStat(stat).getTotal(stats) * (isPercentage ? 100.f : 1.f),
					isPercentage ? "%" : ""
				);
			},
			.eval = [stat](const Stats::Sheet &stats) -> float {
				return stats.character.sheet.fromStat(stat).getTotal(stats);
			},
		};
	}

	template<Stats::SheetLike T>
	[[nodiscard]] consteval auto StatPtr(Stats::SV T::*stat) {
		std::string_view prefix = [&]() consteval {
			if (std::is_same_v<T, Stats::CharacterSheet>) return "";
			if (std::is_same_v<T, Stats::WeaponSheet>) return "Weapon ";
			if (std::is_same_v<T, Stats::ArtifactSheet>) return "Artifact ";
		}();


		return Intermediary{
			.print = [stat, prefix](const Stats::Sheet &stats, Step) -> std::string {
				bool isPercentage = Stats::isSheetMemberPercentage(stat);
				// bool isPercentage = false;
				const auto &sheet = [&]() {
					if constexpr (std::is_same_v<T, Stats::CharacterSheet>)
						return stats.character;
					else if constexpr (std::is_same_v<T, Stats::WeaponSheet>)
						return stats.weapon;
					else if constexpr (std::is_same_v<T, Stats::ArtifactSheet>)
						return stats.artifact;
				}();
				return fmt::format(
					"{}{} {:.2f}{}",
					prefix,
					Utils::Stringify(Stats::getSheetMemberStat(stat)),
					std::invoke(stat, sheet.sheet).getTotal(stats) * (isPercentage ? 100.f : 1.f),
					isPercentage ? "%" : ""
				);
			},
			.eval = [stat](const Stats::Sheet &stats) -> float {
				const auto &sheet = [&]() {
					if constexpr (std::is_same_v<T, Stats::CharacterSheet>)
						return stats.character;
					else if constexpr (std::is_same_v<T, Stats::WeaponSheet>)
						return stats.weapon;
					else if constexpr (std::is_same_v<T, Stats::ArtifactSheet>)
						return stats.artifact;
				}();
				return std::invoke(stat, sheet.sheet).getTotal(stats);
			},
		};
	}

	template<Stats::SheetLike T>
	[[nodiscard]] consteval auto SkillPtr(Stats::SSV T::*skill, Stats::SV Stats::SSV::*stat) {
		std::string_view prefix = [&]() consteval {
			if (std::is_same_v<T, Stats::CharacterSheet>) return "";
			if (std::is_same_v<T, Stats::WeaponSheet>) return "Weapon ";
			if (std::is_same_v<T, Stats::ArtifactSheet>) return "Artifact ";
		}();


		return Intermediary{
			.print = [skill, stat, prefix](const Stats::Sheet &stats, Step) -> std::string {
				bool isPercentage = Stats::SSV::isPercetange(stat);
				// bool isPercentage = false;
				const auto &sheet = [&]() {
					if constexpr (std::is_same_v<T, Stats::CharacterSheet>)
						return stats.character;
					else if constexpr (std::is_same_v<T, Stats::WeaponSheet>)
						return stats.weapon;
					else if constexpr (std::is_same_v<T, Stats::ArtifactSheet>)
						return stats.artifact;
				}();
				return fmt::format(
					"{}{} {:.2f}{}",
					prefix,
					Utils::Stringify(skill, stat),
					std::invoke(stat, std::invoke(skill, sheet.sheet)).getTotal(stats) * (isPercentage ? 100.f : 1.f),
					isPercentage ? "%" : ""
				);
			},
			.eval = [skill, stat](const Stats::Sheet &stats) -> float {
				const auto &sheet = [&]() {
					if constexpr (std::is_same_v<T, Stats::CharacterSheet>)
						return stats.character;
					else if constexpr (std::is_same_v<T, Stats::WeaponSheet>)
						return stats.weapon;
					else if constexpr (std::is_same_v<T, Stats::ArtifactSheet>)
						return stats.artifact;
				}();
				return std::invoke(stat, std::invoke(skill, sheet.sheet)).getTotal(stats);
			},
		};
	}

	[[nodiscard]] consteval auto WeaponStat(::Stat stat) {
		return Intermediary{
			.print = [stat](const Stats::Sheet &stats, Step) -> std::string {
				bool isPercentage = Stats::isPercentage(stat);
				return fmt::format(
					"{} {:.2f}{}",
					Utils::Stringify(stat),
					stats.weapon.sheet.fromStat(stat).getTotal(stats) * (isPercentage ? 100.f : 1.f),
					isPercentage ? "%" : ""
				);
			},
			.eval = [stat](const Stats::Sheet &stats) -> float {
				return stats.weapon.sheet.fromStat(stat).getTotal(stats);
			},
		};
	}

	[[nodiscard]] consteval auto ArtifactStat(::Stat stat) {
		return Intermediary{
			.print = [stat](const Stats::Sheet &stats, Step) -> std::string {
				bool isPercentage = Stats::isPercentage(stat);
				return fmt::format(
					"{} {:.2f}{}",
					Utils::Stringify(stat),
					stats.artifact.sheet.fromStat(stat).getTotal(stats) * (isPercentage ? 100.f : 1.f),
					isPercentage ? "%" : ""
				);
			},
			.eval = [stat](const Stats::Sheet &stats) -> float {
				return stats.artifact.sheet.fromStat(stat).getTotal(stats);
			},
		};
	}
}// namespace Formula