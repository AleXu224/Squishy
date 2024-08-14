#pragma once

#include "stats/helpers.hpp"
#include "value.hpp"// IWYU pragma: keep
#include "weaponBase.hpp"


namespace Stats {
	struct Sheet;
	struct Weapon;
	struct WeaponSheet {
		using _Value = Value<Sheet, 3>;
		using _SkillValue = SkillValue<_Value>;
		unsigned short level{1};
		uint8_t ascension{0};
		unsigned short refinement{0};

		[[nodiscard]] float getBaseAtk();
		[[nodiscard]] float getSubstat();

		_Value hp{};
		_Value hp_{};
		_Value baseHp{};
		_Value atk{};
		_Value atk_{};
		_Value baseAtk{};
		_Value additionalAtk{};
		_Value def{};
		_Value def_{};
		_Value baseDef{};
		_Value er{};
		_Value em{};
		_Value cr{};
		_Value cd{};
		_Value hb{};

		_SkillValue pyro{};
		_SkillValue hydro{};
		_SkillValue cryo{};
		_SkillValue electro{};
		_SkillValue dendro{};
		_SkillValue anemo{};
		_SkillValue geo{};
		_SkillValue physical{};
		_SkillValue all{};

		_SkillValue normal{};
		_SkillValue charged{};
		_SkillValue plunge{};
		_SkillValue skill{};
		_SkillValue burst{};

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
