#pragma once

#include "fmt/core.h"
#include "intermediary.hpp"


namespace Formula {
	template<FormulaLike T, FormulaLike V>
	struct Equal {
		T val1;
		V val2;
		[[nodiscard]] inline std::string print(const Stats::Loadout &source, const Stats::Loadout &target, const Stats::Team &team, Step) const {
			return fmt::format("{} == {}", val1.print(source, target, team, Step::none), val2.print(source, target, team, Step::none));
		}

		[[nodiscard]] inline bool eval(const Stats::Loadout &source, const Stats::Loadout &target, const Stats::Team &team) const {
			return val1.eval(source, target, team) == val2.eval(source, target, team);
		}
	};

	template<FormulaLike T, FormulaLike V>
	struct NotEqual {
		T val1;
		V val2;
		[[nodiscard]] inline std::string print(const Stats::Loadout &source, const Stats::Loadout &target, const Stats::Team &team, Step) const {
			return fmt::format("{} != {}", val1.print(source, target, team, Step::none), val2.print(source, target, team, Step::none));
		}

		[[nodiscard]] inline bool eval(const Stats::Loadout &source, const Stats::Loadout &target, const Stats::Team &team) const {
			return val1.eval(source, target, team) != val2.eval(source, target, team);
		}
	};

	template<BoolFormula T>
	struct Not {
		T val;
		[[nodiscard]] inline std::string print(const Stats::Loadout &source, const Stats::Loadout &target, const Stats::Team &team, Step) const {
			return fmt::format("!{}", val.print(source, target, team, Step::none));
		}

		[[nodiscard]] inline bool eval(const Stats::Loadout &source, const Stats::Loadout &target, const Stats::Team &team) const {
			return !val.eval(source, target, team);
		}
	};

	template<FloatFormula T, FloatFormula V>
	struct Less {
		T val1;
		V val2;
		[[nodiscard]] inline std::string print(const Stats::Loadout &source, const Stats::Loadout &target, const Stats::Team &team, Step) const {
			return fmt::format("{} < {}", val1.print(source, target, team, Step::none), val2.print(source, target, team, Step::none));
		}

		[[nodiscard]] inline bool eval(const Stats::Loadout &source, const Stats::Loadout &target, const Stats::Team &team) const {
			return val1.eval(source, target, team) < val2.eval(source, target, team);
		}
	};
	template<FloatFormula T, FloatFormula V>
	struct LessEqual {
		T val1;
		V val2;
		[[nodiscard]] inline std::string print(const Stats::Loadout &source, const Stats::Loadout &target, const Stats::Team &team, Step) const {
			return fmt::format("{} <= {}", val1.print(source, target, team, Step::none), val2.print(source, target, team, Step::none));
		}

		[[nodiscard]] inline bool eval(const Stats::Loadout &source, const Stats::Loadout &target, const Stats::Team &team) const {
			return val1.eval(source, target, team) <= val2.eval(source, target, team);
		}
	};
	template<FloatFormula T, FloatFormula V>
	struct Greater {
		T val1;
		V val2;
		[[nodiscard]] inline std::string print(const Stats::Loadout &source, const Stats::Loadout &target, const Stats::Team &team, Step) const {
			return fmt::format("{} > {}", val1.print(source, target, team, Step::none), val2.print(source, target, team, Step::none));
		}

		[[nodiscard]] inline bool eval(const Stats::Loadout &source, const Stats::Loadout &target, const Stats::Team &team) const {
			return val1.eval(source, target, team) > val2.eval(source, target, team);
		}
	};
	template<FloatFormula T, FloatFormula V>
	struct GreaterEqual {
		T val1;
		V val2;
		[[nodiscard]] inline std::string print(const Stats::Loadout &source, const Stats::Loadout &target, const Stats::Team &team, Step) const {
			return fmt::format("{} >= {}", val1.print(source, target, team, Step::none), val2.print(source, target, team, Step::none));
		}

		[[nodiscard]] inline bool eval(const Stats::Loadout &source, const Stats::Loadout &target, const Stats::Team &team) const {
			return val1.eval(source, target, team) >= val2.eval(source, target, team);
		}
	};

	template<BoolFormula T, BoolFormula V>
	struct And {
		T val1;
		V val2;
		[[nodiscard]] inline std::string print(const Stats::Loadout &source, const Stats::Loadout &target, const Stats::Team &team, Step) const {
			return fmt::format("{} && {}", val1.print(source, target, team, Step::none), val2.print(source, target, team, Step::none));
		}

		[[nodiscard]] inline bool eval(const Stats::Loadout &source, const Stats::Loadout &target, const Stats::Team &team) const {
			return val1.eval(source, target, team) && val2.eval(source, target, team);
		}
	};
	template<BoolFormula T, BoolFormula V>
	struct Or {
		T val1;
		V val2;
		[[nodiscard]] inline std::string print(const Stats::Loadout &source, const Stats::Loadout &target, const Stats::Team &team, Step) const {
			return fmt::format("{} || {}", val1.print(source, target, team, Step::none), val2.print(source, target, team, Step::none));
		}

		[[nodiscard]] inline bool eval(const Stats::Loadout &source, const Stats::Loadout &target, const Stats::Team &team) const {
			return val1.eval(source, target, team) || val2.eval(source, target, team);
		}
	};
}// namespace Formula