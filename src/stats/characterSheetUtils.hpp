#pragma once

#include "formula/operators.hpp"// IWYU pragma: keep
#include "formula/stat.hpp"     // IWYU pragma: keep
#include "helpers.hpp"
#include "stats/characterSheet.hpp"
#include "stats/team.hpp"
#include "tuple"
#include "utils.hpp"


namespace CharacterSheetUtils {
	template<class T, class V, class U>
	[[nodiscard]] consteval auto getModifiers(T charArg, V wepArg, U artArg) {
		constexpr auto makeFormula = []<class Tup>(Tup val) consteval {
			return std::get<0>(val).makeFormula() + std::get<1>(val).makeFormula();
		};

		return std::tuple{
			std::tuple{
				std::get<0>(Stats::getSheetAllMembers(charArg)),
				squi::utils::transformRange(
					std::views::zip(
						std::get<0>(Stats::getSheetAllMembers(wepArg)), std::get<0>(Stats::getSheetAllMembers(artArg))
					),
					makeFormula
				),
			},
			std::tuple{
				std::get<1>(Stats::getSheetAllMembers(charArg)),
				squi::utils::transformRange(
					std::views::zip(
						std::get<1>(Stats::getSheetAllMembers(wepArg)), std::get<1>(Stats::getSheetAllMembers(artArg))
					),
					makeFormula
				),
			},
		};
	}

	template<class T, class V>
	[[nodiscard]] consteval auto getTeamModifiers(T charArg, V teamArg) {
		constexpr auto makeFormula = []<class Tup>(Tup val) consteval {
			return val.makeFormula();
		};

		return std::tuple{
			std::tuple{
				std::get<0>(Stats::getSheetAllMembers(charArg)),
				squi::utils::transformRange(
					std::get<0>(Stats::getSheetAllMembers(teamArg)),
					makeFormula
				),
			},
			std::tuple{
				std::get<1>(Stats::getSheetAllMembers(charArg)),
				squi::utils::transformRange(
					std::get<1>(Stats::getSheetAllMembers(teamArg)),
					makeFormula
				),
			},
		};
	}

	template<class T, class V, class U>
	[[nodiscard]] consteval auto getTalentModifiers(T charArg, V wepArg, U artArg) {
		constexpr auto makeFormula = []<class Tup>(Tup val) consteval {
			return std::get<0>(val).makeFormula() + std::get<1>(val).makeFormula();
		};

		return std::tuple{
			Stats::getTalentSheetAllMembers(charArg),
			squi::utils::transformRange(
				std::views::zip(
					Stats::getTalentSheetAllMembers(wepArg), Stats::getTalentSheetAllMembers(artArg)
				),
				makeFormula
			),
		};
	}
	[[nodiscard]] consteval auto getTeamTalentModifiers() {
		constexpr auto makeFormula = []<class Tup>(Tup val) consteval {
			return val.makeFormula();
		};

		return std::tuple{
			Stats::getTalentSheetAllMembers(&Stats::CharacterSheet::talents),
			squi::utils::transformRange(
				Stats::getTalentSheetAllMembers(&Stats::Team::talents),
				makeFormula
			),
		};
	}
}// namespace CharacterSheetUtils