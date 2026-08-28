#pragma once

#include "array"
#include "formula/node.hpp"
#include "misc/attackSource.hpp"
#include "misc/attribute.hpp"
#include "misc/skillStat.hpp"
#include "skill.hpp"
#include "stats/helpers.hpp"
#include "utils/optional.hpp"


namespace Formula {
	using AttributeNode = NodeType<Utils::JankyOptional<Misc::Attribute>>;
}

namespace Stats {
	template<class T>
	struct EnemySheet {
		using _Value = T;
		struct _SkillValue {
			_Value physical{};
			_Value fire{};
			_Value ice{};
			_Value electric{};
			_Value ether{};
			_Value wind{};

			[[nodiscard]] static consteval auto getMembers() {
				return std::array{
					&_SkillValue::physical,
					&_SkillValue::fire,
					&_SkillValue::ice,
					&_SkillValue::electric,
					&_SkillValue::ether,
					&_SkillValue::wind,
				};
			}

			[[nodiscard]] constexpr auto fromAttribute(this auto &&self, const Misc::Attribute &attribute) {
				return fromEnemyResAttribute<decltype(self), _Value>(self, attribute);
			}

			[[nodiscard]] static constexpr bool isPercetange(_Value _SkillValue::*) {
				return true;
			}
		};

		_Value level{};
		_Value stunMod{};
		_Value baseDef{};
		_Value DEFReduction{};
		_Value DEFIgnored{};
		_Value dazeRes{};
		_Value daze{};
		_Value dazeTaken{};
		_SkillValue resistance{};
	};

	template<class T>
	struct Sheet {
		using _Value = T;
		using _EnemySheet = EnemySheet<_Value>;
		struct _SkillValue {
			_Value DMG{};
			_Value directDMG{};
			_Value additiveDMG{};
			_Value additiveMultiplier{};
			_Value multiplicativeDMG{};
			_Value critRate{};
			_Value critDMG{};
			_Value penRatio{};
			_EnemySheet enemy{};

			[[nodiscard]] static consteval auto getMembers() {
				return std::array{
					&_SkillValue::DMG,
					&_SkillValue::directDMG,
					&_SkillValue::additiveDMG,
					&_SkillValue::additiveMultiplier,
					&_SkillValue::multiplicativeDMG,
					&_SkillValue::critRate,
					&_SkillValue::critDMG,
					&_SkillValue::penRatio,
				};
			}

			[[nodiscard]] static constexpr bool isPercetange(_Value _SkillValue::*member) {
				if (member == &_SkillValue::DMG) return true;
				if (member == &_SkillValue::directDMG) return true;
				if (member == &_SkillValue::additiveDMG) return false;
				if (member == &_SkillValue::additiveMultiplier) return false;
				if (member == &_SkillValue::multiplicativeDMG) return true;
				if (member == &_SkillValue::critRate) return true;
				if (member == &_SkillValue::critDMG) return true;
				if (member == &_SkillValue::penRatio) return true;
				return false;
			}

			[[nodiscard]] auto &&fromSkillStat(this auto &&self, Misc::SkillStat stat) {
				switch (stat) {
					case Misc::SkillStat::DMG:
						return self.DMG;
					case Misc::SkillStat::directDMG:
						return self.directDMG;
					case Misc::SkillStat::additiveDMG:
						return self.additiveDMG;
					case Misc::SkillStat::additiveMultiplier:
						return self.additiveMultiplier;
					case Misc::SkillStat::multiplicativeDMG:
						return self.multiplicativeDMG;
					case Misc::SkillStat::critRate:
						return self.critRate;
					case Misc::SkillStat::critDMG:
						return self.critDMG;
					case Misc::SkillStat::penRatio:
						return self.penRatio;
				}
				std::unreachable();
			}
		};
		_Value hp{};
		_Value hp_{};
		_Value atk{};
		_Value atk_{};
		_Value def{};
		_Value def_{};
		_Value sheerForce{};
		_Value er{};
		_Value er_{};
		_Value ap{};
		_Value ap_{};
		_Value am{};
		_Value am_{};
		_Value pen{};
		_Value penRatio{};
		_Value impact{};
		_Value impact_{};
		_Value cr{};
		_Value cd{};

		_SkillValue physical{};
		_SkillValue fire{};
		_SkillValue ice{};
		_SkillValue electric{};
		_SkillValue ether{};
		_SkillValue wind{};
		_SkillValue all{};

		_SkillValue basic{};
		_SkillValue dodge{};
		_SkillValue dashAttack{};
		_SkillValue dodgeCounter{};
		_SkillValue assist{};
		_SkillValue quickAssist{};
		_SkillValue defensiveAssist{};
		_SkillValue evasiveAssist{};
		_SkillValue assistFollowup{};
		_SkillValue special{};
		_SkillValue exspecial{};
		_SkillValue chain{};
		_SkillValue ultimate{};

		_SkillValue burn{};
		_SkillValue shock{};
		_SkillValue shatter{};
		_SkillValue assault{};
		_SkillValue corruption{};
		_SkillValue windswept{};
		_SkillValue disorder{};
		_SkillValue burnDisorder{};
		_SkillValue shockDisorder{};
		_SkillValue frozenDisorder{};
		_SkillValue frozenFrostDisorder{};
		_SkillValue assaultDisorder{};
		_SkillValue corruptionDisorder{};
		_SkillValue windsweptDisorder{};
		_SkillValue abloom{};
		_SkillValue vortex{};
		_SkillValue allAnomaly{};

		_SkillValue sheer{};

		[[nodiscard]] auto &fromAttribute(this auto &&self, const Misc::Attribute &attribute) {
			return Stats::fromAttribute(self, attribute);
		}

		[[nodiscard]] auto &&fromDamageAttribute(this auto &&self, const Misc::DamageAttribute &attribute) {
			return Stats::fromDamageAttribute(self, attribute);
		}

		[[nodiscard]] auto &fromStat(this auto &&self, const Stat &stat) {
			return Stats::fromStat(self, stat);
		}

		[[nodiscard]] auto &&fromAttackSource(this auto &&self, const Misc::AttackSource &attackSource) {
			return Stats::fromAttackSource(self, attackSource);
		}
		[[nodiscard]] auto &&fromDamageAnomaly(this auto &&self, const Misc::DamageAnomaly &damageAnomaly) {
			return Stats::fromAnomaly(self, damageAnomaly);
		}
		[[nodiscard]] auto &&fromDamageType(this auto &&self, const Misc::DamageType &damageType) {
			return Stats::fromDamageType(self, damageType);
		}
	};

	template<class T>
	struct SheetSimplified {
		using _Value = T;
		_Value hp{};
		_Value hp_{};
		_Value atk{};
		_Value atk_{};
		_Value def{};
		_Value def_{};
		_Value er_{};
		_Value ap{};
		_Value ap_{};
		_Value am_{};
		_Value pen{};
		_Value penRatio{};
		_Value impact_{};
		_Value cr{};
		_Value cd{};

		_Value physical{};
		_Value fire{};
		_Value ice{};
		_Value electric{};
		_Value ether{};
		_Value wind{};

		[[nodiscard]] auto &fromAttribute(this auto &&self, const Misc::Attribute &attribute) {
			return Stats::fromAttribute(self, attribute);
		}

		[[nodiscard]] auto &fromStat(this auto &&self, const Stat &stat) {
			return Stats::fromStatSimplified(self, stat);
		}
	};

	struct ModsSheet {
		using _Sheet = Stats::Sheet<Formula::FloatNode>;
		using _EnemySheet = Stats::EnemySheet<Formula::FloatNode>;
		using _Skills = Skills<Formula::IntNode>;

		_Sheet base{};
		_Sheet initial{};
		_Sheet combat{};
		_Sheet teamInitial{};
		_Sheet teamCombat{};
		_Sheet activeInitial{};
		_Sheet activeCombat{};

		_EnemySheet enemy{};

		_Skills skills{};
		_Skills teamSkills{};
	};
}// namespace Stats