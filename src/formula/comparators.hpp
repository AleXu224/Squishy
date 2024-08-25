#pragma once

#include "fmt/core.h"
#include "intermediary.hpp"


namespace Formula {
	template<FormulaLike T, FormulaLike V>
	struct Equal {
		T val1;
		V val2;
		[[nodiscard]] inline std::string print(const Stats::Loadout &stats, const Stats::Team &team, Step) const {
			return fmt::format("{} == {}", val1.print(stats, team, Step::none), val2.print(stats, team, Step::none));
		}

		[[nodiscard]] inline bool eval(const Stats::Loadout &stats, const Stats::Team &team) const {
			return val1.eval(stats, team) == val2.eval(stats, team);
		}
	};

	template<FormulaLike T, FormulaLike V>
	struct NotEqual {
		T val1;
		V val2;
		[[nodiscard]] inline std::string print(const Stats::Loadout &stats, const Stats::Team &team, Step) const {
			return fmt::format("{} != {}", val1.print(stats, team, Step::none), val2.print(stats, team, Step::none));
		}

		[[nodiscard]] inline bool eval(const Stats::Loadout &stats, const Stats::Team &team) const {
			return val1.eval(stats, team) != val2.eval(stats, team);
		}
	};

	template<BoolFormula T>
	struct Not {
		T val;
		[[nodiscard]] inline std::string print(const Stats::Loadout &stats, const Stats::Team &team, Step) const {
			return fmt::format("!{}", val.print(stats, team, Step::none));
		}

		[[nodiscard]] inline bool eval(const Stats::Loadout &stats, const Stats::Team &team) const {
			return !val.eval(stats, team);
		}
	};

	template<FloatFormula T, FloatFormula V>
	struct Less {
		T val1;
		V val2;
		[[nodiscard]] inline std::string print(const Stats::Loadout &stats, const Stats::Team &team, Step) const {
			return fmt::format("{} < {}", val1.print(stats, team, Step::none), val2.print(stats, team, Step::none));
		}

		[[nodiscard]] inline bool eval(const Stats::Loadout &stats, const Stats::Team &team) const {
			return val1.eval(stats, team) < val2.eval(stats, team);
		}
	};
	template<FloatFormula T, FloatFormula V>
	struct LessEqual {
		T val1;
		V val2;
		[[nodiscard]] inline std::string print(const Stats::Loadout &stats, const Stats::Team &team, Step) const {
			return fmt::format("{} <= {}", val1.print(stats, team, Step::none), val2.print(stats, team, Step::none));
		}

		[[nodiscard]] inline bool eval(const Stats::Loadout &stats, const Stats::Team &team) const {
			return val1.eval(stats, team) <= val2.eval(stats, team);
		}
	};
	template<FloatFormula T, FloatFormula V>
	struct Greater {
		T val1;
		V val2;
		[[nodiscard]] inline std::string print(const Stats::Loadout &stats, const Stats::Team &team, Step) const {
			return fmt::format("{} > {}", val1.print(stats, team, Step::none), val2.print(stats, team, Step::none));
		}

		[[nodiscard]] inline bool eval(const Stats::Loadout &stats, const Stats::Team &team) const {
			return val1.eval(stats, team) > val2.eval(stats, team);
		}
	};
	template<FloatFormula T, FloatFormula V>
	struct GreaterEqual {
		T val1;
		V val2;
		[[nodiscard]] inline std::string print(const Stats::Loadout &stats, const Stats::Team &team, Step) const {
			return fmt::format("{} >= {}", val1.print(stats, team, Step::none), val2.print(stats, team, Step::none));
		}

		[[nodiscard]] inline bool eval(const Stats::Loadout &stats, const Stats::Team &team) const {
			return val1.eval(stats, team) >= val2.eval(stats, team);
		}
	};

	template<BoolFormula T, BoolFormula V>
	struct And {
		T val1;
		V val2;
		[[nodiscard]] inline std::string print(const Stats::Loadout &stats, const Stats::Team &team, Step) const {
			return fmt::format("{} && {}", val1.print(stats, team, Step::none), val2.print(stats, team, Step::none));
		}

		[[nodiscard]] inline bool eval(const Stats::Loadout &stats, const Stats::Team &team) const {
			return val1.eval(stats, team) && val2.eval(stats, team);
		}
	};
	template<BoolFormula T, BoolFormula V>
	struct Or {
		T val1;
		V val2;
		[[nodiscard]] inline std::string print(const Stats::Loadout &stats, const Stats::Team &team, Step) const {
			return fmt::format("{} || {}", val1.print(stats, team, Step::none), val2.print(stats, team, Step::none));
		}

		[[nodiscard]] inline bool eval(const Stats::Loadout &stats, const Stats::Team &team) const {
			return val1.eval(stats, team) || val2.eval(stats, team);
		}
	};
}// namespace Formula