#include "subStats.hpp"
#include "formula/percentage.hpp"
#include "modifiers/statFactory.hpp"
#include "stats/loadout.hpp"

namespace Modifiers::Disc {
	struct SubStatFormulaPtr : Formula::FormulaBase<float> {
		float Stats::SheetSimplified<float>::*ptr;
		Stat stat;

		[[nodiscard]] Formula::FloatNode fold(const Formula::Context &context, const Formula::FoldArgs &args) const {
			if (&context.origin != &context.source) {
				return Formula::ConstantBase<float>{.value = eval(context)};
			}
			return *this;
		}

		[[nodiscard]] std::string print(const Formula::Context &context, Formula::Step) const {
			return Formula::Percentage(std::format("Disc {}", Utils::Stringify(stat)), eval(context), Utils::isPercentage(stat));
		}

		[[nodiscard]] float eval(const Formula::Context &context) const {
			float total = 0.f;
			for (const auto &disc: context.source.loadout().disc.sheet.equippedDiscs) {
				if (!disc.has_value()) continue;
				total += std::invoke(ptr, *disc.value());
			}
			return total;
		}
	};

	struct SubStatFormulaSimple : Formula::FormulaBase<float> {
		Stat stat;

		[[nodiscard]] Formula::FloatNode fold(const Formula::Context &context, const Formula::FoldArgs &args) const {
			if (&context.origin != &context.source) {
				return Formula::ConstantBase<float>{.value = eval(context)};
			}
			return SubStatFormulaPtr{
				.ptr = Stats::ptrFromStatSimplified<Stats::SheetSimplified<float>>(stat),
				.stat = stat,
			};
		}

		[[nodiscard]] std::string print(const Formula::Context &context, Formula::Step) const {
			return Formula::Percentage(std::format("Disc {}", Utils::Stringify(stat)), eval(context), Utils::isPercentage(stat));
		}

		[[nodiscard]] float eval(const Formula::Context &context) const {
			float total = 0.f;
			for (const auto &disc: context.source.loadout().disc.sheet.equippedDiscs) {
				if (!disc.has_value()) continue;
				total += Stats::fromStatSimplified(*disc.value(), stat);
			}
			return total;
		}
	};

	const Stats::Sheet<Formula::FloatNode> &subStats() {
		static auto ret = [&]() {
			auto ret = constantStatFactory<Formula::FloatNode>(Formula::Constant{.value = 0.f});

			for (const auto &stat: Stats::Disc::mainStats) {
				Stats::fromStat(ret, stat) = SubStatFormulaSimple{.stat = stat};
			}
			for (const auto &stat: Stats::Disc::subStats) {
				Stats::fromStat(ret, stat) = SubStatFormulaSimple{.stat = stat};
			}

			return ret;
		}();

		return ret;
	}
}// namespace Modifiers::Disc