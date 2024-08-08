#pragma once

#include "stats/helpers.hpp"
#include "value.hpp"
#include "weaponBase.hpp"


namespace Stats {
	struct Weapon;
	struct WeaponSheet {
		unsigned short level{1};
		uint8_t ascension{0};
		unsigned short refinement{0};

		Stats::Value<Stats::Weapon> baseAtkInt{};
		Stats::Value<Stats::Weapon> subStat{};

		SV hp{};
		SV hp_{};
		SV baseHp{};
		SV atk{};
		SV atk_{};
		SV baseAtk{};
		SV additionalAtk{};
		SV def{};
		SV def_{};
		SV baseDef{};
		SV er{};
		SV em{};
		SV cr{};
		SV cd{};
		SV hb{};

		SSV pyro{};
		SSV hydro{};
		SSV cryo{};
		SSV electro{};
		SSV dendro{};
		SSV anemo{};
		SSV geo{};
		SSV physical{};
		SSV all{};

		SSV normal{};
		SSV charged{};
		SSV plunge{};
		SSV skill{};
		SSV burst{};

		explicit WeaponSheet(const WeaponBase &base);

		[[nodiscard]] auto &fromElement(this auto &&self, const Misc::Element &element) {
			return Stats::fromElement(self, element);
		}

		[[nodiscard]] auto &fromStat(this auto &&self, const Stat &stat) {
			return Stats::fromStat(self, stat);
		}

		[[nodiscard]] auto fromAttackSource(this auto &&self, const Misc::AttackSource &attackSource) {
			return Stats::fromAttackSource(self, attackSource);
		}
	};
}// namespace Stats
