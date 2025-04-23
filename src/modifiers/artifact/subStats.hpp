#pragma once

#include "formula/formulaContext.hpp"
#include "formula/percentage.hpp"
#include "modifiers/helpers.hpp"
#include "modifiers/statFactory.hpp"
#include "stats/loadout.hpp"
#include "stats/sheet.hpp"
#include <string>


namespace Modifiers::Artifact {
	struct CompiledSubStat : Formula::Compiled::FormulaBase<float> {
		SheetMember<Stats::Sheet<float>> stat;

		[[nodiscard]] float eval(const Formula::Context &context) const {
			float total = 0.f;
			for (const auto &artifact: context.source.artifact.sheet.equippedArtifacts) {
				if (!artifact.has_value()) continue;
				total += stat.resolve(*artifact.value());
			}
			return total;
		}
	};
	template<SheetMember<Stats::Sheet<float>> stat, SheetMemberIdentifier member>
	struct SubStatFormula {
		[[nodiscard]] Formula::Compiled::FloatNode compile(const Formula::Context &context) const {
			if (&context.active != &context.source) {
				return Formula::Compiled::ConstantFloat{.value = eval(context)};
			}
			return CompiledSubStat{
				.stat = stat,
			};
		}

		[[nodiscard]] std::string print(const Formula::Context &context, Formula::Step) const {
			return Formula::Percentage("Artifact Substat", eval(context), member.isPercentage());
		}

		[[nodiscard]] constexpr float eval(const Formula::Context &context) const {
			float total = 0.f;
			for (const auto &artifact: context.source.artifact.sheet.equippedArtifacts) {
				if (!artifact.has_value()) continue;
				total += stat.resolve(*artifact.value());
			}
			return total;
		}
	};

	static constexpr Modifiers::StatFactory<SubStatFormula, StatPointerFactory<Stats::Sheet<float>>{}, StatNameFactory{}> subStats{};
}// namespace Modifiers::Artifact