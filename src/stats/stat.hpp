#pragma once

#include "array"
#include "fmt/core.h"
#include "misc/element.hpp"
#include "unordered_map"
#include "utility"
#include "utils/isPercentage.hpp"// IWYU pragma: keep
#include "utils/stringify.hpp"   // IWYU pragma: keep
#include <vector>


// An underscore after the stat name means %
// Ex: hp_ -> HP%
enum class Stat : uint8_t {
	hp,
	hp_,
	baseHp,
	atk,
	atk_,
	baseAtk,
	// Attack given by characters like yunjin
	additionalAtk,
	def,
	def_,
	baseDef,
	// Energy recharge
	er,
	// Elemental Mastery
	em,
	// Crit Rate
	cr,
	// Crit Damage
	cd,
	// Healing bonus
	hb,
	incHb,
	pyroDmg,
	hydroDmg,
	cryoDmg,
	electroDmg,
	dendroDmg,
	anemoDmg,
	geoDmg,
	physicalDmg,
	allDmg,
};

namespace Stats {
	const inline std::vector characterDisplayStats{
		Stat::hp,
		Stat::atk,
		Stat::def,
		Stat::em,
		Stat::er,
		Stat::cr,
		Stat::cd
	};

	const inline std::vector all{
		std::vector{
			Stat::hp,
			Stat::hp_,
			Stat::baseHp,
			Stat::atk,
			Stat::atk_,
			Stat::baseAtk,
			Stat::additionalAtk,
			Stat::def,
			Stat::def_,
			Stat::baseDef,
			Stat::er,
			Stat::em,
			Stat::cr,
			Stat::cd,
			Stat::hb,
			Stat::incHb,
			Stat::pyroDmg,
			Stat::hydroDmg,
			Stat::cryoDmg,
			Stat::electroDmg,
			Stat::dendroDmg,
			Stat::anemoDmg,
			Stat::geoDmg,
			Stat::physicalDmg,
			Stat::allDmg,
		}
	};

	const inline std::vector simple{
		std::vector{
			Stat::hp,
			Stat::hp_,
			Stat::baseHp,
			Stat::atk,
			Stat::atk_,
			Stat::baseAtk,
			Stat::additionalAtk,
			Stat::def,
			Stat::def_,
			Stat::baseDef,
			Stat::er,
			Stat::em,
			Stat::cr,
			Stat::cd,
			Stat::hb,
			Stat::incHb,
			Stat::pyroDmg,
			Stat::hydroDmg,
			Stat::cryoDmg,
			Stat::electroDmg,
			Stat::dendroDmg,
			Stat::anemoDmg,
			Stat::geoDmg,
			Stat::physicalDmg,
			Stat::allDmg,
		}
	};

	[[maybe_unused]] constexpr Stat fromElement(const Misc::Element &element) {
		switch (element) {
			case Misc::Element::pyro:
				return Stat::pyroDmg;
			case Misc::Element::hydro:
				return Stat::hydroDmg;
			case Misc::Element::cryo:
				return Stat::cryoDmg;
			case Misc::Element::electro:
				return Stat::electroDmg;
			case Misc::Element::dendro:
				return Stat::dendroDmg;
			case Misc::Element::anemo:
				return Stat::anemoDmg;
			case Misc::Element::geo:
				return Stat::geoDmg;
			case Misc::Element::physical:
				return Stat::physicalDmg;
		}
		std::unreachable();
	}

	namespace Values {
		const std::unordered_map<Stat, std::array<float, 21>> mainStat = {
			{Stat::hp, {717, 920, 1123, 1326, 1530, 1733, 1936, 2139, 2342, 2545, 2749, 2952, 3155, 3358, 3561, 3764, 3967, 4171, 4374, 4577, 4780}},
			{Stat::hp_, {.070, .090, .110, .129, .149, .69, .189, .209, .228, .248, .268, .288, .308, .328, .347, .367, .387, .407, .427, .446, .466}},
			{Stat::atk, {47, 60, 73, 86, 100, 113, 126, 139, 152, 166, 179, 192, 205, 219, 232, 245, 258, 272, 285, 298, 311}},
			{Stat::atk_, {.070, .090, .110, .129, .149, .169, .189, .209, .228, .248, .268, .288, .308, .328, .347, .367, .387, .407, .427, .446, .466}},
			{Stat::pyroDmg, {.070, .090, .110, .129, .149, .169, .189, .209, .228, .248, .268, .288, .308, .328, .347, .367, .387, .407, .427, .446, .466}},
			{Stat::hydroDmg, {.070, .090, .110, .129, .149, .169, .189, .209, .228, .248, .268, .288, .308, .328, .347, .367, .387, .407, .427, .446, .466}},
			{Stat::cryoDmg, {.070, .090, .110, .129, .149, .169, .189, .209, .228, .248, .268, .288, .308, .328, .347, .367, .387, .407, .427, .446, .466}},
			{Stat::electroDmg, {.070, .090, .110, .129, .149, .169, .189, .209, .228, .248, .268, .288, .308, .328, .347, .367, .387, .407, .427, .446, .466}},
			{Stat::dendroDmg, {.070, .090, .110, .129, .149, .169, .189, .209, .228, .248, .268, .288, .308, .328, .347, .367, .387, .407, .427, .446, .466}},
			{Stat::anemoDmg, {.070, .090, .110, .129, .149, .169, .189, .209, .228, .248, .268, .288, .308, .328, .347, .367, .387, .407, .427, .446, .466}},
			{Stat::geoDmg, {.070, .090, .110, .129, .149, .169, .189, .209, .228, .248, .268, .288, .308, .328, .347, .367, .387, .407, .427, .446, .466}},
			{Stat::physicalDmg, {.087, .112, .137, .162, .186, .211, .236, .261, .286, .31, .335, .36, .385, .409, .434, .459, .484, .508, .533, .558, .583}},
			{Stat::er, {.078, .100, .122, .144, .166, .188, .210, .232, .254, .276, .298, .320, .342, .364, .386, .408, .430, .452, .474, .496, .518}},
			{Stat::em, {28.0, 35.9, 43.8, 51.8, 59.7, 67.6, 75.5, 83.5, 91.4, 99.3, 107.2, 115.2, 123.1, 131.0, 138.9, 146.9, 154.8, 162.7, 170.6, 178.6, 186.5}},
			{Stat::cr, {.047, .060, .073, .086, .099, .113, .126, .139, .152, .166, .179, .192, .205, .218, .232, .245, .258, .271, .284, .298, .311}},
			{Stat::cd, {.093, .120, .146, .173, .199, .225, .252, .278, .305, .331, .357, .384, .410, .437, .463, .490, .516, .542, .569, .596, .622}},
			{Stat::hb, {.054, .069, .084, .100, .115, .130, .145, .161, .176, .191, .206, .221, .237, .252, .267, .282, .298, .313, .328, .343, .359}},
		};
	}
}// namespace Stats


struct StatValue {
	Stat stat;
	float value;
};

namespace Utils {
	template<>
	[[nodiscard]] constexpr bool isPercentage<::Stat>(const ::Stat &stat) {
		switch (stat) {
			case Stat::hp_:
			case Stat::atk_:
			case Stat::def_:
			case Stat::er:
			case Stat::cr:
			case Stat::cd:
			case Stat::hb:
			case Stat::incHb:
			case Stat::pyroDmg:
			case Stat::hydroDmg:
			case Stat::cryoDmg:
			case Stat::electroDmg:
			case Stat::dendroDmg:
			case Stat::anemoDmg:
			case Stat::geoDmg:
			case Stat::physicalDmg:
			case Stat::allDmg:
				return true;
			default:
				return false;
		};
	}

	template<>
	constexpr std::string Stringify<>(const Stat &stat) {
		switch (stat) {
			case Stat::hp:
				return "HP";
			case Stat::hp_:
				return "HP%";
			case Stat::baseHp:
				return "Base HP";
			case Stat::atk:
				return "ATK";
			case Stat::atk_:
				return "ATK%";
			case Stat::baseAtk:
				return "Base ATK";
			case Stat::def:
				return "DEF";
			case Stat::def_:
				return "DEF%";
			case Stat::baseDef:
				return "Base DEF";
			case Stat::er:
				return "Energy Recharge";
			case Stat::em:
				return "Elemental Mastery";
			case Stat::cr:
				return "Crit Rate";
			case Stat::cd:
				return "Crit DMG";
			case Stat::hb:
				return "Healing Bonus";
			case Stat::incHb:
				return "Incoming Healing Bonus";
			case Stat::pyroDmg:
				return "Pyro DMG Bonus";
			case Stat::hydroDmg:
				return "Hydro DMG Bonus";
			case Stat::cryoDmg:
				return "Cryo DMG Bonus";
			case Stat::electroDmg:
				return "Electro DMG Bonus";
			case Stat::dendroDmg:
				return "Dendro DMG Bonus";
			case Stat::anemoDmg:
				return "Anemo DMG Bonus";
			case Stat::geoDmg:
				return "Geo DMG Bonus";
			case Stat::physicalDmg:
				return "Physical DMG Bonus";
			case Stat::additionalAtk:
				return "Additive DMG Bonus";
			case Stat::allDmg:
				return "All DMG Bonus";
		}
		std::unreachable();
	}

	template<>
	constexpr std::string Stringify<>(const StatValue &stat) {
		if (Utils::isPercentage(stat.stat)) {
			return fmt::format("{:.1f}%", stat.value * 100.f);
		}
		return fmt::format("{:.0f}", stat.value);
	}
}// namespace Utils
