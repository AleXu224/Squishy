#pragma once

#include "fmt/core.h"
#include "context.hpp"
#include "stats/loadout.hpp"
#include "utils/optional.hpp"


namespace Formula {
	using _postModsCharacter = decltype(Stats::CharacterSheet::postMods);
	struct Stat {
		::Stat stat;

		[[nodiscard]] inline std::string print(const Context &context, Step) const {
			bool isPercentage = Stats::isPercentage(stat);
			return fmt::format(
				"{} {:.2f}{}",
				Utils::Stringify(stat),
				context.target.character.sheet.postMods.fromStat(stat).get(context) * (isPercentage ? 100.f : 1.f),
				isPercentage ? "%" : ""
			);
		}

		[[nodiscard]] inline float eval(const Context &context) const {
			return context.target.character.sheet.postMods.fromStat(stat).get(context);
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

		[[nodiscard]] inline std::string print(const Context &context, Step) const {
			bool isPercentage = Stats::isSheetMemberPercentage(stat);
			const auto sheet = [&]() {
				if constexpr (std::is_same_v<U, Stats::CharacterSheet>)
					return &context.target.character.sheet;
				else if constexpr (std::is_same_v<U, Stats::WeaponSheet>)
					return &context.target.weapon.sheet;
				else if constexpr (std::is_same_v<U, Stats::ArtifactSheet>)
					return &context.target.artifact.sheet;
				else if constexpr (std::is_same_v<U, Stats::Team>)
					return &context.team;
			}();
			return fmt::format(
				"{}{} {:.2f}{}",
				prefix,
				Utils::Stringify(Stats::getSheetMemberStat(stat)),
				std::invoke(stat, std::invoke(category, *sheet)).get(context) * (isPercentage ? 100.f : 1.f),
				isPercentage ? "%" : ""
			);
		}

		[[nodiscard]] inline float eval(const Context &context) const {
			if constexpr (std::is_same_v<U, Stats::CharacterSheet>)
				return std::invoke(stat, std::invoke(category, context.target.character.sheet)).get(context);
			else if constexpr (std::is_same_v<U, Stats::WeaponSheet>)
				return std::invoke(stat, std::invoke(category, context.target.weapon.sheet)).get(context);
			else if constexpr (std::is_same_v<U, Stats::ArtifactSheet>)
				return std::invoke(stat, std::invoke(category, context.target.artifact.sheet)).get(context);
			else if constexpr (std::is_same_v<U, Stats::Team>)
				return std::invoke(stat, std::invoke(category, context.team)).get(context);
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

		[[nodiscard]] inline std::string print(const Context &context, Step) const {
			bool isPercentage = T::_SkillValue::isPercetange(stat);
			const auto sheet = [&]() {
				if constexpr (std::is_same_v<U, Stats::CharacterSheet>)
					return &context.target.character.sheet;
				else if constexpr (std::is_same_v<U, Stats::WeaponSheet>)
					return &context.target.weapon.sheet;
				else if constexpr (std::is_same_v<U, Stats::ArtifactSheet>)
					return &context.target.artifact.sheet;
				else if constexpr (std::is_same_v<U, Stats::Team>)
					return &context.team;
			}();
			return fmt::format(
				"{}{} {:.2f}{}",
				prefix,
				Utils::Stringify<T>(skill, stat),
				std::invoke(stat, std::invoke(skill, std::invoke(category, *sheet))).get(context) * (isPercentage ? 100.f : 1.f),
				isPercentage ? "%" : ""
			);
		}

		[[nodiscard]] inline float eval(const Context &context) const {
			if constexpr (std::is_same_v<U, Stats::CharacterSheet>) {
				return std::invoke(stat, std::invoke(skill, std::invoke(category, context.target.character.sheet))).get(context);
			} else if constexpr (std::is_same_v<U, Stats::WeaponSheet>) {
				return std::invoke(stat, std::invoke(skill, std::invoke(category, context.target.weapon.sheet))).get(context);
			} else if constexpr (std::is_same_v<U, Stats::ArtifactSheet>) {
				return std::invoke(stat, std::invoke(skill, std::invoke(category, context.target.artifact.sheet))).get(context);
			} else if constexpr (std::is_same_v<U, Stats::Team>) {
				return std::invoke(stat, std::invoke(skill, std::invoke(category, context.team))).get(context);
			}
		}
	};

	[[nodiscard]] inline auto _getElement(Misc::AttackSource attackSource, Utils::JankyOptional<Misc::Element> element, const Context &context) {
		switch (attackSource) {
			case Misc::AttackSource::normal:
			case Misc::AttackSource::charged:
			case Misc::AttackSource::plunge:
				return element.value_or(context.target.character.sheet.infusion.eval(context).value_or(Misc::Element::physical));
			case Misc::AttackSource::skill:
			case Misc::AttackSource::burst:
				return element.value_or(context.target.character.base.element);
		}
		std::unreachable();
	}

	struct ElementStat {
		Misc::AttackSource attackSource{};
		Utils::JankyOptional<Misc::Element> element;
		_postModsCharacter::_Value _postModsCharacter::_SkillValue:: *stat{};

		[[nodiscard]] inline std::string print(const Context &context, Step) const {
			bool isPercentage = _postModsCharacter::_SkillValue::isPercetange(stat);

			auto el = _getElement(attackSource, element, context);

			auto skill = Stats::getSheetMemberByElement<_postModsCharacter>(el);
			return fmt::format(
				"{} {:.2f}{}",
				Utils::Stringify<_postModsCharacter>(skill, stat),
				std::invoke(stat, std::invoke(skill, context.target.character.sheet.postMods)).get(context) * (isPercentage ? 100.f : 1.f),
				isPercentage ? "%" : ""
			);
		}

		[[nodiscard]] inline float eval(const Context &context) const {
			Misc::Element el = _getElement(attackSource, element, context);
			auto skill = Stats::getSheetMemberByElement<_postModsCharacter>(el);
			return std::invoke(stat, std::invoke(skill, context.target.character.sheet.postMods)).get(context);
		}
	};

	struct WeaponStat {
		::Stat stat;

		[[nodiscard]] inline std::string print(const Context &context, Step) const {
			bool isPercentage = Stats::isPercentage(stat);
			return fmt::format(
				"{} {:.2f}{}",
				Utils::Stringify(stat),
				context.target.weapon.sheet.postMods.fromStat(stat).get(context) * (isPercentage ? 100.f : 1.f),
				isPercentage ? "%" : ""
			);
		}

		[[nodiscard]] inline float eval(const Context &context) const {
			return context.target.weapon.sheet.postMods.fromStat(stat).get(context);
		}
	};

	struct ArtifactStat {
		::Stat stat;

		[[nodiscard]] inline std::string print(const Context &context, Step) const {
			bool isPercentage = Stats::isPercentage(stat);
			return fmt::format(
				"{} {:.2f}{}",
				Utils::Stringify(stat),
				context.target.artifact.sheet.postMods.fromStat(stat).get(context) * (isPercentage ? 100.f : 1.f),
				isPercentage ? "%" : ""
			);
		}

		[[nodiscard]] inline float eval(const Context &context) const {
			return context.target.artifact.sheet.postMods.fromStat(stat).get(context);
		}
	};
}// namespace Formula