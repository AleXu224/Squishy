#pragma once

#include "formula/percentage.hpp"
#include "misc/attribute.hpp"
#include "utility"
#include "utils/isPercentage.hpp"// IWYU pragma: keep
#include "utils/stringify.hpp"   // IWYU pragma: keep
#include <optional>
#include <unordered_map>
#include <vector>


// An underscore after the stat name means %
// Ex: hp_ -> HP%
enum class Stat : uint8_t {
	hp = 0,
	hp_ = 1,
	atk = 2,
	atk_ = 3,
	def = 4,
	def_ = 5,
	sheerForce = 24,
	// Energy recharge
	er = 6,
	er_ = 23,
	// Anomaly Proficiency
	ap = 7,
	ap_ = 8,
	// Anomaly Mastery
	am = 9,
	am_ = 10,
	// Crit Rate
	cr = 11,
	// Crit Damage
	cd = 12,
	pen = 13,
	penRatio = 14,
	impact = 15,
	impact_ = 16,

	physicalDmg = 17,
	fireDmg = 18,
	iceDmg = 19,
	electricDmg = 20,
	etherDmg = 21,
	windDmg = 25,
	allDmg = 22,
};

namespace Stats {
	const inline std::vector agentDisplayStats{
		Stat::hp,
		Stat::atk,
		Stat::def,
		Stat::sheerForce,
		Stat::impact,
		Stat::ap,
		Stat::am,
		Stat::er,
		Stat::cr,
		Stat::cd
	};

	const inline std::vector all{
		std::vector{
			Stat::hp,
			Stat::hp_,
			Stat::atk,
			Stat::atk_,
			Stat::def,
			Stat::def_,
			Stat::sheerForce,
			Stat::er,
			Stat::er_,
			Stat::ap,
			Stat::ap_,
			Stat::am,
			Stat::am_,
			Stat::cr,
			Stat::cd,
			Stat::pen,
			Stat::penRatio,
			Stat::impact,
			Stat::impact_,
			Stat::physicalDmg,
			Stat::fireDmg,
			Stat::iceDmg,
			Stat::electricDmg,
			Stat::etherDmg,
			Stat::windDmg,
			Stat::allDmg,
		}
	};

	const inline std::vector simple{
		std::vector{
			Stat::hp,
			Stat::hp_,
			Stat::atk,
			Stat::atk_,
			Stat::def,
			Stat::def_,
			Stat::sheerForce,
			Stat::er,
			Stat::er_,
			Stat::ap,
			Stat::ap_,
			Stat::am,
			Stat::am_,
			Stat::cr,
			Stat::cd,
			Stat::pen,
			Stat::penRatio,
			Stat::impact,
			Stat::impact_,
		}
	};
	const inline std::vector subStats{
		std::vector{
			Stat::hp,
			Stat::hp_,
			Stat::atk,
			Stat::atk_,
			Stat::def,
			Stat::def_,
			Stat::ap,
			Stat::cr,
			Stat::cd,
			Stat::pen,
		}
	};

	template<class T>
	[[nodiscard]] constexpr T::_Value T::*ptrFromStatSimplified(Stat stat) {
		switch (stat) {
			case Stat::hp:
				return &T::hp;
			case Stat::hp_:
				return &T::hp_;
			case Stat::atk:
				return &T::atk;
			case Stat::atk_:
				return &T::atk_;
			case Stat::def:
				return &T::def;
			case Stat::def_:
				return &T::def_;
			case Stat::er_:
				return &T::er_;
			case Stat::ap:
				return &T::ap;
			case Stat::ap_:
				return &T::ap_;
			case Stat::am_:
				return &T::am_;
			case Stat::cr:
				return &T::cr;
			case Stat::cd:
				return &T::cd;
			case Stat::pen:
				return &T::pen;
			case Stat::penRatio:
				return &T::penRatio;
			case Stat::impact_:
				return &T::impact_;
			case Stat::physicalDmg:
				return &T::physical;
			case Stat::fireDmg:
				return &T::fire;
			case Stat::iceDmg:
				return &T::ice;
			case Stat::electricDmg:
				return &T::electric;
			case Stat::etherDmg:
				return &T::ether;
			case Stat::windDmg:
				return &T::wind;
			case Stat::sheerForce:
			case Stat::er:
			case Stat::am:
			case Stat::impact:
			case Stat::allDmg:
				return &T::hp;
		}
	}

	[[maybe_unused]] constexpr Stat fromAttribute(const Misc::Attribute &attribute) {
		switch (attribute) {
			case Misc::Attribute::physical:
				return Stat::physicalDmg;
			case Misc::Attribute::fire:
				return Stat::fireDmg;
			case Misc::Attribute::ice:
				return Stat::iceDmg;
			case Misc::Attribute::electric:
				return Stat::electricDmg;
			case Misc::Attribute::ether:
				return Stat::etherDmg;
			case Misc::Attribute::wind:
				return Stat::windDmg;
		}
		std::unreachable();
	}

	namespace Values {
		struct MainStatContainer {
			std::array<float, 10> threeStar{};
			std::array<float, 13> fourStar{};
			std::array<float, 16> fiveStar{};

			float at(uint8_t rarity, uint8_t level) const {
				switch (rarity) {
					case 3:
						return threeStar.at(level);
					case 4:
						return fourStar.at(level);
					case 5:
						return fiveStar.at(level);
				}
				return {};
			}
		};

		inline const std::unordered_map<Stat, MainStatContainer> mainStat{
			{Stat::am_, {std::array{0.0250f, 0.0333f, 0.0417f, 0.0500f, 0.0583f, 0.0667f, 0.0750f, 0.0833f, 0.0917f, 0.1000f}, std::array{0.0500f, 0.0625f, 0.0750f, 0.0875f, 0.1000f, 0.1125f, 0.1250f, 0.1375f, 0.1500f, 0.1625f, 0.1750f, 0.1875f, 0.2000f}, std::array{0.0750f, 0.0900f, 0.1050f, 0.1200f, 0.1350f, 0.1500f, 0.1650f, 0.1800f, 0.1950f, 0.2100f, 0.2250f, 0.2400f, 0.2550f, 0.2700f, 0.2850f, 0.3000f}}},
			{Stat::ap, {std::array{8.f, 11.f, 13.f, 16.f, 19.f, 21.f, 24.f, 27.f, 29.f, 32.f}, std::array{15.f, 19.f, 23.f, 26.f, 30.f, 34.f, 38.f, 41.f, 45.f, 49.f, 53.f, 56.f, 60.f}, std::array{23.f, 28.f, 32.f, 37.f, 41.f, 46.f, 51.f, 55.f, 60.f, 64.f, 69.f, 74.f, 78.f, 83.f, 87.f, 92.f}}},
			{Stat::atk, {std::array{26.f, 35.f, 43.f, 52.f, 61.f, 69.f, 78.f, 87.f, 95.f, 104.f}, std::array{53.f, 66.f, 80.f, 93.f, 106.f, 119.f, 133.f, 146.f, 159.f, 172.f, 186.f, 199.f, 212.f}, std::array{79.f, 95.f, 111.f, 126.f, 142.f, 158.f, 174.f, 190.f, 205.f, 221.f, 237.f, 253.f, 269.f, 284.f, 300.f, 316.f}}},
			{Stat::atk_, {std::array{0.0250f, 0.0333f, 0.0417f, 0.0500f, 0.0583f, 0.0667f, 0.0750f, 0.0833f, 0.0917f, 0.1000f}, std::array{0.0500f, 0.0625f, 0.0750f, 0.0875f, 0.1000f, 0.1125f, 0.1250f, 0.1375f, 0.1500f, 0.1625f, 0.1750f, 0.1875f, 0.2000f}, std::array{0.0750f, 0.0900f, 0.1050f, 0.1200f, 0.1350f, 0.1500f, 0.1650f, 0.1800f, 0.1950f, 0.2100f, 0.2250f, 0.2400f, 0.2550f, 0.2700f, 0.2850f, 0.3000f}}},
			{Stat::cd, {std::array{0.0400f, 0.0533f, 0.0667f, 0.0800f, 0.0933f, 0.1067f, 0.1200f, 0.1333f, 0.1467f, 0.1600f}, std::array{0.0800f, 0.1000f, 0.1200f, 0.1400f, 0.1600f, 0.1800f, 0.2000f, 0.2200f, 0.2400f, 0.2600f, 0.2800f, 0.3000f, 0.3200f}, std::array{0.1200f, 0.1440f, 0.1680f, 0.1920f, 0.2160f, 0.2400f, 0.2640f, 0.2880f, 0.3120f, 0.3360f, 0.3600f, 0.3840f, 0.4080f, 0.4320f, 0.4560f, 0.4800f}}},
			{Stat::cr, {std::array{0.0200f, 0.0267f, 0.0333f, 0.0400f, 0.0467f, 0.0533f, 0.0600f, 0.0667f, 0.0733f, 0.0800f}, std::array{0.0400f, 0.0500f, 0.0600f, 0.0700f, 0.0800f, 0.0900f, 0.1000f, 0.1100f, 0.1200f, 0.1300f, 0.1400f, 0.1500f, 0.1600f}, std::array{0.0600f, 0.0720f, 0.0840f, 0.0960f, 0.1080f, 0.1200f, 0.1320f, 0.1440f, 0.1560f, 0.1680f, 0.1800f, 0.1920f, 0.2040f, 0.2160f, 0.2280f, 0.2400f}}},
			{Stat::def, {std::array{15.f, 20.f, 25.f, 30.f, 35.f, 40.f, 45.f, 50.f, 55.f, 60.f}, std::array{31.f, 39.f, 47.f, 54.f, 62.f, 70.f, 78.f, 85.f, 93.f, 101.f, 109.f, 116.f, 124.f}, std::array{46.f, 55.f, 64.f, 74.f, 83.f, 92.f, 101.f, 110.f, 120.f, 129.f, 138.f, 147.f, 156.f, 166.f, 175.f, 184.f}}},
			{Stat::def_, {std::array{0.0400f, 0.0533f, 0.0667f, 0.0800f, 0.0933f, 0.1067f, 0.1200f, 0.1333f, 0.1467f, 0.1600f}, std::array{0.0800f, 0.1000f, 0.1200f, 0.1400f, 0.1600f, 0.1800f, 0.2000f, 0.2200f, 0.2400f, 0.2600f, 0.2800f, 0.3000f, 0.3200f}, std::array{0.1200f, 0.1440f, 0.1680f, 0.1920f, 0.2160f, 0.2400f, 0.2640f, 0.2880f, 0.3120f, 0.3360f, 0.3600f, 0.3840f, 0.4080f, 0.4320f, 0.4560f, 0.4800f}}},
			{Stat::electricDmg, {std::array{0.0250f, 0.0333f, 0.0417f, 0.0500f, 0.0583f, 0.0667f, 0.0750f, 0.0833f, 0.0917f, 0.1000f}, std::array{0.0500f, 0.0625f, 0.0750f, 0.0875f, 0.1000f, 0.1125f, 0.1250f, 0.1375f, 0.1500f, 0.1625f, 0.1750f, 0.1875f, 0.2000f}, std::array{0.0750f, 0.0900f, 0.1050f, 0.1200f, 0.1350f, 0.1500f, 0.1650f, 0.1800f, 0.1950f, 0.2100f, 0.2250f, 0.2400f, 0.2550f, 0.2700f, 0.2850f, 0.3000f}}},
			{Stat::er_, {std::array{0.0500f, 0.0667f, 0.0833f, 0.1000f, 0.1167f, 0.1333f, 0.1500f, 0.1667f, 0.1833f, 0.2000f}, std::array{0.1000f, 0.1250f, 0.1500f, 0.1750f, 0.2000f, 0.2250f, 0.2500f, 0.2750f, 0.3000f, 0.3250f, 0.3500f, 0.3750f, 0.4000f}, std::array{0.1500f, 0.1800f, 0.2100f, 0.2400f, 0.2700f, 0.3000f, 0.3300f, 0.3600f, 0.3900f, 0.4200f, 0.4500f, 0.4800f, 0.5100f, 0.5400f, 0.5700f, 0.6000f}}},
			{Stat::etherDmg, {std::array{0.0250f, 0.0333f, 0.0417f, 0.0500f, 0.0583f, 0.0667f, 0.0750f, 0.0833f, 0.0917f, 0.1000f}, std::array{0.0500f, 0.0625f, 0.0750f, 0.0875f, 0.1000f, 0.1125f, 0.1250f, 0.1375f, 0.1500f, 0.1625f, 0.1750f, 0.1875f, 0.2000f}, std::array{0.0750f, 0.0900f, 0.1050f, 0.1200f, 0.1350f, 0.1500f, 0.1650f, 0.1800f, 0.1950f, 0.2100f, 0.2250f, 0.2400f, 0.2550f, 0.2700f, 0.2850f, 0.3000f}}},
			{Stat::fireDmg, {std::array{0.0250f, 0.0333f, 0.0417f, 0.0500f, 0.0583f, 0.0667f, 0.0750f, 0.0833f, 0.0917f, 0.1000f}, std::array{0.0500f, 0.0625f, 0.0750f, 0.0875f, 0.1000f, 0.1125f, 0.1250f, 0.1375f, 0.1500f, 0.1625f, 0.1750f, 0.1875f, 0.2000f}, std::array{0.0750f, 0.0900f, 0.1050f, 0.1200f, 0.1350f, 0.1500f, 0.1650f, 0.1800f, 0.1950f, 0.2100f, 0.2250f, 0.2400f, 0.2550f, 0.2700f, 0.2850f, 0.3000f}}},
			{Stat::hp, {std::array{183.f, 244.f, 305.f, 366.f, 427.f, 488.f, 549.f, 610.f, 671.f, 732.f}, std::array{367.f, 459.f, 551.f, 642.f, 734.f, 826.f, 918.f, 1009.f, 1101.f, 1193.f, 1285.f, 1376.f, 1468.f}, std::array{550.f, 660.f, 770.f, 880.f, 990.f, 1100.f, 1210.f, 1320.f, 1430.f, 1540.f, 1650.f, 1760.f, 1870.f, 1980.f, 2090.f, 2200.f}}},
			{Stat::hp_, {std::array{0.0250f, 0.0333f, 0.0417f, 0.0500f, 0.0583f, 0.0667f, 0.0750f, 0.0833f, 0.0917f, 0.1000f}, std::array{0.0500f, 0.0625f, 0.0750f, 0.0875f, 0.1000f, 0.1125f, 0.1250f, 0.1375f, 0.1500f, 0.1625f, 0.1750f, 0.1875f, 0.2000f}, std::array{0.0750f, 0.0900f, 0.1050f, 0.1200f, 0.1350f, 0.1500f, 0.1650f, 0.1800f, 0.1950f, 0.2100f, 0.2250f, 0.2400f, 0.2550f, 0.2700f, 0.2850f, 0.3000f}}},
			{Stat::iceDmg, {std::array{0.0250f, 0.0333f, 0.0417f, 0.0500f, 0.0583f, 0.0667f, 0.0750f, 0.0833f, 0.0917f, 0.1000f}, std::array{0.0500f, 0.0625f, 0.0750f, 0.0875f, 0.1000f, 0.1125f, 0.1250f, 0.1375f, 0.1500f, 0.1625f, 0.1750f, 0.1875f, 0.2000f}, std::array{0.0750f, 0.0900f, 0.1050f, 0.1200f, 0.1350f, 0.1500f, 0.1650f, 0.1800f, 0.1950f, 0.2100f, 0.2250f, 0.2400f, 0.2550f, 0.2700f, 0.2850f, 0.3000f}}},
			{Stat::impact_, {std::array{0.0150f, 0.0200f, 0.0250f, 0.0300f, 0.0350f, 0.0400f, 0.0450f, 0.0500f, 0.0550f, 0.0600f}, std::array{0.0300f, 0.0375f, 0.0450f, 0.0525f, 0.0600f, 0.0675f, 0.0750f, 0.0825f, 0.0900f, 0.0975f, 0.1050f, 0.1125f, 0.1200f}, std::array{0.0450f, 0.0540f, 0.0630f, 0.0720f, 0.0810f, 0.0900f, 0.0990f, 0.1080f, 0.1170f, 0.1260f, 0.1350f, 0.1440f, 0.1530f, 0.1620f, 0.1710f, 0.1800f}}},
			{Stat::penRatio, {std::array{0.0200f, 0.0267f, 0.0333f, 0.0400f, 0.0467f, 0.0533f, 0.0600f, 0.0667f, 0.0733f, 0.0800f}, std::array{0.0400f, 0.0500f, 0.0600f, 0.0700f, 0.0800f, 0.0900f, 0.1000f, 0.1100f, 0.1200f, 0.1300f, 0.1400f, 0.1500f, 0.1600f}, std::array{0.0600f, 0.0720f, 0.0840f, 0.0960f, 0.1080f, 0.1200f, 0.1320f, 0.1440f, 0.1560f, 0.1680f, 0.1800f, 0.1920f, 0.2040f, 0.2160f, 0.2280f, 0.2400f}}},
			{Stat::physicalDmg, {std::array{0.0250f, 0.0333f, 0.0417f, 0.0500f, 0.0583f, 0.0667f, 0.0750f, 0.0833f, 0.0917f, 0.1000f}, std::array{0.0500f, 0.0625f, 0.0750f, 0.0875f, 0.1000f, 0.1125f, 0.1250f, 0.1375f, 0.1500f, 0.1625f, 0.1750f, 0.1875f, 0.2000f}, std::array{0.0750f, 0.0900f, 0.1050f, 0.1200f, 0.1350f, 0.1500f, 0.1650f, 0.1800f, 0.1950f, 0.2100f, 0.2250f, 0.2400f, 0.2550f, 0.2700f, 0.2850f, 0.3000f}}},
			{Stat::windDmg, {std::array{0.0250f, 0.0333f, 0.0417f, 0.0500f, 0.0583f, 0.0667f, 0.0750f, 0.0833f, 0.0917f, 0.1000f}, std::array{0.0500f, 0.0625f, 0.0750f, 0.0875f, 0.1000f, 0.1125f, 0.1250f, 0.1375f, 0.1500f, 0.1625f, 0.1750f, 0.1875f, 0.2000f}, std::array{0.0750f, 0.0900f, 0.1050f, 0.1200f, 0.1350f, 0.1500f, 0.1650f, 0.1800f, 0.1950f, 0.2100f, 0.2250f, 0.2400f, 0.2550f, 0.2700f, 0.2850f, 0.3000f}}}
		};

		struct SubStatContainer {
			float threeStar;
			float fourStar;
			float fiveStar;

			float at(uint8_t rarity) const {
				switch (rarity) {
					case 3:
						return threeStar;
					case 4:
						return fourStar;
					case 5:
						return fiveStar;
				}
				return {};
			}
		};

		inline const std::unordered_map<Stat, SubStatContainer> subStat{
			{Stat::ap, {3.f, 6.f, 9.f}},
			{Stat::atk, {7.f, 13.f, 19.f}},
			{Stat::atk_, {0.01f, 0.02f, 0.03f}},
			{Stat::cd, {0.016f, 0.032f, 0.048f}},
			{Stat::cr, {0.008f, 0.016f, 0.024f}},
			{Stat::def, {5.f, 10.f, 15.f}},
			{Stat::def_, {0.016f, 0.032f, 0.048f}},
			{Stat::hp, {39.f, 75.f, 112.f}},
			{Stat::hp_, {0.01f, 0.02f, 0.03f}},
			{Stat::pen, {3.f, 6.f, 9.f}}
		};
	}// namespace Values
}// namespace Stats


struct StatValue {
	std::optional<Stat> stat;
	bool activated = true;
	float value;
};

namespace Utils {
	template<>
	[[nodiscard]] constexpr bool isPercentage<::Stat>(const ::Stat &stat) {
		switch (stat) {
			case Stat::hp_:
			case Stat::atk_:
			case Stat::def_:
			case Stat::er_:
			case Stat::ap_:
			case Stat::am_:
			case Stat::cr:
			case Stat::cd:
			case Stat::penRatio:
			case Stat::impact_:
			case Stat::physicalDmg:
			case Stat::fireDmg:
			case Stat::iceDmg:
			case Stat::electricDmg:
			case Stat::etherDmg:
			case Stat::windDmg:
			case Stat::allDmg:
				return true;
			default:
				return false;
		};
	}

	template<>
	[[nodiscard]] constexpr bool isPercentage<>(const std::optional<::Stat> &stat) {
		if (!stat.has_value()) return false;
		return isPercentage(stat.value());
	}

	template<>
	constexpr std::string Stringify<>(const Stat &stat) {
		switch (stat) {
			case Stat::hp:
				return "HP";
			case Stat::hp_:
				return "HP%";
			case Stat::atk:
				return "ATK";
			case Stat::atk_:
				return "ATK%";
			case Stat::def:
				return "DEF";
			case Stat::def_:
				return "DEF%";
			case Stat::sheerForce:
				return "Sheer Force";
			case Stat::er:
				return "Energy Regen";
			case Stat::er_:
				return "Energy Regen%";
			case Stat::ap:
				return "Anomaly Proficiency";
			case Stat::ap_:
				return "Anomaly Proficiency%";
			case Stat::am:
				return "Anomaly Mastery";
			case Stat::am_:
				return "Anomaly Mastery%";
			case Stat::cr:
				return "CRIT Rate";
			case Stat::cd:
				return "CRIT DMG";
			case Stat::pen:
				return "PEN";
			case Stat::penRatio:
				return "PEN Ratio%";
			case Stat::impact:
				return "Impact";
			case Stat::impact_:
				return "Impact%";
			case Stat::physicalDmg:
				return "Physical DMG%";
			case Stat::fireDmg:
				return "Fire DMG%";
			case Stat::iceDmg:
				return "Ice DMG%";
			case Stat::electricDmg:
				return "Electric DMG%";
			case Stat::etherDmg:
				return "Ether DMG%";
			case Stat::windDmg:
				return "Wind DMG%";
			case Stat::allDmg:
				return "All DMG%";
		}
		std::unreachable();
	}

	template<>
	constexpr std::string Stringify<>(const std::optional<Stat> &stat) {
		if (!stat.has_value()) return "None";
		return Stringify(stat.value());
	}

	template<>
	constexpr std::string Stringify<>(const StatValue &stat) {
		if (!stat.stat.has_value()) return "0";
		return Formula::Percentage("", stat.value, Utils::isPercentage(stat.stat));
	}
}// namespace Utils
