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

		[[nodiscard]] std::string print(const Context &context, Step) const {
			auto cond = requirement.eval(context);
			return fmt::format("{}", cond ? fmt::format("{} ({})", requirement.print(context, Step::none), ret.print(context, Step::none)) : "");
		}

		[[nodiscard]] RetType eval(const Context &context) const {
			if (requirement.eval(context))
				return ret.eval(context);

			return RetType{};
		}
	};
}// namespace Formula

namespace Requirement {
	static constexpr struct impl_Passive1 {
		[[nodiscard]] static std::string print(const Formula::Context &, Formula::Step) {
			return "Passive 1";
		}

		[[nodiscard]] static bool eval(const Formula::Context &context) {
			return context.source.character.sheet.ascension >= 1;
		}
	} passive1{};
	static constexpr struct impl_Passive2 {
		[[nodiscard]] static std::string print(const Formula::Context &, Formula::Step) {
			return "Passive 2";
		}

		[[nodiscard]] static bool eval(const Formula::Context &context) {
			return context.source.character.sheet.ascension >= 4;
		}
	} passive2{};
	static constexpr struct impl_Constellation1 {
		[[nodiscard]] static std::string print(const Formula::Context &, Formula::Step) {
			return "Constellation 1";
		}

		[[nodiscard]] static bool eval(const Formula::Context &context) {
			return context.source.character.sheet.constellation >= 1;
		}
	} constellation1{};
	static constexpr struct impl_Constellation2 {
		[[nodiscard]] static std::string print(const Formula::Context &, Formula::Step) {
			return "Constellation 2";
		}

		[[nodiscard]] static bool eval(const Formula::Context &context) {
			return context.source.character.sheet.constellation >= 2;
		}
	} constellation2{};
	static constexpr struct impl_Constellation3 {
		[[nodiscard]] static std::string print(const Formula::Context &, Formula::Step) {
			return "Constellation 3";
		}

		[[nodiscard]] static bool eval(const Formula::Context &context) {
			return context.source.character.sheet.constellation >= 3;
		}
	} constellation3{};
	static constexpr struct impl_Constellation4 {
		[[nodiscard]] static std::string print(const Formula::Context &, Formula::Step) {
			return "Constellation 4";
		}

		[[nodiscard]] static bool eval(const Formula::Context &context) {
			return context.source.character.sheet.constellation >= 4;
		}
	} constellation4{};
	static constexpr struct impl_Constellation5 {
		[[nodiscard]] static std::string print(const Formula::Context &, Formula::Step) {
			return "Constellation 5";
		}

		[[nodiscard]] static bool eval(const Formula::Context &context) {
			return context.source.character.sheet.constellation >= 5;
		}
	} constellation5{};
	static constexpr struct impl_Constellation6 {
		[[nodiscard]] static std::string print(const Formula::Context &, Formula::Step) {
			return "Constellation 6";
		}

		[[nodiscard]] static bool eval(const Formula::Context &context) {
			return context.source.character.sheet.constellation >= 6;
		}
	} constellation6{};
}// namespace Requirement