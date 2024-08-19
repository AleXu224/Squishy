#pragma once

#include "fmt/core.h"
#include "intermediary.hpp"
#include "stats/loadout.hpp"


namespace Formula {
	enum class Requirement {
		passive1,
		passive2,
		constellation1,
		constellation2,
		constellation3,
		constellation4,
		constellation5,
		constellation6,
	};

	[[nodiscard]] inline bool _getRequirement(Requirement req, const Stats::Loadout &stats) {
		switch (req) {
			case Requirement::passive1:
				return stats.character.sheet.ascension >= 1;
			case Requirement::passive2:
				return stats.character.sheet.ascension >= 4;
			case Requirement::constellation1:
				return stats.character.sheet.constellation >= 1;
			case Requirement::constellation2:
				return stats.character.sheet.constellation >= 2;
			case Requirement::constellation3:
				return stats.character.sheet.constellation >= 3;
			case Requirement::constellation4:
				return stats.character.sheet.constellation >= 4;
			case Requirement::constellation5:
				return stats.character.sheet.constellation >= 5;
			case Requirement::constellation6:
				return stats.character.sheet.constellation >= 6;
		}
		std::unreachable();
	}

	[[nodiscard]] inline std::string_view _getRequirementName(Requirement req) {
		switch (req) {
			case Requirement::passive1:
				return "Passive 1";
			case Requirement::passive2:
				return "Passive 2";
			case Requirement::constellation1:
				return "Constellation 1";
			case Requirement::constellation2:
				return "Constellation 2";
			case Requirement::constellation3:
				return "Constellation 3";
			case Requirement::constellation4:
				return "Constellation 4";
			case Requirement::constellation5:
				return "Constellation 5";
			case Requirement::constellation6:
				return "Constellation 6";
		}
		std::unreachable();
	}

	template<class T>
	struct Requires {
		Requirement requirement;
		T ret;

		[[nodiscard]] inline std::string print(const Stats::Loadout &stats, Step) const {
			auto cond = _getRequirement(requirement, stats);
			return fmt::format("{}", cond ? fmt::format("{} : ({})", _getRequirementName(requirement), ret.print(stats, Step::none)) : "");
		}

		[[nodiscard]] inline float eval(const Stats::Loadout &stats) const {
			auto cond = _getRequirement(requirement, stats);
			if (cond)
				return ret.eval(stats);
			else
				return 0.f;
		}
	};
}// namespace Formula