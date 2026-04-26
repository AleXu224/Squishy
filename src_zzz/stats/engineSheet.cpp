#include "engineSheet.hpp"

#include "engine/data.hpp"
#include "formula/base.hpp"
#include "formula/custom.hpp"
#include "formula/prefix.hpp"
#include "stats/engine.hpp"
#include "stats/loadout.hpp"


constexpr auto getEngineAtk = [](const Stats::Engine &stats) {
	return stats.data->baseStats.getAtkAt(stats.sheet.level, stats.sheet.modification);
};
constexpr auto getEngineSubstat = [](const Stats::Engine &stats) {
	return stats.data->baseStats.getSubstatAt(stats.sheet.level, stats.sheet.modification);
};

Stats::EngineSheet::EngineSheet(const EngineBase &base) {
	this->base.atk.modifiers.at(0) = Formula::Prefix{
		.prefix = "Engine Base",
		.val = Formula::Custom{
			.foldFunc = [](const Formula::Context &context, const Formula::FoldArgs &args) -> Formula::FloatNode {
				return Formula::ConstantFlat{.value = getEngineAtk(*context.source.loadout().engine)};
			},
			.func = [](const Formula::Context &context) {
				return getEngineAtk(*context.source.loadout().engine);
			}
		}
	};

	this->initial.fromStat(base.subStat.stat.value()).modifiers.at(0) = Formula::Prefix{
		.prefix = "Engine Base",
		.val = Formula::Custom{
			.foldFunc = [](const Formula::Context &context, const Formula::FoldArgs &args) -> Formula::FloatNode {
				return Formula::ConstantFlat{.value = getEngineSubstat(*context.source.loadout().engine)};
			},
			.func = [](const Formula::Context &context) {
				return getEngineSubstat(*context.source.loadout().engine);
			},
			.isPercentage = Utils::isPercentage(base.subStat.stat)
		}
	};
}
