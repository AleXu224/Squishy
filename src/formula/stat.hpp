#pragma once

#include "fmt/core.h"
#include "intermediary.hpp"
#include "stats/loadout.hpp"
#include "utils/optional.hpp"


namespace Formula {
	using _postModsCharacter = decltype(Stats::CharacterSheet::postMods);
	struct Stat {
		::Stat stat;

		[[nodiscard]] inline std::string print(const Stats::Loadout &stats, const Stats::Team &team, Step) const {
			bool isPercentage = Stats::isPercentage(stat);
			return fmt::format(
				"{} {:.2f}{}",
				Utils::Stringify(stat),
				stats.character.sheet.postMods.fromStat(stat).get(stats, team) * (isPercentage ? 100.f : 1.f),
				isPercentage ? "%" : ""
			);
		}

		[[nodiscard]] inline float eval(const Stats::Loadout &stats, const Stats::Team &team) const {
			return stats.character.sheet.postMods.fromStat(stat).get(stats, team);
		}
	};

	template<Stats::SheetLike T, class U>
	struct StatPtr {
		T U:: *category;
		T::_Value T:: *stat;

		std::string_view prefix = [&]() consteval {
			if (std::is_same_v<U, Stats::CharacterSheet>) return "";
			if (std::is_same_v<U, Stats::WeaponSheet>) return "Weapon ";
			if (std::is_same_v<U, Stats::ArtifactSheet>) return "Artifact ";
			if (std::is_same_v<U, Stats::Team>) return "Team ";
		}();

		[[nodiscard]] inline std::string print(const Stats::Loadout &stats, const Stats::Team &team, Step) const {
			bool isPercentage = Stats::isSheetMemberPercentage(stat);
			const auto sheet = [&]() {
				if constexpr (std::is_same_v<U, Stats::CharacterSheet>)
					return &stats.character.sheet;
				else if constexpr (std::is_same_v<U, Stats::WeaponSheet>)
					return &stats.weapon.sheet;
				else if constexpr (std::is_same_v<U, Stats::ArtifactSheet>)
					return &stats.artifact.sheet;
				else if constexpr (std::is_same_v<U, Stats::Team>)
					return &team;
			}();
			return fmt::format(
				"{}{} {:.2f}{}",
				prefix,
				Utils::Stringify(Stats::getSheetMemberStat(stat)),
				std::invoke(stat, std::invoke(category, *sheet)).get(stats, team) * (isPercentage ? 100.f : 1.f),
				isPercentage ? "%" : ""
			);
		}

		[[nodiscard]] inline float eval(const Stats::Loadout &stats, const Stats::Team &team) const {
			if constexpr (std::is_same_v<U, Stats::CharacterSheet>)
				return std::invoke(stat, std::invoke(category, stats.character.sheet)).get(stats, team);
			else if constexpr (std::is_same_v<U, Stats::WeaponSheet>)
				return std::invoke(stat, std::invoke(category, stats.weapon.sheet)).get(stats, team);
			else if constexpr (std::is_same_v<U, Stats::ArtifactSheet>)
				return std::invoke(stat, std::invoke(category, stats.artifact.sheet)).get(stats, team);
			else if constexpr (std::is_same_v<U, Stats::Team>)
				return std::invoke(stat, std::invoke(category, team)).get(stats, team);
		}
	};

	template<Stats::SheetLike T, class U>
	struct SkillPtr {
		T U:: *category;
		T::_SkillValue T:: *skill;
		T::_Value T::_SkillValue:: *stat;

		std::string_view prefix = [&]() consteval {
			if (std::is_same_v<U, Stats::CharacterSheet>) return "";
			if (std::is_same_v<U, Stats::WeaponSheet>) return "Weapon ";
			if (std::is_same_v<U, Stats::ArtifactSheet>) return "Artifact ";
			if (std::is_same_v<U, Stats::Team>) return "Team ";
		}();

		[[nodiscard]] inline std::string print(const Stats::Loadout &stats, const Stats::Team &team, Step) const {
			bool isPercentage = T::_SkillValue::isPercetange(stat);
			const auto sheet = [&]() {
				if constexpr (std::is_same_v<U, Stats::CharacterSheet>)
					return &stats.character.sheet;
				else if constexpr (std::is_same_v<U, Stats::WeaponSheet>)
					return &stats.weapon.sheet;
				else if constexpr (std::is_same_v<U, Stats::ArtifactSheet>)
					return &stats.artifact.sheet;
				else if constexpr (std::is_same_v<U, Stats::Team>)
					return &team;
			}();
			return fmt::format(
				"{}{} {:.2f}{}",
				prefix,
				Utils::Stringify<T>(skill, stat),
				std::invoke(stat, std::invoke(skill, std::invoke(category, *sheet))).get(stats, team) * (isPercentage ? 100.f : 1.f),
				isPercentage ? "%" : ""
			);
		}

		[[nodiscard]] inline float eval(const Stats::Loadout &stats, const Stats::Team &team) const {
			if constexpr (std::is_same_v<U, Stats::CharacterSheet>) {
				return std::invoke(stat, std::invoke(skill, std::invoke(category, stats.character.sheet))).get(stats, team);
			} else if constexpr (std::is_same_v<U, Stats::WeaponSheet>) {
				return std::invoke(stat, std::invoke(skill, std::invoke(category, stats.weapon.sheet))).get(stats, team);
			} else if constexpr (std::is_same_v<U, Stats::ArtifactSheet>) {
				return std::invoke(stat, std::invoke(skill, std::invoke(category, stats.artifact.sheet))).get(stats, team);
			} else if constexpr (std::is_same_v<U, Stats::Team>) {
				return std::invoke(stat, std::invoke(skill, std::invoke(category, team))).get(stats, team);
			}
		}
	};

	[[nodiscard]] inline auto _getElement(Misc::AttackSource attackSource, Utils::JankyOptional<Misc::Element> element, const Stats::Loadout &stats, const Stats::Team &team) {
		switch (attackSource) {
			case Misc::AttackSource::normal:
			case Misc::AttackSource::charged:
			case Misc::AttackSource::plunge:
				return element.value_or(stats.character.sheet.infusion.eval(stats, team).value_or(Misc::Element::physical));
			case Misc::AttackSource::skill:
			case Misc::AttackSource::burst:
				return element.value_or(stats.character.base.element);
		}
		std::unreachable();
	}

	struct ElementStat {
		Misc::AttackSource attackSource{};
		Utils::JankyOptional<Misc::Element> element;
		_postModsCharacter::_Value _postModsCharacter::_SkillValue:: *stat{};

		[[nodiscard]] inline std::string print(const Stats::Loadout &stats, const Stats::Team &team, Step) const {
			bool isPercentage = _postModsCharacter::_SkillValue::isPercetange(stat);

			auto el = _getElement(attackSource, element, stats, team);

			auto skill = Stats::getSheetMemberByElement<_postModsCharacter>(el);
			return fmt::format(
				"{} {:.2f}{}",
				Utils::Stringify<_postModsCharacter>(skill, stat),
				std::invoke(stat, std::invoke(skill, stats.character.sheet.postMods)).get(stats, team) * (isPercentage ? 100.f : 1.f),
				isPercentage ? "%" : ""
			);
		}

		[[nodiscard]] inline float eval(const Stats::Loadout &stats, const Stats::Team &team) const {
			Misc::Element el = _getElement(attackSource, element, stats, team);
			auto skill = Stats::getSheetMemberByElement<_postModsCharacter>(el);
			return std::invoke(stat, std::invoke(skill, stats.character.sheet.postMods)).get(stats, team);
		}
	};

	struct WeaponStat {
		::Stat stat;

		[[nodiscard]] inline std::string print(const Stats::Loadout &stats, const Stats::Team &team, Step) const {
			bool isPercentage = Stats::isPercentage(stat);
			return fmt::format(
				"{} {:.2f}{}",
				Utils::Stringify(stat),
				stats.weapon.sheet.postMods.fromStat(stat).get(stats, team) * (isPercentage ? 100.f : 1.f),
				isPercentage ? "%" : ""
			);
		}

		[[nodiscard]] inline float eval(const Stats::Loadout &stats, const Stats::Team &team) const {
			return stats.weapon.sheet.postMods.fromStat(stat).get(stats, team);
		}
	};

	struct ArtifactStat {
		::Stat stat;

		[[nodiscard]] inline std::string print(const Stats::Loadout &stats, const Stats::Team &team, Step) const {
			bool isPercentage = Stats::isPercentage(stat);
			return fmt::format(
				"{} {:.2f}{}",
				Utils::Stringify(stat),
				stats.artifact.sheet.postMods.fromStat(stat).get(stats, team) * (isPercentage ? 100.f : 1.f),
				isPercentage ? "%" : ""
			);
		}

		[[nodiscard]] inline float eval(const Stats::Loadout &stats, const Stats::Team &team) const {
			return stats.artifact.sheet.postMods.fromStat(stat).get(stats, team);
		}
	};
}// namespace Formula