#include "subStats.hpp"
#include "formula/percentage.hpp"
#include "modifiers/helpers.hpp"
#include "modifiers/statFactory.hpp"
#include "stats/loadout.hpp"

namespace Modifiers::Artifact {
	struct SubStatFormula : Formula::FormulaBase<float> {
		SheetMember<Stats::Sheet<float>> stat;
		SheetMemberIdentifier member;

		[[nodiscard]] Formula::FloatNode fold(const Formula::Context &context, const Formula::FoldArgs &args) const {
			if (&context.origin != &context.source || !member.isArtifactStat()) {
				return Formula::ConstantBase<float>{.value = eval(context)};
			}
			return *this;
		}

		[[nodiscard]] std::string print(const Formula::Context &context, Formula::Step) const {
			return Formula::Percentage("Artifact Substat", eval(context), member.isPercentage());
		}

		[[nodiscard]] float eval(const Formula::Context &context) const {
			float total = 0.f;
			for (const auto &artifact: context.source.loadout().artifact.sheet.equippedArtifacts) {
				if (!artifact.has_value()) continue;
				total += stat.resolve(*artifact.value());
			}
			return total;
		}
	};

	const Stats::Sheet<Formula::FloatNode> &subStats() {
		static auto ret = statFactory<Formula::FloatNode, SubStatFormula>(StatPointerFactory<Stats::Sheet<float>>{}, StatNameFactory{});
		return ret;
	}
}// namespace Modifiers::Artifact