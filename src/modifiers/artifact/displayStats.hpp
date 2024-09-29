#pragma once

#include "modifiers/artifact/set.hpp"

namespace Modifiers::Artifact {
	template<auto sheetPre, auto sheetPost>
	struct DisplayStatsFrm {
		[[nodiscard]] std::string print(const Formula::Context &context, Formula::Step prevStep) const {
			return (sheetPre + sheetPost).print(context, prevStep);
		}

		using Ret = RetType<sheetPre>;

		[[nodiscard]] constexpr Ret eval(const Formula::Context &context) const {
			return (sheetPre + sheetPost).eval(context);
		}
	};

	static constexpr StatFactory<DisplayStatsFrm, Set::sheet1PreMods, Set::sheet1PostMods> display1;
	static constexpr StatFactory<DisplayStatsFrm, Set::sheet2PreMods, Set::sheet2PostMods> display2;
}// namespace Modifiers::Artifact