#pragma once

#include "fmt/core.h"
#include "intermediary.hpp"
#include "stats/sheet.hpp"
#include "utils/optional.hpp"


namespace Formula {
	struct Stat {
		::Stat stat;

		[[nodiscard]] inline std::string print(const Stats::Sheet &stats, Step) const {
			bool isPercentage = Stats::isPercentage(stat);
			return fmt::format(
				"{} {:.2f}{}",
				Utils::Stringify(stat),
				stats.character.sheet.fromStat(stat).getTotal(stats) * (isPercentage ? 100.f : 1.f),
				isPercentage ? "%" : ""
			);
		}

		[[nodiscard]] inline float eval(const Stats::Sheet &stats) const {
			return stats.character.sheet.fromStat(stat).getTotal(stats);
		}
	};

	template<Stats::SheetLike T>
	struct StatPtr {
		Stats::SV T:: *stat;

		std::string_view prefix = [&]() consteval {
			if (std::is_same_v<T, Stats::CharacterSheet>) return "";
			if (std::is_same_v<T, Stats::WeaponSheet>) return "Weapon ";
			if (std::is_same_v<T, Stats::ArtifactSheet>) return "Artifact ";
		}();

		[[nodiscard]] inline std::string print(const Stats::Sheet &stats, Step) const {
			bool isPercentage = Stats::isSheetMemberPercentage(stat);
			const auto sheet = [&]() {
				if constexpr (std::is_same_v<T, Stats::CharacterSheet>)
					return &stats.character;
				else if constexpr (std::is_same_v<T, Stats::WeaponSheet>)
					return &stats.weapon;
				else if constexpr (std::is_same_v<T, Stats::ArtifactSheet>)
					return &stats.artifact;
			}();
			return fmt::format(
				"{}{} {:.2f}{}",
				prefix,
				Utils::Stringify(Stats::getSheetMemberStat(stat)),
				std::invoke(stat, sheet->sheet).getTotal(stats) * (isPercentage ? 100.f : 1.f),
				isPercentage ? "%" : ""
			);
		}

		[[nodiscard]] inline float eval(const Stats::Sheet &stats) const {
			if constexpr (std::is_same_v<T, Stats::CharacterSheet>)
				return std::invoke(stat, stats.character.sheet).getTotal(stats);
			else if constexpr (std::is_same_v<T, Stats::WeaponSheet>)
				return std::invoke(stat, stats.weapon.sheet).getTotal(stats);
			else if constexpr (std::is_same_v<T, Stats::ArtifactSheet>)
				return std::invoke(stat, stats.artifact.sheet).getTotal(stats);
		}
	};

	template<Stats::SheetLike T>
	struct SkillPtr {
		Stats::SSV T:: *skill;
		Stats::SV Stats::SSV:: *stat;

		std::string_view prefix = [&]() consteval {
			if (std::is_same_v<T, Stats::CharacterSheet>) return "";
			if (std::is_same_v<T, Stats::WeaponSheet>) return "Weapon ";
			if (std::is_same_v<T, Stats::ArtifactSheet>) return "Artifact ";
		}();

		[[nodiscard]] inline std::string print(const Stats::Sheet &stats, Step) const {
			bool isPercentage = Stats::SSV::isPercetange(stat);
			// FIXME: do the same optimization as below with if constexpr
			// otherwise this will do a copy instead !!!!
			const auto sheet = [&]() {
				if constexpr (std::is_same_v<T, Stats::CharacterSheet>)
					return &stats.character;
				else if constexpr (std::is_same_v<T, Stats::WeaponSheet>)
					return &stats.weapon;
				else if constexpr (std::is_same_v<T, Stats::ArtifactSheet>)
					return &stats.artifact;
			}();
			return fmt::format(
				"{}{} {:.2f}{}",
				prefix,
				Utils::Stringify(skill, stat),
				std::invoke(stat, std::invoke(skill, sheet->sheet)).getTotal(stats) * (isPercentage ? 100.f : 1.f),
				isPercentage ? "%" : ""
			);
		}

		[[nodiscard]] inline float eval(const Stats::Sheet &stats) const {
			if constexpr (std::is_same_v<T, Stats::CharacterSheet>) {
				return std::invoke(stat, std::invoke(skill, stats.character.sheet)).getTotal(stats);
			} else if constexpr (std::is_same_v<T, Stats::WeaponSheet>) {
				return std::invoke(stat, std::invoke(skill, stats.weapon.sheet)).getTotal(stats);
			} else if constexpr (std::is_same_v<T, Stats::ArtifactSheet>) {
				return std::invoke(stat, std::invoke(skill, stats.artifact.sheet)).getTotal(stats);
			}
		}
	};

	[[nodiscard]] inline auto _getElement(Misc::AttackSource attackSource, Utils::JankyOptional<Misc::Element> element, const Stats::Sheet &stats) {
		switch (attackSource) {
			case Misc::AttackSource::normal:
			case Misc::AttackSource::charged:
			case Misc::AttackSource::plunge:
				return element.value_or(Misc::Element::physical);
			case Misc::AttackSource::skill:
			case Misc::AttackSource::burst:
				return element.value_or(stats.character.base.element);
		}
		std::unreachable();
	}

	struct ElementStat {
		Misc::AttackSource attackSource;
		Utils::JankyOptional<Misc::Element> element;
		Stats::SV Stats::SSV:: *stat;

		[[nodiscard]] inline std::string print(const Stats::Sheet &stats, Step) const {
			bool isPercentage = Stats::SSV::isPercetange(stat);

			auto el = _getElement(attackSource, element, stats);

			auto skill = Stats::getSheetMemberByElement<Stats::CharacterSheet>(el);
			return fmt::format(
				"{} {:.2f}{}",
				Utils::Stringify(skill, stat),
				std::invoke(stat, std::invoke(skill, stats.character.sheet)).getTotal(stats) * (isPercentage ? 100.f : 1.f),
				isPercentage ? "%" : ""
			);
		}

		[[nodiscard]] inline float eval(const Stats::Sheet &stats) const {
			Misc::Element el = _getElement(attackSource, element, stats);
			auto skill = Stats::getSheetMemberByElement<Stats::CharacterSheet>(el);
			return std::invoke(stat, std::invoke(skill, stats.character.sheet)).getTotal(stats);
		}
	};

	struct WeaponStat {
		::Stat stat;

		[[nodiscard]] inline std::string print(const Stats::Sheet &stats, Step) const {
			bool isPercentage = Stats::isPercentage(stat);
			return fmt::format(
				"{} {:.2f}{}",
				Utils::Stringify(stat),
				stats.weapon.sheet.fromStat(stat).getTotal(stats) * (isPercentage ? 100.f : 1.f),
				isPercentage ? "%" : ""
			);
		}

		[[nodiscard]] inline float eval(const Stats::Sheet &stats) const {
			return stats.weapon.sheet.fromStat(stat).getTotal(stats);
		}
	};

	struct ArtifactStat {
		::Stat stat;

		[[nodiscard]] inline std::string print(const Stats::Sheet &stats, Step) const {
			bool isPercentage = Stats::isPercentage(stat);
			return fmt::format(
				"{} {:.2f}{}",
				Utils::Stringify(stat),
				stats.artifact.sheet.fromStat(stat).getTotal(stats) * (isPercentage ? 100.f : 1.f),
				isPercentage ? "%" : ""
			);
		}

		[[nodiscard]] inline float eval(const Stats::Sheet &stats) const {
			return stats.artifact.sheet.fromStat(stat).getTotal(stats);
		}
	};
}// namespace Formula