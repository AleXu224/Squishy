#pragma once

#include "fmt/core.h"
#include "intermediary.hpp"
#include "stats/loadout.hpp"
#include "utils/optional.hpp"


namespace Formula {
	using _PostModsCharacter = decltype(decltype(Stats::CharacterSheet::stats)::postMods);
	struct Stat {
		::Stat stat;

		[[nodiscard]] inline std::string print(const Stats::Loadout &stats, Step) const {
			bool isPercentage = Stats::isPercentage(stat);
			return fmt::format(
				"{} {:.2f}{}",
				Utils::Stringify(stat),
				stats.character.sheet.stats.postMods.fromStat(stat).get(stats) * (isPercentage ? 100.f : 1.f),
				isPercentage ? "%" : ""
			);
		}

		[[nodiscard]] inline float eval(const Stats::Loadout &stats) const {
			return stats.character.sheet.stats.postMods.fromStat(stat).get(stats);
		}
	};

	template<Stats::SheetLike T, class U>
	struct StatPtr {
		T::_Value T:: *stat;
		T decltype(U::stats):: *category;

		std::string_view prefix = [&]() consteval {
			if (std::is_same_v<U, Stats::CharacterSheet>) return "";
			if (std::is_same_v<U, Stats::WeaponSheet>) return "Weapon ";
			if (std::is_same_v<U, Stats::ArtifactSheet>) return "Artifact ";
		}();

		[[nodiscard]] inline std::string print(const Stats::Loadout &stats, Step) const {
			bool isPercentage = Stats::isSheetMemberPercentage(stat);
			const auto sheet = [&]() {
				if constexpr (std::is_same_v<U, Stats::CharacterSheet>)
					return &stats.character;
				else if constexpr (std::is_same_v<U, Stats::WeaponSheet>)
					return &stats.weapon;
				else if constexpr (std::is_same_v<U, Stats::ArtifactSheet>)
					return &stats.artifact;
			}();
			return fmt::format(
				"{}{} {:.2f}{}",
				prefix,
				Utils::Stringify(Stats::getSheetMemberStat(stat)),
				std::invoke(stat, std::invoke(category, sheet->sheet.stats)).get(stats) * (isPercentage ? 100.f : 1.f),
				isPercentage ? "%" : ""
			);
		}

		[[nodiscard]] inline float eval(const Stats::Loadout &stats) const {
			if constexpr (std::is_same_v<U, Stats::CharacterSheet>)
				return std::invoke(stat, std::invoke(category, stats.character.sheet.stats)).get(stats);
			else if constexpr (std::is_same_v<U, Stats::WeaponSheet>)
				return std::invoke(stat, std::invoke(category, stats.weapon.sheet.stats)).get(stats);
			else if constexpr (std::is_same_v<U, Stats::ArtifactSheet>)
				return std::invoke(stat, std::invoke(category, stats.artifact.sheet.stats)).get(stats);
		}
	};

	template<Stats::SheetLike T, class U>
	struct SkillPtr {
		T::_SkillValue T:: *skill;
		T::_Value T::_SkillValue:: *stat;
		T decltype(U::stats):: *category;

		std::string_view prefix = [&]() consteval {
			if (std::is_same_v<U, Stats::CharacterSheet>) return "";
			if (std::is_same_v<U, Stats::WeaponSheet>) return "Weapon ";
			if (std::is_same_v<U, Stats::ArtifactSheet>) return "Artifact ";
		}();

		[[nodiscard]] inline std::string print(const Stats::Loadout &stats, Step) const {
			bool isPercentage = T::_SkillValue::isPercetange(stat);
			// FIXME: do the same optimization as below with if constexpr
			// otherwise this will do a copy instead !!!!
			const auto sheet = [&]() {
				if constexpr (std::is_same_v<U, Stats::CharacterSheet>)
					return &stats.character;
				else if constexpr (std::is_same_v<U, Stats::WeaponSheet>)
					return &stats.weapon;
				else if constexpr (std::is_same_v<U, Stats::ArtifactSheet>)
					return &stats.artifact;
			}();
			return fmt::format(
				"{}{} {:.2f}{}",
				prefix,
				Utils::Stringify<T>(skill, stat),
				std::invoke(stat, std::invoke(skill, std::invoke(category, sheet->sheet.stats))).get(stats) * (isPercentage ? 100.f : 1.f),
				isPercentage ? "%" : ""
			);
		}

		[[nodiscard]] inline float eval(const Stats::Loadout &stats) const {
			if constexpr (std::is_same_v<U, Stats::CharacterSheet>) {
				return std::invoke(stat, std::invoke(skill, std::invoke(category, stats.character.sheet.stats))).get(stats);
			} else if constexpr (std::is_same_v<U, Stats::WeaponSheet>) {
				return std::invoke(stat, std::invoke(skill, std::invoke(category, stats.weapon.sheet.stats))).get(stats);
			} else if constexpr (std::is_same_v<U, Stats::ArtifactSheet>) {
				return std::invoke(stat, std::invoke(skill, std::invoke(category, stats.artifact.sheet.stats))).get(stats);
			}
		}
	};

	[[nodiscard]] inline auto _getElement(Misc::AttackSource attackSource, Utils::JankyOptional<Misc::Element> element, const Stats::Loadout &stats) {
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

	template<class T>
	struct ElementStat {
		Misc::AttackSource attackSource;
		Utils::JankyOptional<Misc::Element> element;
		T::_Value T::_SkillValue:: *stat;

		[[nodiscard]] inline std::string print(const Stats::Loadout &stats, Step) const {
			bool isPercentage = T::_SkillValue::isPercetange(stat);

			auto el = _getElement(attackSource, element, stats);

			auto skill = Stats::getSheetMemberByElement<_PostModsCharacter>(el);
			return fmt::format(
				"{} {:.2f}{}",
				Utils::Stringify<T>(skill, stat),
				std::invoke(stat, std::invoke(skill, stats.character.sheet.stats.postMods)).get(stats) * (isPercentage ? 100.f : 1.f),
				isPercentage ? "%" : ""
			);
		}

		[[nodiscard]] inline float eval(const Stats::Loadout &stats) const {
			Misc::Element el = _getElement(attackSource, element, stats);
			auto skill = Stats::getSheetMemberByElement<_PostModsCharacter>(el);
			return std::invoke(stat, std::invoke(skill, stats.character.sheet.stats.postMods)).get(stats);
		}
	};

	struct WeaponStat {
		::Stat stat;

		[[nodiscard]] inline std::string print(const Stats::Loadout &stats, Step) const {
			bool isPercentage = Stats::isPercentage(stat);
			return fmt::format(
				"{} {:.2f}{}",
				Utils::Stringify(stat),
				stats.weapon.sheet.stats.postMods.fromStat(stat).get(stats) * (isPercentage ? 100.f : 1.f),
				isPercentage ? "%" : ""
			);
		}

		[[nodiscard]] inline float eval(const Stats::Loadout &stats) const {
			return stats.weapon.sheet.stats.postMods.fromStat(stat).get(stats);
		}
	};

	struct ArtifactStat {
		::Stat stat;

		[[nodiscard]] inline std::string print(const Stats::Loadout &stats, Step) const {
			bool isPercentage = Stats::isPercentage(stat);
			return fmt::format(
				"{} {:.2f}{}",
				Utils::Stringify(stat),
				stats.artifact.sheet.stats.postMods.fromStat(stat).get(stats) * (isPercentage ? 100.f : 1.f),
				isPercentage ? "%" : ""
			);
		}

		[[nodiscard]] inline float eval(const Stats::Loadout &stats) const {
			return stats.artifact.sheet.stats.postMods.fromStat(stat).get(stats);
		}
	};
}// namespace Formula