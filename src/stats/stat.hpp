#pragma once

#include "array"
#include "fmt/core.h"
#include "misc/element.hpp"
#include "unordered_map"
#include "utility"
#include "utils/isPercentage.hpp"// IWYU pragma: keep
#include "utils/stringify.hpp"   // IWYU pragma: keep
#include <optional>
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

	shield_,
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
			Stat::shield_,
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
			Stat::shield_,
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
		static inline const std::unordered_map<Stat, std::array<std::array<float, 21>, 5>> mainStat{
			{Stat::hp, std::array{std::array{129.000f, 178.000f, 227.000f, 275.000f, 324.000f, 373.000f, 422.000f, 470.000f, 519.000f, 568.000f, 617.000f, 665.000f, 714.000f, 763.000f, 812.000f, 860.000f, 0.000f, 0.000f, 0.000f, 0.000f, 0.000f}, std::array{258.000f, 331.000f, 404.000f, 478.000f, 551.000f, 624.000f, 697.000f, 770.000f, 843.000f, 916.000f, 989.000f, 1063.000f, 1136.000f, 1209.000f, 1282.000f, 1355.000f, 1428.000f, 1501.000f, 1575.000f, 1648.000f, 1721.000f}, std::array{430.000f, 552.000f, 674.000f, 796.000f, 918.000f, 1040.000f, 1162.000f, 1283.000f, 1405.000f, 1527.000f, 1649.000f, 1771.000f, 1893.000f, 2015.000f, 2137.000f, 2259.000f, 2380.000f, 2502.000f, 2624.000f, 2746.000f, 2868.000f}, std::array{645.000f, 828.000f, 1011.000f, 1194.000f, 1377.000f, 1559.000f, 1742.000f, 1925.000f, 2108.000f, 2291.000f, 2474.000f, 2657.000f, 2839.000f, 3022.000f, 3205.000f, 3388.000f, 3571.000f, 3754.000f, 3936.000f, 4119.000f, 4302.000f}, std::array{717.000f, 920.000f, 1123.000f, 1326.000f, 1530.000f, 1733.000f, 1936.000f, 2139.000f, 2342.000f, 2545.000f, 2749.000f, 2952.000f, 3155.000f, 3358.000f, 3561.000f, 3764.000f, 3967.000f, 4171.000f, 4374.000f, 4577.000f, 4780.000f}}},
			{Stat::atk, std::array{std::array{8.000f, 12.000f, 15.000f, 18.000f, 21.000f, 24.000f, 27.000f, 31.000f, 34.000f, 37.000f, 40.000f, 43.000f, 46.000f, 50.000f, 53.000f, 56.000f, 0.000f, 0.000f, 0.000f, 0.000f, 0.000f}, std::array{17.000f, 22.000f, 26.000f, 31.000f, 36.000f, 41.000f, 45.000f, 50.000f, 55.000f, 60.000f, 64.000f, 69.000f, 74.000f, 79.000f, 83.000f, 88.000f, 93.000f, 98.000f, 103.000f, 107.000f, 112.000f}, std::array{28.000f, 36.000f, 44.000f, 52.000f, 60.000f, 68.000f, 76.000f, 84.000f, 91.000f, 99.000f, 107.000f, 115.000f, 123.000f, 131.000f, 139.000f, 147.000f, 155.000f, 163.000f, 171.000f, 179.000f, 187.000f}, std::array{42.000f, 54.000f, 66.000f, 78.000f, 90.000f, 102.000f, 113.000f, 125.000f, 137.000f, 149.000f, 161.000f, 173.000f, 185.000f, 197.000f, 209.000f, 221.000f, 232.000f, 244.000f, 256.000f, 268.000f, 280.000f}, std::array{47.000f, 60.000f, 73.000f, 86.000f, 100.000f, 113.000f, 126.000f, 139.000f, 152.000f, 166.000f, 179.000f, 192.000f, 205.000f, 219.000f, 232.000f, 245.000f, 258.000f, 272.000f, 285.000f, 298.000f, 311.000f}}},
			{Stat::def, std::array{std::array{20.000f, 28.000f, 35.000f, 43.000f, 50.000f, 58.000f, 65.000f, 73.000f, 80.000f, 88.000f, 96.000f, 103.000f, 111.000f, 118.000f, 126.000f, 133.000f, 0.000f, 0.000f, 0.000f, 0.000f, 0.000f}, std::array{40.000f, 51.000f, 63.000f, 74.000f, 85.000f, 97.000f, 108.000f, 119.000f, 131.000f, 142.000f, 153.000f, 165.000f, 176.000f, 187.000f, 199.000f, 210.000f, 221.000f, 233.000f, 244.000f, 255.000f, 267.000f}, std::array{67.000f, 86.000f, 104.000f, 123.000f, 142.000f, 161.000f, 180.000f, 199.000f, 218.000f, 237.000f, 256.000f, 274.000f, 293.000f, 312.000f, 331.000f, 350.000f, 369.000f, 388.000f, 407.000f, 426.000f, 444.000f}, std::array{100.000f, 128.000f, 157.000f, 185.000f, 213.000f, 242.000f, 270.000f, 298.000f, 327.000f, 355.000f, 383.000f, 412.000f, 440.000f, 468.000f, 497.000f, 525.000f, 553.000f, 582.000f, 610.000f, 638.000f, 667.000f}, std::array{111.000f, 143.000f, 174.000f, 206.000f, 237.000f, 269.000f, 300.000f, 331.000f, 363.000f, 394.000f, 426.000f, 457.000f, 489.000f, 520.000f, 552.000f, 583.000f, 615.000f, 646.000f, 678.000f, 709.000f, 741.000f}}},
			{Stat::hp_, std::array{std::array{0.031f, 0.043f, 0.055f, 0.067f, 0.079f, 0.091f, 0.103f, 0.115f, 0.127f, 0.138f, 0.150f, 0.162f, 0.174f, 0.186f, 0.198f, 0.210f, 0.000f, 0.000f, 0.000f, 0.000f, 0.000f}, std::array{0.042f, 0.054f, 0.066f, 0.078f, 0.090f, 0.101f, 0.113f, 0.125f, 0.137f, 0.149f, 0.161f, 0.173f, 0.185f, 0.197f, 0.208f, 0.220f, 0.232f, 0.244f, 0.256f, 0.268f, 0.280f}, std::array{0.052f, 0.067f, 0.082f, 0.097f, 0.112f, 0.127f, 0.142f, 0.156f, 0.171f, 0.186f, 0.201f, 0.216f, 0.231f, 0.246f, 0.261f, 0.275f, 0.290f, 0.305f, 0.320f, 0.335f, 0.350f}, std::array{0.063f, 0.081f, 0.099f, 0.116f, 0.134f, 0.152f, 0.170f, 0.188f, 0.206f, 0.223f, 0.241f, 0.259f, 0.277f, 0.295f, 0.313f, 0.330f, 0.348f, 0.366f, 0.384f, 0.402f, 0.420f}, std::array{0.070f, 0.090f, 0.110f, 0.129f, 0.149f, 0.169f, 0.189f, 0.209f, 0.228f, 0.248f, 0.268f, 0.288f, 0.308f, 0.328f, 0.347f, 0.367f, 0.387f, 0.407f, 0.427f, 0.446f, 0.466f}}},
			{Stat::atk_, std::array{std::array{0.031f, 0.043f, 0.055f, 0.067f, 0.079f, 0.091f, 0.103f, 0.115f, 0.127f, 0.138f, 0.150f, 0.162f, 0.174f, 0.186f, 0.198f, 0.210f, 0.000f, 0.000f, 0.000f, 0.000f, 0.000f}, std::array{0.042f, 0.054f, 0.066f, 0.078f, 0.090f, 0.101f, 0.113f, 0.125f, 0.137f, 0.149f, 0.161f, 0.173f, 0.185f, 0.197f, 0.208f, 0.220f, 0.232f, 0.244f, 0.256f, 0.268f, 0.280f}, std::array{0.052f, 0.067f, 0.082f, 0.097f, 0.112f, 0.127f, 0.142f, 0.156f, 0.171f, 0.186f, 0.201f, 0.216f, 0.231f, 0.246f, 0.261f, 0.275f, 0.290f, 0.305f, 0.320f, 0.335f, 0.350f}, std::array{0.063f, 0.081f, 0.099f, 0.116f, 0.134f, 0.152f, 0.170f, 0.188f, 0.206f, 0.223f, 0.241f, 0.259f, 0.277f, 0.295f, 0.313f, 0.330f, 0.348f, 0.366f, 0.384f, 0.402f, 0.420f}, std::array{0.070f, 0.090f, 0.110f, 0.129f, 0.149f, 0.169f, 0.189f, 0.209f, 0.228f, 0.248f, 0.268f, 0.288f, 0.308f, 0.328f, 0.347f, 0.367f, 0.387f, 0.407f, 0.427f, 0.446f, 0.466f}}},
			{Stat::def_, std::array{std::array{0.039f, 0.054f, 0.069f, 0.084f, 0.099f, 0.114f, 0.129f, 0.143f, 0.158f, 0.173f, 0.188f, 0.203f, 0.218f, 0.233f, 0.247f, 0.262f, 0.000f, 0.000f, 0.000f, 0.000f, 0.000f}, std::array{0.052f, 0.067f, 0.082f, 0.097f, 0.112f, 0.127f, 0.142f, 0.156f, 0.171f, 0.186f, 0.201f, 0.216f, 0.231f, 0.246f, 0.261f, 0.275f, 0.290f, 0.305f, 0.320f, 0.335f, 0.350f}, std::array{0.066f, 0.084f, 0.103f, 0.121f, 0.140f, 0.158f, 0.177f, 0.196f, 0.214f, 0.233f, 0.251f, 0.270f, 0.288f, 0.307f, 0.326f, 0.344f, 0.363f, 0.381f, 0.400f, 0.419f, 0.437f}, std::array{0.079f, 0.101f, 0.123f, 0.146f, 0.168f, 0.190f, 0.212f, 0.235f, 0.257f, 0.279f, 0.302f, 0.324f, 0.346f, 0.368f, 0.391f, 0.413f, 0.435f, 0.458f, 0.480f, 0.502f, 0.525f}, std::array{0.087f, 0.112f, 0.137f, 0.162f, 0.186f, 0.211f, 0.236f, 0.261f, 0.286f, 0.310f, 0.335f, 0.360f, 0.385f, 0.409f, 0.434f, 0.459f, 0.484f, 0.508f, 0.533f, 0.558f, 0.583f}}},
			{Stat::em, std::array{std::array{12.600f, 17.300f, 22.100f, 26.900f, 31.600f, 36.400f, 41.100f, 45.900f, 50.600f, 55.400f, 60.100f, 64.900f, 69.700f, 74.400f, 79.200f, 83.900f, 0.000f, 0.000f, 0.000f, 0.000f, 0.000f}, std::array{16.800f, 21.500f, 26.300f, 31.100f, 35.800f, 40.600f, 45.300f, 50.100f, 54.800f, 59.600f, 64.300f, 69.100f, 73.900f, 78.600f, 83.400f, 88.100f, 92.900f, 97.600f, 102.400f, 107.100f, 111.900f}, std::array{21.000f, 26.900f, 32.900f, 38.800f, 44.800f, 50.700f, 56.700f, 62.600f, 68.500f, 74.500f, 80.400f, 86.400f, 92.300f, 98.300f, 104.200f, 110.200f, 116.100f, 122.000f, 128.000f, 133.900f, 139.900f}, std::array{25.200f, 32.300f, 39.400f, 46.600f, 53.700f, 60.800f, 68.000f, 75.100f, 82.200f, 89.400f, 96.500f, 103.600f, 110.800f, 117.900f, 125.000f, 132.200f, 139.300f, 146.400f, 153.600f, 160.700f, 167.900f}, std::array{28.000f, 35.900f, 43.800f, 51.800f, 59.700f, 67.600f, 75.500f, 83.500f, 91.400f, 99.300f, 107.200f, 115.200f, 123.100f, 131.000f, 138.900f, 146.900f, 154.800f, 162.700f, 170.600f, 178.600f, 186.500f}}},
			{Stat::er, std::array{std::array{0.035f, 0.048f, 0.061f, 0.075f, 0.088f, 0.101f, 0.114f, 0.127f, 0.141f, 0.154f, 0.167f, 0.180f, 0.193f, 0.207f, 0.220f, 0.233f, 0.000f, 0.000f, 0.000f, 0.000f, 0.000f}, std::array{0.047f, 0.060f, 0.073f, 0.086f, 0.099f, 0.113f, 0.126f, 0.139f, 0.152f, 0.166f, 0.179f, 0.192f, 0.205f, 0.218f, 0.232f, 0.245f, 0.258f, 0.271f, 0.284f, 0.298f, 0.311f}, std::array{0.058f, 0.075f, 0.091f, 0.108f, 0.124f, 0.141f, 0.157f, 0.174f, 0.190f, 0.207f, 0.223f, 0.240f, 0.256f, 0.273f, 0.289f, 0.306f, 0.322f, 0.339f, 0.355f, 0.372f, 0.389f}, std::array{0.070f, 0.090f, 0.110f, 0.129f, 0.149f, 0.169f, 0.189f, 0.209f, 0.228f, 0.248f, 0.268f, 0.288f, 0.308f, 0.328f, 0.347f, 0.367f, 0.387f, 0.407f, 0.427f, 0.446f, 0.466f}, std::array{0.078f, 0.100f, 0.122f, 0.144f, 0.166f, 0.188f, 0.210f, 0.232f, 0.254f, 0.276f, 0.298f, 0.320f, 0.342f, 0.364f, 0.386f, 0.408f, 0.430f, 0.452f, 0.474f, 0.496f, 0.518f}}},
			{Stat::hb, std::array{std::array{0.024f, 0.033f, 0.043f, 0.052f, 0.061f, 0.070f, 0.079f, 0.088f, 0.097f, 0.107f, 0.116f, 0.125f, 0.134f, 0.143f, 0.152f, 0.161f, 0.000f, 0.000f, 0.000f, 0.000f, 0.000f}, std::array{0.032f, 0.041f, 0.051f, 0.060f, 0.069f, 0.078f, 0.087f, 0.096f, 0.105f, 0.115f, 0.124f, 0.133f, 0.142f, 0.151f, 0.160f, 0.169f, 0.179f, 0.188f, 0.197f, 0.206f, 0.215f}, std::array{0.040f, 0.052f, 0.063f, 0.075f, 0.086f, 0.098f, 0.109f, 0.120f, 0.132f, 0.143f, 0.155f, 0.166f, 0.178f, 0.189f, 0.200f, 0.212f, 0.223f, 0.235f, 0.246f, 0.258f, 0.269f}, std::array{0.048f, 0.062f, 0.076f, 0.090f, 0.103f, 0.117f, 0.131f, 0.144f, 0.158f, 0.172f, 0.186f, 0.199f, 0.213f, 0.227f, 0.240f, 0.254f, 0.268f, 0.282f, 0.295f, 0.309f, 0.323f}, std::array{0.054f, 0.069f, 0.084f, 0.100f, 0.115f, 0.130f, 0.145f, 0.161f, 0.176f, 0.191f, 0.206f, 0.221f, 0.237f, 0.252f, 0.267f, 0.282f, 0.298f, 0.313f, 0.328f, 0.343f, 0.359f}}},
			{Stat::cd, std::array{std::array{0.042f, 0.058f, 0.074f, 0.090f, 0.105f, 0.121f, 0.137f, 0.153f, 0.169f, 0.185f, 0.201f, 0.216f, 0.232f, 0.248f, 0.264f, 0.280f, 0.000f, 0.000f, 0.000f, 0.000f, 0.000f}, std::array{0.056f, 0.072f, 0.088f, 0.104f, 0.119f, 0.135f, 0.151f, 0.167f, 0.183f, 0.199f, 0.214f, 0.230f, 0.246f, 0.262f, 0.278f, 0.294f, 0.310f, 0.325f, 0.341f, 0.357f, 0.373f}, std::array{0.070f, 0.090f, 0.110f, 0.129f, 0.149f, 0.169f, 0.189f, 0.209f, 0.228f, 0.248f, 0.268f, 0.288f, 0.308f, 0.328f, 0.347f, 0.367f, 0.387f, 0.407f, 0.427f, 0.446f, 0.466f}, std::array{0.084f, 0.108f, 0.131f, 0.155f, 0.179f, 0.203f, 0.227f, 0.250f, 0.274f, 0.298f, 0.322f, 0.345f, 0.369f, 0.393f, 0.417f, 0.441f, 0.464f, 0.488f, 0.512f, 0.536f, 0.560f}, std::array{0.093f, 0.120f, 0.146f, 0.173f, 0.199f, 0.225f, 0.252f, 0.278f, 0.305f, 0.331f, 0.357f, 0.384f, 0.410f, 0.437f, 0.463f, 0.490f, 0.516f, 0.542f, 0.569f, 0.595f, 0.622f}}},
			{Stat::cr, std::array{std::array{0.021f, 0.029f, 0.037f, 0.045f, 0.053f, 0.061f, 0.069f, 0.076f, 0.084f, 0.092f, 0.100f, 0.108f, 0.116f, 0.124f, 0.132f, 0.140f, 0.000f, 0.000f, 0.000f, 0.000f, 0.000f}, std::array{0.028f, 0.036f, 0.044f, 0.052f, 0.060f, 0.068f, 0.076f, 0.083f, 0.091f, 0.099f, 0.107f, 0.115f, 0.123f, 0.131f, 0.139f, 0.147f, 0.155f, 0.163f, 0.171f, 0.179f, 0.187f}, std::array{0.035f, 0.045f, 0.055f, 0.065f, 0.075f, 0.084f, 0.094f, 0.104f, 0.114f, 0.124f, 0.134f, 0.144f, 0.154f, 0.164f, 0.174f, 0.184f, 0.193f, 0.203f, 0.213f, 0.223f, 0.233f}, std::array{0.042f, 0.054f, 0.066f, 0.078f, 0.090f, 0.101f, 0.113f, 0.125f, 0.137f, 0.149f, 0.161f, 0.173f, 0.185f, 0.197f, 0.208f, 0.220f, 0.232f, 0.244f, 0.256f, 0.268f, 0.280f}, std::array{0.047f, 0.060f, 0.073f, 0.086f, 0.099f, 0.113f, 0.126f, 0.139f, 0.152f, 0.166f, 0.179f, 0.192f, 0.205f, 0.218f, 0.232f, 0.245f, 0.258f, 0.271f, 0.284f, 0.298f, 0.311f}}},
			{Stat::physicalDmg, std::array{std::array{0.039f, 0.054f, 0.069f, 0.084f, 0.099f, 0.114f, 0.129f, 0.143f, 0.158f, 0.173f, 0.188f, 0.203f, 0.218f, 0.233f, 0.247f, 0.262f, 0.000f, 0.000f, 0.000f, 0.000f, 0.000f}, std::array{0.052f, 0.067f, 0.082f, 0.097f, 0.112f, 0.127f, 0.142f, 0.156f, 0.171f, 0.186f, 0.201f, 0.216f, 0.231f, 0.246f, 0.261f, 0.275f, 0.290f, 0.305f, 0.320f, 0.335f, 0.350f}, std::array{0.066f, 0.084f, 0.103f, 0.121f, 0.140f, 0.158f, 0.177f, 0.196f, 0.214f, 0.233f, 0.251f, 0.270f, 0.288f, 0.307f, 0.326f, 0.344f, 0.363f, 0.381f, 0.400f, 0.419f, 0.437f}, std::array{0.079f, 0.101f, 0.123f, 0.146f, 0.168f, 0.190f, 0.212f, 0.235f, 0.257f, 0.279f, 0.302f, 0.324f, 0.346f, 0.368f, 0.391f, 0.413f, 0.435f, 0.458f, 0.480f, 0.502f, 0.525f}, std::array{0.087f, 0.112f, 0.137f, 0.162f, 0.186f, 0.211f, 0.236f, 0.261f, 0.286f, 0.310f, 0.335f, 0.360f, 0.385f, 0.409f, 0.434f, 0.459f, 0.484f, 0.508f, 0.533f, 0.558f, 0.583f}}},
			{Stat::dendroDmg, std::array{std::array{0.031f, 0.043f, 0.055f, 0.067f, 0.079f, 0.091f, 0.103f, 0.115f, 0.127f, 0.138f, 0.150f, 0.162f, 0.174f, 0.186f, 0.198f, 0.210f, 0.000f, 0.000f, 0.000f, 0.000f, 0.000f}, std::array{0.042f, 0.054f, 0.066f, 0.078f, 0.090f, 0.101f, 0.113f, 0.125f, 0.137f, 0.149f, 0.161f, 0.173f, 0.185f, 0.197f, 0.208f, 0.220f, 0.232f, 0.244f, 0.256f, 0.268f, 0.280f}, std::array{0.052f, 0.067f, 0.082f, 0.097f, 0.112f, 0.127f, 0.142f, 0.156f, 0.171f, 0.186f, 0.201f, 0.216f, 0.231f, 0.246f, 0.261f, 0.275f, 0.290f, 0.305f, 0.320f, 0.335f, 0.350f}, std::array{0.063f, 0.081f, 0.099f, 0.116f, 0.134f, 0.152f, 0.170f, 0.188f, 0.206f, 0.223f, 0.241f, 0.259f, 0.277f, 0.295f, 0.313f, 0.330f, 0.348f, 0.366f, 0.384f, 0.402f, 0.420f}, std::array{0.070f, 0.090f, 0.110f, 0.129f, 0.149f, 0.169f, 0.189f, 0.209f, 0.228f, 0.248f, 0.268f, 0.288f, 0.308f, 0.328f, 0.347f, 0.367f, 0.387f, 0.407f, 0.427f, 0.446f, 0.466f}}},
			{Stat::geoDmg, std::array{std::array{0.031f, 0.043f, 0.055f, 0.067f, 0.079f, 0.091f, 0.103f, 0.115f, 0.127f, 0.138f, 0.150f, 0.162f, 0.174f, 0.186f, 0.198f, 0.210f, 0.000f, 0.000f, 0.000f, 0.000f, 0.000f}, std::array{0.042f, 0.054f, 0.066f, 0.078f, 0.090f, 0.101f, 0.113f, 0.125f, 0.137f, 0.149f, 0.161f, 0.173f, 0.185f, 0.197f, 0.208f, 0.220f, 0.232f, 0.244f, 0.256f, 0.268f, 0.280f}, std::array{0.052f, 0.067f, 0.082f, 0.097f, 0.112f, 0.127f, 0.142f, 0.156f, 0.171f, 0.186f, 0.201f, 0.216f, 0.231f, 0.246f, 0.261f, 0.275f, 0.290f, 0.305f, 0.320f, 0.335f, 0.350f}, std::array{0.063f, 0.081f, 0.099f, 0.116f, 0.134f, 0.152f, 0.170f, 0.188f, 0.206f, 0.223f, 0.241f, 0.259f, 0.277f, 0.295f, 0.313f, 0.330f, 0.348f, 0.366f, 0.384f, 0.402f, 0.420f}, std::array{0.070f, 0.090f, 0.110f, 0.129f, 0.149f, 0.169f, 0.189f, 0.209f, 0.228f, 0.248f, 0.268f, 0.288f, 0.308f, 0.328f, 0.347f, 0.367f, 0.387f, 0.407f, 0.427f, 0.446f, 0.466f}}},
			{Stat::anemoDmg, std::array{std::array{0.031f, 0.043f, 0.055f, 0.067f, 0.079f, 0.091f, 0.103f, 0.115f, 0.127f, 0.138f, 0.150f, 0.162f, 0.174f, 0.186f, 0.198f, 0.210f, 0.000f, 0.000f, 0.000f, 0.000f, 0.000f}, std::array{0.042f, 0.054f, 0.066f, 0.078f, 0.090f, 0.101f, 0.113f, 0.125f, 0.137f, 0.149f, 0.161f, 0.173f, 0.185f, 0.197f, 0.208f, 0.220f, 0.232f, 0.244f, 0.256f, 0.268f, 0.280f}, std::array{0.052f, 0.067f, 0.082f, 0.097f, 0.112f, 0.127f, 0.142f, 0.156f, 0.171f, 0.186f, 0.201f, 0.216f, 0.231f, 0.246f, 0.261f, 0.275f, 0.290f, 0.305f, 0.320f, 0.335f, 0.350f}, std::array{0.063f, 0.081f, 0.099f, 0.116f, 0.134f, 0.152f, 0.170f, 0.188f, 0.206f, 0.223f, 0.241f, 0.259f, 0.277f, 0.295f, 0.313f, 0.330f, 0.348f, 0.366f, 0.384f, 0.402f, 0.420f}, std::array{0.070f, 0.090f, 0.110f, 0.129f, 0.149f, 0.169f, 0.189f, 0.209f, 0.228f, 0.248f, 0.268f, 0.288f, 0.308f, 0.328f, 0.347f, 0.367f, 0.387f, 0.407f, 0.427f, 0.446f, 0.466f}}},
			{Stat::hydroDmg, std::array{std::array{0.031f, 0.043f, 0.055f, 0.067f, 0.079f, 0.091f, 0.103f, 0.115f, 0.127f, 0.138f, 0.150f, 0.162f, 0.174f, 0.186f, 0.198f, 0.210f, 0.000f, 0.000f, 0.000f, 0.000f, 0.000f}, std::array{0.042f, 0.054f, 0.066f, 0.078f, 0.090f, 0.101f, 0.113f, 0.125f, 0.137f, 0.149f, 0.161f, 0.173f, 0.185f, 0.197f, 0.208f, 0.220f, 0.232f, 0.244f, 0.256f, 0.268f, 0.280f}, std::array{0.052f, 0.067f, 0.082f, 0.097f, 0.112f, 0.127f, 0.142f, 0.156f, 0.171f, 0.186f, 0.201f, 0.216f, 0.231f, 0.246f, 0.261f, 0.275f, 0.290f, 0.305f, 0.320f, 0.335f, 0.350f}, std::array{0.063f, 0.081f, 0.099f, 0.116f, 0.134f, 0.152f, 0.170f, 0.188f, 0.206f, 0.223f, 0.241f, 0.259f, 0.277f, 0.295f, 0.313f, 0.330f, 0.348f, 0.366f, 0.384f, 0.402f, 0.420f}, std::array{0.070f, 0.090f, 0.110f, 0.129f, 0.149f, 0.169f, 0.189f, 0.209f, 0.228f, 0.248f, 0.268f, 0.288f, 0.308f, 0.328f, 0.347f, 0.367f, 0.387f, 0.407f, 0.427f, 0.446f, 0.466f}}},
			{Stat::cryoDmg, std::array{std::array{0.031f, 0.043f, 0.055f, 0.067f, 0.079f, 0.091f, 0.103f, 0.115f, 0.127f, 0.138f, 0.150f, 0.162f, 0.174f, 0.186f, 0.198f, 0.210f, 0.000f, 0.000f, 0.000f, 0.000f, 0.000f}, std::array{0.042f, 0.054f, 0.066f, 0.078f, 0.090f, 0.101f, 0.113f, 0.125f, 0.137f, 0.149f, 0.161f, 0.173f, 0.185f, 0.197f, 0.208f, 0.220f, 0.232f, 0.244f, 0.256f, 0.268f, 0.280f}, std::array{0.052f, 0.067f, 0.082f, 0.097f, 0.112f, 0.127f, 0.142f, 0.156f, 0.171f, 0.186f, 0.201f, 0.216f, 0.231f, 0.246f, 0.261f, 0.275f, 0.290f, 0.305f, 0.320f, 0.335f, 0.350f}, std::array{0.063f, 0.081f, 0.099f, 0.116f, 0.134f, 0.152f, 0.170f, 0.188f, 0.206f, 0.223f, 0.241f, 0.259f, 0.277f, 0.295f, 0.313f, 0.330f, 0.348f, 0.366f, 0.384f, 0.402f, 0.420f}, std::array{0.070f, 0.090f, 0.110f, 0.129f, 0.149f, 0.169f, 0.189f, 0.209f, 0.228f, 0.248f, 0.268f, 0.288f, 0.308f, 0.328f, 0.347f, 0.367f, 0.387f, 0.407f, 0.427f, 0.446f, 0.466f}}},
			{Stat::electroDmg, std::array{std::array{0.031f, 0.043f, 0.055f, 0.067f, 0.079f, 0.091f, 0.103f, 0.115f, 0.127f, 0.138f, 0.150f, 0.162f, 0.174f, 0.186f, 0.198f, 0.210f, 0.000f, 0.000f, 0.000f, 0.000f, 0.000f}, std::array{0.042f, 0.054f, 0.066f, 0.078f, 0.090f, 0.101f, 0.113f, 0.125f, 0.137f, 0.149f, 0.161f, 0.173f, 0.185f, 0.197f, 0.208f, 0.220f, 0.232f, 0.244f, 0.256f, 0.268f, 0.280f}, std::array{0.052f, 0.067f, 0.082f, 0.097f, 0.112f, 0.127f, 0.142f, 0.156f, 0.171f, 0.186f, 0.201f, 0.216f, 0.231f, 0.246f, 0.261f, 0.275f, 0.290f, 0.305f, 0.320f, 0.335f, 0.350f}, std::array{0.063f, 0.081f, 0.099f, 0.116f, 0.134f, 0.152f, 0.170f, 0.188f, 0.206f, 0.223f, 0.241f, 0.259f, 0.277f, 0.295f, 0.313f, 0.330f, 0.348f, 0.366f, 0.384f, 0.402f, 0.420f}, std::array{0.070f, 0.090f, 0.110f, 0.129f, 0.149f, 0.169f, 0.189f, 0.209f, 0.228f, 0.248f, 0.268f, 0.288f, 0.308f, 0.328f, 0.347f, 0.367f, 0.387f, 0.407f, 0.427f, 0.446f, 0.466f}}},
			{Stat::pyroDmg, std::array{std::array{0.031f, 0.043f, 0.055f, 0.067f, 0.079f, 0.091f, 0.103f, 0.115f, 0.127f, 0.138f, 0.150f, 0.162f, 0.174f, 0.186f, 0.198f, 0.210f, 0.000f, 0.000f, 0.000f, 0.000f, 0.000f}, std::array{0.042f, 0.054f, 0.066f, 0.078f, 0.090f, 0.101f, 0.113f, 0.125f, 0.137f, 0.149f, 0.161f, 0.173f, 0.185f, 0.197f, 0.208f, 0.220f, 0.232f, 0.244f, 0.256f, 0.268f, 0.280f}, std::array{0.052f, 0.067f, 0.082f, 0.097f, 0.112f, 0.127f, 0.142f, 0.156f, 0.171f, 0.186f, 0.201f, 0.216f, 0.231f, 0.246f, 0.261f, 0.275f, 0.290f, 0.305f, 0.320f, 0.335f, 0.350f}, std::array{0.063f, 0.081f, 0.099f, 0.116f, 0.134f, 0.152f, 0.170f, 0.188f, 0.206f, 0.223f, 0.241f, 0.259f, 0.277f, 0.295f, 0.313f, 0.330f, 0.348f, 0.366f, 0.384f, 0.402f, 0.420f}, std::array{0.070f, 0.090f, 0.110f, 0.129f, 0.149f, 0.169f, 0.189f, 0.209f, 0.228f, 0.248f, 0.268f, 0.288f, 0.308f, 0.328f, 0.347f, 0.367f, 0.387f, 0.407f, 0.427f, 0.446f, 0.466f}}},
		};
	}// namespace Values
}// namespace Stats


struct StatValue {
	std::optional<Stat> stat;
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
			case Stat::shield_:
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
			case Stat::shield_:
				return "Shield Strength%";
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
	constexpr std::string Stringify<>(const std::optional<Stat> &stat) {
		if (!stat.has_value()) return "None";
		return Stringify(stat.value());
	}

	template<>
	constexpr std::string Stringify<>(const StatValue &stat) {
		if (!stat.stat.has_value()) return "0";
		if (Utils::isPercentage(stat.stat)) {
			return fmt::format("{:.1f}%", stat.value * 100.f);
		}
		return fmt::format("{:.0f}", stat.value);
	}
}// namespace Utils
