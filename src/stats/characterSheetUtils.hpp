#pragma once

#include "formula/formula.hpp"

namespace CharacterSheetUtils {
	template<class T>
	using _Pre = decltype(decltype(T::stats)::preMods);
	template<class T>
	using _Post = decltype(decltype(T::stats)::postMods);

	template<class T>
	[[nodiscard]] static consteval auto _PreS() {
		return &decltype(T::stats)::preMods;
	}
	template<class T>
	[[nodiscard]] static consteval auto _PostS() {
		return &decltype(T::stats)::postMods;
	}

	template<template<class> class _Location, class _Sheet, class T>
	[[nodiscard]] static consteval auto _PreStat(T &&stat) {
		return Formula::StatPtr<_Location<_Sheet>, _Sheet>(stat, _PreS<_Sheet>());
	}
	template<template<class> class _Location, class _Sheet, class T>
	[[nodiscard]] static consteval auto _PostStat(T &&stat) {
		return Formula::StatPtr<_Location<_Sheet>, _Sheet>(stat, _PostS<_Sheet>());
	}

	template<template<class> class _Location, class _Sheet, class T>
	[[nodiscard]] static consteval auto _PreSkill(T &&statTuple) {
		return Formula::SkillPtr<_Location<_Sheet>, _Sheet>(std::get<0>(statTuple), std::get<1>(statTuple), _PreS<_Sheet>());
	}
	template<template<class> class _Location, class _Sheet, class T>
	[[nodiscard]] static consteval auto _PostSkill(T &&statTuple) {
		return Formula::SkillPtr<_Location<_Sheet>, _Sheet>(std::get<0>(statTuple), std::get<1>(statTuple), _PostS<_Sheet>());
	}

	template<class T, size_t... Is>
	consteval auto _iterate(const T &data, std::index_sequence<Is...>) {
		return std::tuple{
			[&]() consteval {
				auto character = std::get<0>(data[Is]);
				auto weapon = std::get<1>(data[Is]);
				auto artifact = std::get<2>(data[Is]);

				auto &[characterPre, characterPost] = character;
				auto &[weaponPre, weaponPost] = weapon;
				auto &[artifactPre, artifactPost] = artifact;

				return std::tuple{
					std::tuple{
						characterPre,
						_PreStat<_Pre, Stats::WeaponSheet>(weaponPre) + _PreStat<_Pre, Stats::ArtifactSheet>(artifactPre),
					},
					std::tuple{
						characterPost,
						_PostStat<_Post, Stats::WeaponSheet>(weaponPost) + _PostStat<_Post, Stats::ArtifactSheet>(artifactPost),
					},
				};
			}()...,
		};
	}

	template<size_t Size, class T>
	consteval auto iterate(const T &data) {
		return _iterate(data, std::make_index_sequence<Size>());
	}

	constexpr auto dataValues = std::views::zip(
		std::views::zip(Stats::getSheetValuesMembers<_Pre<Stats::CharacterSheet>>(), Stats::getSheetValuesMembers<_Post<Stats::CharacterSheet>>()),
		std::views::zip(Stats::getSheetValuesMembers<_Pre<Stats::WeaponSheet>>(), Stats::getSheetValuesMembers<_Post<Stats::WeaponSheet>>()),
		std::views::zip(Stats::getSheetValuesMembers<_Pre<Stats::ArtifactSheet>>(), Stats::getSheetValuesMembers<_Post<Stats::ArtifactSheet>>())
	);
	[[nodiscard]] consteval auto getSheetValuesModifiers() {
		return iterate<dataValues.size()>(dataValues);
	}

	template<class T, size_t... Is>
	consteval auto _iterate2(const T &data, std::index_sequence<Is...>) {
		return std::tuple{
			[&]() consteval {
				auto [pre, post] = data[Is];
				auto &[characterPre, weaponPre, artifactPre] = pre;
				auto &[characterPost, weaponPost, artifactPost] = post;

				return std::tuple{
					std::tuple{
						characterPre,
						_PreSkill<_Pre, Stats::WeaponSheet>(weaponPre) + _PreSkill<_Pre, Stats::ArtifactSheet>(artifactPre),
					},
					std::tuple{
						characterPost,
						_PostSkill<_Post, Stats::WeaponSheet>(weaponPost) + _PostSkill<_Post, Stats::ArtifactSheet>(artifactPost),
					},
				};
			}()...,
		};
	}

	template<size_t Size, class T>
	consteval auto iterate2(const T &data) {
		return _iterate2(data, std::make_index_sequence<Size>());
	}

	static constexpr auto dataElements = std::views::zip(
		std::views::zip(
			std::views::cartesian_product(
				Stats::getSheetElementsMembers<_Pre<Stats::CharacterSheet>>(),
				_Pre<Stats::CharacterSheet>::_SkillValue::getMembers()
			),
			std::views::cartesian_product(
				Stats::getSheetElementsMembers<_Pre<Stats::WeaponSheet>>(),
				_Pre<Stats::WeaponSheet>::_SkillValue::getMembers()
			),
			std::views::cartesian_product(
				Stats::getSheetElementsMembers<_Pre<Stats::ArtifactSheet>>(),
				_Pre<Stats::ArtifactSheet>::_SkillValue::getMembers()
			)
		),
		std::views::zip(
			std::views::cartesian_product(
				Stats::getSheetElementsMembers<_Post<Stats::CharacterSheet>>(),
				_Post<Stats::CharacterSheet>::_SkillValue::getMembers()
			),
			std::views::cartesian_product(
				Stats::getSheetElementsMembers<_Post<Stats::WeaponSheet>>(),
				_Post<Stats::WeaponSheet>::_SkillValue::getMembers()
			),
			std::views::cartesian_product(
				Stats::getSheetElementsMembers<_Post<Stats::ArtifactSheet>>(),
				_Post<Stats::ArtifactSheet>::_SkillValue::getMembers()
			)
		)
	);
	[[nodiscard]] consteval auto getSheetElementsModifiers() {
		return iterate2<dataElements.size()>(dataElements);
	}

	static constexpr auto dataAttacks = std::views::zip(
		std::views::zip(
			std::views::cartesian_product(
				Stats::getSheetAttackSourceMembers<_Pre<Stats::CharacterSheet>>(),
				_Pre<Stats::CharacterSheet>::_SkillValue::getMembers()
			),
			std::views::cartesian_product(
				Stats::getSheetAttackSourceMembers<_Pre<Stats::WeaponSheet>>(),
				_Pre<Stats::WeaponSheet>::_SkillValue::getMembers()
			),
			std::views::cartesian_product(
				Stats::getSheetAttackSourceMembers<_Pre<Stats::ArtifactSheet>>(),
				_Pre<Stats::ArtifactSheet>::_SkillValue::getMembers()
			)
		),
		std::views::zip(
			std::views::cartesian_product(
				Stats::getSheetAttackSourceMembers<_Post<Stats::CharacterSheet>>(),
				_Post<Stats::CharacterSheet>::_SkillValue::getMembers()
			),
			std::views::cartesian_product(
				Stats::getSheetAttackSourceMembers<_Post<Stats::WeaponSheet>>(),
				_Post<Stats::WeaponSheet>::_SkillValue::getMembers()
			),
			std::views::cartesian_product(
				Stats::getSheetAttackSourceMembers<_Post<Stats::ArtifactSheet>>(),
				_Post<Stats::ArtifactSheet>::_SkillValue::getMembers()
			)
		)
	);
	[[nodiscard]] consteval auto getSheetAttacksModifiers() {
		return iterate2<dataAttacks.size()>(dataAttacks);
	}

	static constexpr auto sheetValueModifiers = getSheetValuesModifiers();
	static constexpr auto sheetElementModifiers = getSheetElementsModifiers();
	static constexpr auto sheetAttacksModifiers = getSheetAttacksModifiers();
}// namespace CharacterSheetUtils