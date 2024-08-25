#pragma once

#include "fmt/core.h"
#include "intermediary.hpp"
#include "stats/loadout.hpp"


namespace Formula {
	template<BoolFormula T, class V>
	struct Requires {
		T requirement;
		V ret;

		using RetType = decltype(std::declval<V>().eval(std::declval<const Stats::Loadout &>(), std::declval<const Stats::Team &>()));

		[[nodiscard]] inline std::string print(const Stats::Loadout &stats, const Stats::Team &team, Step) const {
			auto cond = requirement.eval(stats, team);
			return fmt::format("{}", cond ? fmt::format("{} : ({})", requirement.print(stats, team, Step::none), ret.print(stats, team, Step::none)) : "");
		}

		[[nodiscard]] inline RetType eval(const Stats::Loadout &stats, const Stats::Team &team) const {
			if (requirement.eval(stats, team))
				return ret.eval(stats, team);

			return RetType{};
		}
	};
}// namespace Formula

namespace Requirement {
	struct Passive1 {
		[[nodiscard]] static inline std::string print(const Stats::Loadout &, const Stats::Team &, Formula::Step) {
			return "Passive 1";
		}

		[[nodiscard]] static inline bool eval(const Stats::Loadout &stats, const Stats::Team &) {
			return stats.character.sheet.ascension >= 1;
		}
	};
	struct Passive2 {
		[[nodiscard]] static inline std::string print(const Stats::Loadout &, const Stats::Team &, Formula::Step) {
			return "Passive 2";
		}

		[[nodiscard]] static inline bool eval(const Stats::Loadout &stats, const Stats::Team &) {
			return stats.character.sheet.ascension >= 4;
		}
	};
	struct Constellation1 {
		[[nodiscard]] static inline std::string print(const Stats::Loadout &, const Stats::Team &, Formula::Step) {
			return "Constellation 1";
		}

		[[nodiscard]] static inline bool eval(const Stats::Loadout &stats, const Stats::Team &) {
			return stats.character.sheet.constellation >= 1;
		}
	};
	struct Constellation2 {
		[[nodiscard]] static inline std::string print(const Stats::Loadout &, const Stats::Team &, Formula::Step) {
			return "Constellation 2";
		}

		[[nodiscard]] static inline bool eval(const Stats::Loadout &stats, const Stats::Team &) {
			return stats.character.sheet.constellation >= 2;
		}
	};
	struct Constellation3 {
		[[nodiscard]] static inline std::string print(const Stats::Loadout &, const Stats::Team &, Formula::Step) {
			return "Constellation 3";
		}

		[[nodiscard]] static inline bool eval(const Stats::Loadout &stats, const Stats::Team &) {
			return stats.character.sheet.constellation >= 3;
		}
	};
	struct Constellation4 {
		[[nodiscard]] static inline std::string print(const Stats::Loadout &, const Stats::Team &, Formula::Step) {
			return "Constellation 4";
		}

		[[nodiscard]] static inline bool eval(const Stats::Loadout &stats, const Stats::Team &) {
			return stats.character.sheet.constellation >= 4;
		}
	};
	struct Constellation5 {
		[[nodiscard]] static inline std::string print(const Stats::Loadout &, const Stats::Team &, Formula::Step) {
			return "Constellation 5";
		}

		[[nodiscard]] static inline bool eval(const Stats::Loadout &stats, const Stats::Team &) {
			return stats.character.sheet.constellation >= 5;
		}
	};
	struct Constellation6 {
		[[nodiscard]] static inline std::string print(const Stats::Loadout &, const Stats::Team &, Formula::Step) {
			return "Constellation 6";
		}

		[[nodiscard]] static inline bool eval(const Stats::Loadout &stats, const Stats::Team &) {
			return stats.character.sheet.constellation >= 6;
		}
	};
}// namespace Requirement