#pragma once

#include "Talents.hpp"
#include "array"
#include "formula/infusion.hpp"
#include "formula/node.hpp"
#include "stats/helpers.hpp"


namespace Stats {
	template<class T>
	struct EnemySheet {
		using _Value = T;
		struct _SkillValue {
			_Value pyro{};
			_Value hydro{};
			_Value cryo{};
			_Value electro{};
			_Value dendro{};
			_Value anemo{};
			_Value geo{};
			_Value physical{};

			[[nodiscard]] static consteval auto getMembers() {
				return std::array{
					&_SkillValue::pyro,
					&_SkillValue::hydro,
					&_SkillValue::cryo,
					&_SkillValue::electro,
					&_SkillValue::dendro,
					&_SkillValue::anemo,
					&_SkillValue::geo,
					&_SkillValue::physical,
				};
			}

			[[nodiscard]] constexpr auto fromElement(this auto &&self, const Misc::Element &element) {
				return fromEnemyResElement<decltype(self), _Value>(self, element);
			}

			[[nodiscard]] static constexpr bool isPercetange(_Value _SkillValue::*) {
				return true;
			}
		};

		_Value level{};
		_Value DEFReduction{};
		_Value DEFIgnored{};
		_SkillValue resistance{};
	};

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

			[[nodiscard]] static constexpr bool isPercetange(_Value _SkillValue::*member) {
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
		_Value incHb{};

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

		_SkillValue vape{};
		_SkillValue melt{};

		_SkillValue aggravate{};
		_SkillValue spread{};

		_SkillValue burning{};
		_SkillValue superconduct{};
		_SkillValue swirl{};
		_SkillValue electroCharged{};
		_SkillValue shattered{};
		_SkillValue overloaded{};
		_SkillValue bloom{};
		_SkillValue burgeon{};
		_SkillValue hyperbloom{};

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
		using _Sheet = Stats::Sheet<Formula::FloatNode>;
		using _EnemySheet = Stats::EnemySheet<Formula::FloatNode>;
		using _Talents = Talents<Formula::IntNode>;

		_Sheet preMod{};
		_Sheet postMod{};
		_Sheet teamPreMod{};
		_Sheet teamPostMod{};

		_EnemySheet enemy{};

		_Talents talents{};
		_Talents teamTalents{};

		Formula::ElementNode infusion = Formula::NoInfusion{};
		Formula::ElementNode teamInfusion = Formula::NoInfusion{};
	};
}// namespace Stats