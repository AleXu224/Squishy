#pragma once

#include "array"
#include "formula/infusion.hpp"
#include "formula/node.hpp"
#include "stats/helpers.hpp"


namespace Stats {
	template<class T>
	struct Sheet {
		using _Value = T;
		struct _SkillValue {
			_Value DMG{};
			_Value additiveDMG{};
			_Value multiplicativeDMG{};
			_Value critRate{};
			_Value critDMG{};

			[[nodiscard]] static consteval auto getMembers() {
				return std::array{
					&_SkillValue::DMG,
					&_SkillValue::additiveDMG,
					&_SkillValue::multiplicativeDMG,
					&_SkillValue::critRate,
					&_SkillValue::critDMG,
				};
			}

			[[nodiscard]] static constexpr bool isPercetange(_Value _SkillValue:: *member) {
				if (member == &_SkillValue::DMG) return true;
				if (member == &_SkillValue::additiveDMG) return false;
				if (member == &_SkillValue::multiplicativeDMG) return true;
				if (member == &_SkillValue::critRate) return true;
				if (member == &_SkillValue::critDMG) return true;
				return false;
			}
		};
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

	struct ModsSheet {
		using _Sheet = Stats::Sheet<Formula::Node>;

		_Sheet preMod{};
		_Sheet postMod{};
		_Sheet teamPreMod{};
		_Sheet teamPostMod{};

		Formula::ElementNode infusion = Formula::NoInfusion{};
		Formula::ElementNode teamInfusion = Formula::NoInfusion{};
	};
}// namespace Stats