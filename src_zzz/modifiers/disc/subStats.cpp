#include "subStats.hpp"
#include "formula/percentage.hpp"
#include "modifiers/helpers.hpp"
#include "modifiers/statFactory.hpp"
#include "stats/loadout.hpp"

namespace Modifiers::Disc {
	struct SubStatFormula : Formula::FormulaBase<float> {
		SheetMember<Stats::Sheet<float>> stat;
		SheetMemberIdentifier member;

		[[nodiscard]] Formula::FloatNode fold(const Formula::Context &context, const Formula::FoldArgs &args) const {
			if (&context.origin != &context.source || !member.isDiscStat()) {
				return Formula::ConstantBase<float>{.value = eval(context)};
			}
			return *this;
		}

		[[nodiscard]] std::string print(const Formula::Context &context, Formula::Step) const {
			return Formula::Percentage("Disc Substat", eval(context), member.isPercentage());
		}

		[[nodiscard]] float eval(const Formula::Context &context) const {
			float total = 0.f;
			for (const auto &disc: context.source.loadout().disc.sheet.equippedDiscs) {
				if (!disc.has_value()) continue;
				total += stat.resolve(*disc.value());
			}
			return total;
		}
	};

	const Stats::Sheet<Formula::FloatNode> &subStats() {
		static auto ret = statFactory<Formula::FloatNode, SubStatFormula>(StatPointerFactory<Stats::Sheet<float>>{}, StatNameFactory{});
		return ret;
	}
}// namespace Modifiers::Disc