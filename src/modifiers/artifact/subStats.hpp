#pragma once

#include "formula/formulaContext.hpp"
#include "formula/percentage.hpp"
#include "modifiers/helpers.hpp"
#include "modifiers/statFactory.hpp"
#include "stats/loadout.hpp"
#include "stats/sheet.hpp"
#include <string>


namespace Modifiers::Artifact {
	template<SheetMember<Stats::Sheet<float>> stat, SheetMemberIdentifier member>
	struct SubStatFormula {
		[[nodiscard]] std::string print(const Formula::Context &context, Formula::Step) const {
			return Formula::Percentage("Artifact Substat", eval(context), member.isPercentage());
		}

		[[nodiscard]] constexpr float eval(const Formula::Context &context) const {
			float total = 0.f;
			for (const auto &artifact: context.target.artifact.sheet.equippedArtifacts) {
				total += artifact.transform([&](Stats::Sheet<float> *sheet) {
									 return stat.resolve(*sheet);
								 })
							 .value_or(0.f);
			}
			return total;
		}
	};

	static constexpr Modifiers::StatFactory<SubStatFormula, StatPointerFactory<Stats::Sheet<float>>{}, StatNameFactory{}> subStats{};
}// namespace Modifiers::Artifact