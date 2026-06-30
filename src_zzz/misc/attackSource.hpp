#pragma once

#include "cstdint"
#include "utils/optional.hpp"
#include "utils/stringify.hpp"
#include <string>
#include <utility>
#include <vector>


namespace Misc {
	enum class AttackSource : uint8_t {
		basic,
		dodge,
		dashAttack,
		dodgeCounter,
		assist,
		quickAssist,
		defensiveAssist,
		evasiveAssist,
		assistFollowup,
		special,
		exspecial,
		chain,
		ultimate,
	};

	static inline std::vector<AttackSource> attackSources{
		AttackSource::basic,
		AttackSource::dodge,
		AttackSource::dashAttack,
		AttackSource::dodgeCounter,
		AttackSource::assist,
		AttackSource::quickAssist,
		AttackSource::defensiveAssist,
		AttackSource::evasiveAssist,
		AttackSource::assistFollowup,
		AttackSource::special,
		AttackSource::exspecial,
		AttackSource::chain,
		AttackSource::ultimate,
	};

	template<class T>
	[[nodiscard]] inline auto ptrFromAttackSource(const Misc::AttackSource &attackSource) {
		switch (attackSource) {
			case Misc::AttackSource::basic:
				return &T::basic;
			case Misc::AttackSource::dodge:
				return &T::dodge;
			case Misc::AttackSource::dashAttack:
				return &T::dashAttack;
			case Misc::AttackSource::dodgeCounter:
				return &T::dodgeCounter;
			case Misc::AttackSource::assist:
				return &T::assist;
			case Misc::AttackSource::quickAssist:
				return &T::quickAssist;
			case Misc::AttackSource::defensiveAssist:
				return &T::defensiveAssist;
			case Misc::AttackSource::evasiveAssist:
				return &T::evasiveAssist;
			case Misc::AttackSource::assistFollowup:
				return &T::assistFollowup;
			case Misc::AttackSource::special:
				return &T::special;
			case Misc::AttackSource::exspecial:
				return &T::exspecial;
			case Misc::AttackSource::chain:
				return &T::chain;
			case Misc::AttackSource::ultimate:
				return &T::ultimate;
		}
		std::unreachable();
	}
}// namespace Misc

namespace Utils {
	template<>
	constexpr std::string Stringify<>(const Misc::AttackSource &attackSource) {
		switch (attackSource) {
			case Misc::AttackSource::basic:
				return "Basic";
			case Misc::AttackSource::dodge:
				return "Dodge";
			case Misc::AttackSource::dashAttack:
				return "Dash Attack";
			case Misc::AttackSource::dodgeCounter:
				return "Dodge Counter";
			case Misc::AttackSource::assist:
				return "Assist";
			case Misc::AttackSource::quickAssist:
				return "Quick Assist";
			case Misc::AttackSource::defensiveAssist:
				return "Defensive Assist";
			case Misc::AttackSource::evasiveAssist:
				return "Evasive Assist";
			case Misc::AttackSource::assistFollowup:
				return "Assist Followup";
			case Misc::AttackSource::special:
				return "Special";
			case Misc::AttackSource::exspecial:
				return "EX Special";
			case Misc::AttackSource::chain:
				return "Chain";
			case Misc::AttackSource::ultimate:
				return "Ultimate";
		}
		std::unreachable();
	}
	template<>
	constexpr std::string Stringify<>(const Utils::JankyOptional<Misc::AttackSource> &attackSource) {
		if (!attackSource.has_value()) return "Unknown Attack Source";
		return Stringify(attackSource.value());
	}
}// namespace Utils