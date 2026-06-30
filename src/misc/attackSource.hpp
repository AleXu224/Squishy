#pragma once

#include "cstdint"
#include "utils/optional.hpp"
#include "utils/stringify.hpp"
#include <string>
#include <utility>
#include <vector>


namespace Misc {
	enum class AttackSource : uint8_t {
		normal,
		charged,
		plunge,
		skill,
		burst,
	};

	static inline std::vector<AttackSource> attackSources{
		AttackSource::normal,
		AttackSource::charged,
		AttackSource::plunge,
		AttackSource::skill,
		AttackSource::burst
	};

	template<class T>
	[[nodiscard]] inline auto ptrFromAttackSource(const Misc::AttackSource &attackSource) {
		switch (attackSource) {
			case Misc::AttackSource::normal:
				return &T::normal;
			case Misc::AttackSource::charged:
				return &T::charged;
			case Misc::AttackSource::plunge:
				return &T::plunge;
			case Misc::AttackSource::skill:
				return &T::skill;
			case Misc::AttackSource::burst:
				return &T::burst;
		}
		std::unreachable();
	}
}// namespace Misc

namespace Utils {
	template<>
	constexpr std::string Stringify<>(const Misc::AttackSource &attackSource) {
		switch (attackSource) {
			case Misc::AttackSource::normal:
				return "Normal Attack";
			case Misc::AttackSource::charged:
				return "Charged Attack";
			case Misc::AttackSource::plunge:
				return "Plunge Attack";
			case Misc::AttackSource::skill:
				return "Elemental Skill";
			case Misc::AttackSource::burst:
				return "Elemental Burst";
		}
		std::unreachable();
	}
	template<>
	constexpr std::string Stringify<>(const Utils::JankyOptional<Misc::AttackSource> &attackSource) {
		if (!attackSource.has_value()) return "Unknown Attack Source";
		switch (attackSource.value()) {
			case Misc::AttackSource::normal:
				return "Normal Attack";
			case Misc::AttackSource::charged:
				return "Charged Attack";
			case Misc::AttackSource::plunge:
				return "Plunge Attack";
			case Misc::AttackSource::skill:
				return "Elemental Skill";
			case Misc::AttackSource::burst:
				return "Elemental Burst";
		}
		std::unreachable();
	}
}// namespace Utils