#pragma once

#include "formula/compiled/constantOrValue.hpp"
#include "formula/formulaContext.hpp"
#include "formula/percentage.hpp"
#include "modifiers/helpers.hpp"
#include "modifiers/statFactory.hpp"
#include "stats/loadout.hpp"
#include "stats/sheet.hpp"
#include <string>


namespace Modifiers::Artifact {
	struct CompiledSubStat {
		SheetMember<Stats::Sheet<float>> stat;

		[[nodiscard]] float eval(const Formula::Context &context) const {
			float total = 0.f;
			for (const auto &artifact: context.source.artifact.sheet.equippedArtifacts) {
				if (!artifact.has_value()) continue;
				total += stat.resolve(*artifact.value());
			}
			return total;
		}

		[[nodiscard]] bool isConstant() const {
			return false;
		}
	};
	template<SheetMember<Stats::Sheet<float>> stat, SheetMemberIdentifier member>
	struct SubStatFormula {
		using CompiledRet = Formula::Compiled::ConstantOr<float, CompiledSubStat>;

		[[nodiscard]] CompiledRet compile(const Formula::Context &context) const {
			if (&context.active != &context.source) {
				return CompiledRet{Formula::Compiled::ConstantFloat{eval(context)}};
			}
			return CompiledRet{
				CompiledSubStat{
					.stat = stat,
				},
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