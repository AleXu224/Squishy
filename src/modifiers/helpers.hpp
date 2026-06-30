#pragma once

#include "Talents.hpp"
#include "formula/base.hpp"
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
	using TalentType = T::Type T::*;

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
	struct TalentMember {
		using RetType = T::Type;
		TalentType<T> _t;

		constexpr TalentMember(const TalentType<T> &talent) : _t(talent) {}

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

	using SkillStatVariant = std::variant<Misc::SkillStat, Misc::EnemyStat, std::pair<Misc::EnemyResistances, Misc::Element>>;

	struct SheetMemberIdentifier {
		struct Infusion {};
		struct MoonsignLevel {};
		enum class Type : uint8_t {
			stat,
			attack,
			element,
			reaction,
			talent,
			enemyStat,
			enemyRes,
			infusion,
			moonsignLevel,
		} _type;
		union _Uni {
			::Stat stat;
			std::pair<Misc::AttackSource, SkillStatVariant> attack;
			std::pair<Misc::DamageElement, SkillStatVariant> element;
			std::pair<Misc::Reaction, SkillStatVariant> reaction;
			::LevelableTalent talent;
			::Misc::EnemyStat enemyStat;
			std::pair<Misc::EnemyResistances, Misc::Element> enemyRes;
			Infusion infusion;
			MoonsignLevel moonsignLevel;
		} _uni;

		constexpr SheetMemberIdentifier(::Stat stat) : _type(Type::stat), _uni{.stat = stat} {}
		constexpr SheetMemberIdentifier(Misc::AttackSource attack, SkillStatVariant stat) : _type(Type::attack), _uni{.attack{attack, stat}} {}
		constexpr SheetMemberIdentifier(Misc::DamageElement element, SkillStatVariant stat) : _type(Type::element), _uni{.element{element, stat}} {}
		constexpr SheetMemberIdentifier(Misc::Reaction reaction, SkillStatVariant stat) : _type(Type::reaction), _uni{.reaction{reaction, stat}} {}
		constexpr SheetMemberIdentifier(LevelableTalent talent) : _type(Type::talent), _uni{.talent = talent} {}
		constexpr SheetMemberIdentifier(Misc::EnemyStat enemyStat) : _type(Type::enemyStat), _uni{.enemyStat = enemyStat} {}
		constexpr SheetMemberIdentifier(Misc::EnemyResistances enemyRes, Misc::Element element) : _type(Type::enemyRes), _uni{.enemyRes{enemyRes, element}} {}
		constexpr SheetMemberIdentifier(Infusion) : _type(Type::infusion), _uni{.infusion{}} {}
		constexpr SheetMemberIdentifier(MoonsignLevel) : _type(Type::moonsignLevel), _uni{.moonsignLevel{}} {}
		static constexpr SheetMemberIdentifier infusion() {
			return Infusion{};
		}
		static constexpr SheetMemberIdentifier moonsignLevel() {
			return MoonsignLevel{};
		}

		[[nodiscard]] std::string getName() const {
			switch (_type) {
				case Type::stat:
					return Utils::Stringify(_uni.stat);
				case Type::attack:
					return std::format("{} {}", Utils::Stringify(_uni.attack.first), Utils::Stringify(_uni.attack.second));
				case Type::element:
					return std::format("{} {}", Utils::Stringify(_uni.element.first), Utils::Stringify(_uni.element.second));
				case Type::reaction:
					return std::format("{} {}", Utils::Stringify(_uni.reaction.first), Utils::Stringify(_uni.reaction.second));
				case Type::talent:
					return std::format("{}", Utils::Stringify(_uni.talent));
				case Type::enemyStat:
					return std::format("{}", Utils::Stringify(_uni.enemyStat));
				case Type::enemyRes:
					return std::format("{} {}", Utils::Stringify(_uni.enemyRes.second), Utils::Stringify(_uni.enemyRes.first));
				case Type::infusion:
					return "Infusion";
				case Type::moonsignLevel:
					return "Moonsign Level";
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
				case Type::moonsignLevel:
					return false;
			}
			std::unreachable();
		}

		[[nodiscard]] constexpr bool isArtifactStat() const {
			switch (_type) {
				case Type::stat: {
					switch (_uni.stat) {
						case Stat::hp:
						case Stat::hp_:
						case Stat::atk:
						case Stat::atk_:
						case Stat::def:
						case Stat::def_:
						case Stat::er:
						case Stat::em:
						case Stat::cr:
						case Stat::cd:
						case Stat::hb:
							return true;
						default:
							return false;
					}
				}
				case Type::element: {
					switch (_uni.element.first) {
						case Misc::DamageElement::all:
						case Misc::DamageElement::allElemental:
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
							[](std::pair<Misc::EnemyResistances, Misc::Element> enemyRes) {
								return false;
							},
						},
						_uni.element.second
					);
				}
				default:
					return false;
			}
			std::unreachable();
		}
	};
}// namespace Modifiers