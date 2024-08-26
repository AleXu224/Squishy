#pragma once

#include "fmt/core.h"
#include "intermediary.hpp"
#include "stats/loadout.hpp"


namespace Formula {
	template<BoolFormula T, class V>
	struct Requires {
		T requirement;
		V ret;

		using RetType = decltype(std::declval<V>().eval(std::declval<const Context &>()));

		[[nodiscard]] inline std::string print(const Context &context, Step) const {
			auto cond = requirement.eval(context);
			return fmt::format("{}", cond ? fmt::format("{} : ({})", requirement.print(context, Step::none), ret.print(context, Step::none)) : "");
		}

		[[nodiscard]] inline RetType eval(const Context &context) const {
			if (requirement.eval(context))
				return ret.eval(context);

			return RetType{};
		}
	};
}// namespace Formula

namespace Requirement {
	struct Passive1 {
		[[nodiscard]] static inline std::string print(const Formula::Context &, Formula::Step) {
			return "Passive 1";
		}

		[[nodiscard]] static inline bool eval(const Formula::Context &context) {
			return context.source.character.sheet.ascension >= 1;
		}
	};
	struct Passive2 {
		[[nodiscard]] static inline std::string print(const Formula::Context &, Formula::Step) {
			return "Passive 2";
		}

		[[nodiscard]] static inline bool eval(const Formula::Context &context) {
			return context.source.character.sheet.ascension >= 4;
		}
	};
	struct Constellation1 {
		[[nodiscard]] static inline std::string print(const Formula::Context &, Formula::Step) {
			return "Constellation 1";
		}

		[[nodiscard]] static inline bool eval(const Formula::Context &context) {
			return context.source.character.sheet.constellation >= 1;
		}
	};
	struct Constellation2 {
		[[nodiscard]] static inline std::string print(const Formula::Context &, Formula::Step) {
			return "Constellation 2";
		}

		[[nodiscard]] static inline bool eval(const Formula::Context &context) {
			return context.source.character.sheet.constellation >= 2;
		}
	};
	struct Constellation3 {
		[[nodiscard]] static inline std::string print(const Formula::Context &, Formula::Step) {
			return "Constellation 3";
		}

		[[nodiscard]] static inline bool eval(const Formula::Context &context) {
			return context.source.character.sheet.constellation >= 3;
		}
	};
	struct Constellation4 {
		[[nodiscard]] static inline std::string print(const Formula::Context &, Formula::Step) {
			return "Constellation 4";
		}

		[[nodiscard]] static inline bool eval(const Formula::Context &context) {
			return context.source.character.sheet.constellation >= 4;
		}
	};
	struct Constellation5 {
		[[nodiscard]] static inline std::string print(const Formula::Context &, Formula::Step) {
			return "Constellation 5";
		}

		[[nodiscard]] static inline bool eval(const Formula::Context &context) {
			return context.source.character.sheet.constellation >= 5;
		}
	};
	struct Constellation6 {
		[[nodiscard]] static inline std::string print(const Formula::Context &, Formula::Step) {
			return "Constellation 6";
		}

		[[nodiscard]] static inline bool eval(const Formula::Context &context) {
			return context.source.character.sheet.constellation >= 6;
		}
	};
}// namespace Requirement