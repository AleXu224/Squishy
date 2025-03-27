#pragma once

#include "Talents.hpp"
#include "formula/formulaContext.hpp"
#include "misc/attackSource.hpp"
#include "misc/damageElement.hpp"
#include "misc/enemyProps.hpp"
#include "misc/reaction.hpp"
#include "misc/skillStat.hpp"
#include "stats/stat.hpp"
#include "utility"
#include <functional>


namespace Modifiers {
	template<class T>
	using StatType = T::_Value T::*;
	template<class T>
	using SkillType = std::pair<typename T::_SkillValue T::*, typename T::_Value T::_SkillValue::*>;

	template<class T>
	using TalentType = T::Type T::*;

	template<class T>
	struct SheetMember {
		using RetType = T::_Value;
		enum class Type : uint8_t {
			stat,
			skill,
		} _type;
		union _Uni {
			StatType<T> stat;
			SkillType<T> skill;
		} _uni;

		constexpr SheetMember(const StatType<T> &stat) : _type(Type::stat), _uni{.stat = stat} {};
		constexpr SheetMember(const SkillType<T> &skill) : _type(Type::skill), _uni{.skill = skill} {};

		constexpr const T::_Value &resolve(const T &sheet) const {
			switch (_type) {
				case Type::stat:
					return std::invoke(_uni.stat, sheet);
				case Type::skill:
					return std::invoke(_uni.skill.second, std::invoke(_uni.skill.first, sheet));
			}
			std::unreachable();
		}
	};

	template<class T>
	struct TalentMember {
		using RetType = T::Type;
		TalentType<T> _t;

		constexpr TalentMember(const TalentType<T> &talent) : _t(talent) {}

		constexpr const T::Type &resolve(const T &sheet) const {
			return std::invoke(_t, sheet);
		}
	};

	template<auto val>
	using RetType = decltype(val.eval(std::declval<const Formula::Context &>()));

	template<auto val>
	using RetTypeMember = decltype(std::declval<typename decltype(val)::RetType>().eval(std::declval<const Formula::Context &>()));

	struct SheetMemberIdentifier {
		struct Infusion {};
		enum class Type : uint8_t {
			stat,
			attack,
			element,
			reaction,
			talent,
			enemyStat,
			enemyRes,
			infusion,
		} _type;
		union _Uni {
			::Stat stat;
			std::pair<Misc::AttackSource, Misc::SkillStat> attack;
			std::pair<Misc::DamageElement, Misc::SkillStat> element;
			std::pair<Misc::Reaction, Misc::SkillStat> reaction;
			::LevelableTalent talent;
			::Misc::EnemyStat enemyStat;
			std::pair<Misc::EnemyResistances, Misc::Element> enemyRes;
			Infusion infusion;
		} _uni;

		constexpr SheetMemberIdentifier(::Stat stat) : _type(Type::stat), _uni{.stat = stat} {}
		constexpr SheetMemberIdentifier(Misc::AttackSource attack, Misc::SkillStat stat) : _type(Type::attack), _uni{.attack{attack, stat}} {}
		constexpr SheetMemberIdentifier(Misc::DamageElement element, Misc::SkillStat stat) : _type(Type::element), _uni{.element{element, stat}} {}
		constexpr SheetMemberIdentifier(Misc::Reaction reaction, Misc::SkillStat stat) : _type(Type::reaction), _uni{.reaction{reaction, stat}} {}
		constexpr SheetMemberIdentifier(LevelableTalent talent) : _type(Type::talent), _uni{.talent = talent} {}
		constexpr SheetMemberIdentifier(Misc::EnemyStat enemyStat) : _type(Type::enemyStat), _uni{.enemyStat = enemyStat} {}
		constexpr SheetMemberIdentifier(Misc::EnemyResistances enemyRes, Misc::Element element) : _type(Type::enemyRes), _uni{.enemyRes{enemyRes, element}} {}
		constexpr SheetMemberIdentifier(Infusion infusion) : _type(Type::infusion), _uni{.infusion{}} {}
		static constexpr SheetMemberIdentifier infusion() {
			return Infusion{};
		}

		[[nodiscard]] std::string getName() const {
			switch (_type) {
				case Type::stat:
					return Utils::Stringify(_uni.stat);
				case Type::attack:
					return fmt::format("{} {}", Utils::Stringify(_uni.attack.first), Utils::Stringify(_uni.attack.second));
				case Type::element:
					return fmt::format("{} {}", Utils::Stringify(_uni.element.first), Utils::Stringify(_uni.attack.second));
				case Type::reaction:
					return fmt::format("{} {}", Utils::Stringify(_uni.reaction.first), Utils::Stringify(_uni.attack.second));
				case Type::talent:
					return fmt::format("{}", Utils::Stringify(_uni.talent));
				case Type::enemyStat:
					return fmt::format("{}", Utils::Stringify(_uni.enemyStat));
				case Type::enemyRes:
					return fmt::format("{} {}", Utils::Stringify(_uni.enemyRes.second), Utils::Stringify(_uni.enemyRes.first));
				case Type::infusion:
					return "Infusion";
			}
			std::unreachable();
		}

		[[nodiscard]] constexpr bool isPercentage() const {
			switch (_type) {
				case Type::stat:
					return Utils::isPercentage(_uni.stat);
				case Type::attack:
					return Utils::isPercentage(_uni.attack.second);
				case Type::element:
					return Utils::isPercentage(_uni.element.second);
				case Type::reaction:
					return Utils::isPercentage(_uni.reaction.second);
				case Type::talent:
					return false;
				case Type::enemyStat:
					return Utils::isPercentage(_uni.enemyStat);
				case Type::enemyRes:
					return true;
				case Type::infusion:
					return false;
			}
			std::unreachable();
		}
	};
}// namespace Modifiers