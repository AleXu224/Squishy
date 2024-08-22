#pragma once

#include "conditional/conditional.hpp"
#include "fmt/core.h"
#include "intermediary.hpp"
#include "stats/loadout.hpp"


namespace Formula {
	[[nodiscard]] inline const Conditional::TypesMap &_getConditionalLocation(Conditional::Location location, const Stats::Loadout &stats) {
		switch (location) {
			case Conditional::Location::normal:
				return stats.character.conditionals.normal;
			case Conditional::Location::charged:
				return stats.character.conditionals.charged;
			case Conditional::Location::plunge:
				return stats.character.conditionals.plunge;
			case Conditional::Location::skill:
				return stats.character.conditionals.skill;
			case Conditional::Location::burst:
				return stats.character.conditionals.burst;
			case Conditional::Location::passive1:
				return stats.character.conditionals.passive1;
			case Conditional::Location::passive2:
				return stats.character.conditionals.passive2;
			case Conditional::Location::constellation1:
				return stats.character.conditionals.constellation1;
			case Conditional::Location::constellation2:
				return stats.character.conditionals.constellation2;
			case Conditional::Location::constellation4:
				return stats.character.conditionals.constellation4;
			case Conditional::Location::constellation6:
				return stats.character.conditionals.constellation6;
			case Conditional::Location::weapon:
				return stats.weapon.conditionals;
			case Conditional::Location::artifact:
				return stats.artifact.conditionals;
		}
		std::unreachable();
	}

	template<class T>
	struct Conditional {
		::Conditional::Location location{};
		Utils::HashedString name;
		T val;

		[[nodiscard]] inline std::string print(const Stats::Loadout &stats, const Stats::Team &team, Step) const {
			auto cond = ::Conditional::getBool(_getConditionalLocation(location, stats), name);
			return fmt::format("{}", cond ? fmt::format("{} {}", name.str, val.print(stats, team, Step::none)) : "");
		}

		[[nodiscard]] inline float eval(const Stats::Loadout &stats, const Stats::Team &team) const {
			auto cond = ::Conditional::getBool(_getConditionalLocation(location, stats), name);
			if (cond)
				return val.eval(stats, team);

			return 0.f;
		}
	};
	struct ConditionalValue {
		::Conditional::Location location{};
		Utils::HashedString name;
		float defaultValue = 0.f;

		[[nodiscard]] inline std::string print(const Stats::Loadout &stats, const Stats::Team &, Step) const {
			auto cond = ::Conditional::getFloat(_getConditionalLocation(location, stats), name, defaultValue);
			return fmt::format("{} {}", name.str, cond);
		}

		[[nodiscard]] inline float eval(const Stats::Loadout &stats, const Stats::Team &) const {
			auto cond = ::Conditional::getFloat(_getConditionalLocation(location, stats), name, defaultValue);
			return cond;
		}
	};
}// namespace Formula