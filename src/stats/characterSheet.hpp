#pragma once

#include "Talents.hpp"
#include "misc/attackSource.hpp"
#include "stat.hpp"
#include "value.hpp"// IWYU pragma: keep


namespace Stats {
	struct Sheet;
	struct CharacterSheet {
		using _Value = Value<Sheet, 4>;
		using _SkillValue = SkillValue<_Value>;
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
		_Value er{.constant = 1.f};
		_Value em{};
		_Value cr{.constant = 0.05f};
		_Value cd{.constant = 0.5f};
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

		Talents talents{};
		unsigned short constellation{};
		unsigned short level{1};
		unsigned short ascension{0};

		void init(Stats::Sheet &sheet);

		[[nodiscard]] auto &fromElement(this auto &&self, const Misc::Element &element) {
			return Stats::fromElement(self, element);
		}

		[[nodiscard]] auto &fromStat(this auto &&self, const Stat &stat) {
			return Stats::fromStat(self, stat);
		}

		[[nodiscard]] auto fromAttackSource(this auto &&self, const Misc::AttackSource &attackSource) {
			return Stats::fromAttackSource(self, attackSource);
		}

	private:
		void linkWeaponAndArtifacts(Stats::Sheet &);
	};
}// namespace Stats
