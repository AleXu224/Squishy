#pragma once

#include "formula/base.hpp"
#include "misc/anomaly.hpp"
#include "misc/attackSource.hpp"
#include "misc/damageAttribute.hpp"
#include "misc/damageType.hpp"
#include "misc/enemyProps.hpp"
#include "misc/skillStat.hpp"
#include "stats/skill.hpp"
#include "stats/stat.hpp"
#include "utility"
#include "utils/overloaded.hpp"
#include <functional>


namespace Modifiers {
	template<class T>
	using StatType = T::_Value T::*;
	template<class T>
	struct SkillType {
		T::_SkillValue T::*first;
		T::_Value T::_SkillValue::*second;
	};

	template<class T>
	struct EnemySkillType {
		T::_SkillValue T::*first;
		T::_Value T::_EnemySheet::*second;
	};

	template<class T>
	struct EnemyResSkillType {
		T::_SkillValue T::*first;
		T::_Value T::_EnemySheet::_SkillValue::*second;
	};

	template<class T>
	using SkillMemberType = T::Type T::*;

	template<class T>
	struct SheetMember {
		using RetType = T::_Value;
		enum class Type : uint8_t {
			stat,
			skill,
			skillEnemy,
			skillEnemyRes,
		} _type;
		union _Uni {
			StatType<T> stat;
			SkillType<T> skill;
			EnemySkillType<T> skillEnemy;
			EnemyResSkillType<T> skillEnemyRes;
		} _uni;

		constexpr SheetMember(const StatType<T> &stat) : _type(Type::stat), _uni{.stat = stat} {};
		constexpr SheetMember(const SkillType<T> &skill) : _type(Type::skill), _uni{.skill = skill} {};
		constexpr SheetMember(const EnemySkillType<T> &skillEnemy) : _type(Type::skillEnemy), _uni{.skillEnemy = skillEnemy} {};
		constexpr SheetMember(const EnemyResSkillType<T> &skillEnemyRes) : _type(Type::skillEnemyRes), _uni{.skillEnemyRes = skillEnemyRes} {};

		constexpr const T::_Value &resolve(const T &sheet) const {
			switch (_type) {
				case Type::stat:
					return std::invoke(_uni.stat, sheet);
				case Type::skill:
					return std::invoke(_uni.skill.second, std::invoke(_uni.skill.first, sheet));
				case Type::skillEnemy:
					return std::invoke(_uni.skillEnemy.second, std::invoke(_uni.skillEnemy.first, sheet).enemy);
				case Type::skillEnemyRes:
					return std::invoke(_uni.skillEnemyRes.second, std::invoke(_uni.skillEnemyRes.first, sheet).enemy.resistance);
			}
			std::unreachable();
		}
	};

	template<class T>
	struct EnemyMember {
		using RetType = T::_Value;
		enum class Type : uint8_t {
			stat,
			skill,
		} _type;
		union _Uni {
			StatType<T> stat;
			SkillType<T> skill;
		} _uni;

		constexpr EnemyMember(const StatType<T> &stat) : _type(Type::stat), _uni{.stat = stat} {};
		constexpr EnemyMember(const SkillType<T> &skill) : _type(Type::skill), _uni{.skill = skill} {};

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
	struct SkillMember {
		using RetType = T::Type;
		SkillMemberType<T> _t;

		constexpr SkillMember(const SkillMemberType<T> &skill) : _t(skill) {}

		constexpr const T::Type &resolve(const T &sheet) const {
			return std::invoke(_t, sheet);
		}
	};

	// template<auto val>
	// using RetType = decltype(val.eval(std::declval<const Formula::Context &>()));

	template<class T>
	using RetType = decltype(std::declval<T>().eval(std::declval<const Formula::Context &>()));

	template<auto val>
	using RetTypeMember = decltype(std::declval<typename decltype(val)::RetType>().eval(std::declval<const Formula::Context &>()));

	using SkillStatVariant = std::variant<Misc::SkillStat, Misc::EnemyStat, std::pair<Misc::EnemyResistances, Misc::Attribute>>;

	struct SheetMemberIdentifier {
		enum class Type : uint8_t {
			stat,
			attack,
			attribute,
			anomaly,
			damageType,
			skill,
			enemyStat,
			enemyRes,
		} _type;
		union _Uni {
			::Stat stat;
			std::pair<Misc::AttackSource, SkillStatVariant> attack;
			std::pair<Misc::DamageAttribute, SkillStatVariant> attribute;
			std::pair<Misc::DamageAnomaly, SkillStatVariant> anomaly;
			std::pair<Misc::DamageType, SkillStatVariant> damageType;
			::LevelableSkill skill;
			::Misc::EnemyStat enemyStat;
			std::pair<Misc::EnemyResistances, Misc::Attribute> enemyRes;
		} _uni;

		constexpr SheetMemberIdentifier(::Stat stat) : _type(Type::stat), _uni{.stat = stat} {}
		constexpr SheetMemberIdentifier(Misc::AttackSource attack, SkillStatVariant stat) : _type(Type::attack), _uni{.attack{attack, stat}} {}
		constexpr SheetMemberIdentifier(Misc::DamageAttribute attribute, SkillStatVariant stat) : _type(Type::attribute), _uni{.attribute{attribute, stat}} {}
		constexpr SheetMemberIdentifier(Misc::DamageAnomaly anomaly, SkillStatVariant stat) : _type(Type::anomaly), _uni{.anomaly{anomaly, stat}} {}
		constexpr SheetMemberIdentifier(Misc::DamageType damageType, SkillStatVariant stat) : _type(Type::damageType), _uni{.damageType{damageType, stat}} {}
		constexpr SheetMemberIdentifier(::LevelableSkill skill) : _type(Type::skill), _uni{.skill = skill} {}
		constexpr SheetMemberIdentifier(Misc::EnemyStat enemyStat) : _type(Type::enemyStat), _uni{.enemyStat = enemyStat} {}
		constexpr SheetMemberIdentifier(Misc::EnemyResistances enemyRes, Misc::Attribute attribute) : _type(Type::enemyRes), _uni{.enemyRes{enemyRes, attribute}} {}

		[[nodiscard]] std::string getName() const {
			switch (_type) {
				case Type::stat:
					return Utils::Stringify(_uni.stat);
				case Type::attack:
					return std::format("{} {}", Utils::Stringify(_uni.attack.first), Utils::Stringify(_uni.attack.second));
				case Type::attribute:
					return std::format("{} {}", Utils::Stringify(_uni.attribute.first), Utils::Stringify(_uni.attribute.second));
				case Type::anomaly:
					return std::format("{} {}", Utils::Stringify(_uni.anomaly.first), Utils::Stringify(_uni.anomaly.second));
				case Type::skill:
					return std::format("{}", Utils::Stringify(_uni.skill));
				case Type::enemyStat:
					return std::format("{}", Utils::Stringify(_uni.enemyStat));
				case Type::enemyRes:
					return std::format("{} {}", Utils::Stringify(_uni.enemyRes.second), Utils::Stringify(_uni.enemyRes.first));
				case Type::damageType:
					return std::format("{} {}", Utils::Stringify(_uni.damageType.first), Utils::Stringify(_uni.damageType.second));
			}
			std::unreachable();
		}

		[[nodiscard]] constexpr bool isPercentage() const {
			switch (_type) {
				case Type::stat:
					return Utils::isPercentage(_uni.stat);
				case Type::attack:
					return Utils::isPercentage(_uni.attack.second);
				case Type::attribute:
					return Utils::isPercentage(_uni.attribute.second);
				case Type::anomaly:
					return Utils::isPercentage(_uni.anomaly.second);
				case Type::damageType:
					return Utils::isPercentage(_uni.damageType.second);
				case Type::skill:
					return false;
				case Type::enemyStat:
					return Utils::isPercentage(_uni.enemyStat);
				case Type::enemyRes:
					return true;
			}
			std::unreachable();
		}

		[[nodiscard]] constexpr bool isDiscStat() const {
			switch (_type) {
				case Type::stat: {
					switch (_uni.stat) {
						case Stat::hp:
						case Stat::hp_:
						case Stat::atk:
						case Stat::atk_:
						case Stat::def:
						case Stat::def_:
						case Stat::ap:
						case Stat::am_:
						case Stat::pen:
						case Stat::penRatio:
						case Stat::cr:
						case Stat::cd:
						case Stat::impact_:
						case Stat::er_:
							return true;
						default:
							return false;
					}
				}
				case Type::attribute: {
					switch (_uni.attribute.first) {
						case Misc::DamageAttribute::all:
							return false;
						default:
							break;
					}
					return std::visit(
						Utils::overloaded{
							[](Misc::SkillStat skillStat) {
								return skillStat == Misc::SkillStat::DMG;
							},
							[](Misc::EnemyStat enemyStat) {
								return false;
							},
							[](std::pair<Misc::EnemyResistances, Misc::Attribute> enemyRes) {
								return false;
							},
						},
						_uni.attribute.second
					);
				}
				default:
					return false;
			}
			std::unreachable();
		}
	};
}// namespace Modifiers