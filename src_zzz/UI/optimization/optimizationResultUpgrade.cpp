#include "optimizationResultUpgrade.hpp"

#include "UI/disc/discCard.hpp"
#include "UI/utils/displayCard.hpp"
#include "UI/utils/skillEntry.hpp"
#include "widgets/column.hpp"


using namespace squi;

[[nodiscard]] squi::core::Child UI::OptimizationResultUpgrade::build(const Element &element) const {
	Child chanceEntry = SkillEntry{
		.isTransparent = false,
		.name = "Upgrade chance",
		.value = solution.upgradeChance,
		.isPercentage = true,
	};
	Child averageEntry = SkillEntry{
		.isTransparent = true,
		.name = "Average upgrade",
		.value = solution.upgradeAverage,
		.isPercentage = true,
	};

	std::string type;
	std::string subTitle;
	bool hasChosenSubstats = false;
	switch (solution.kind) {
		case ::Optimization::SolutionUpgrade::Kind::definition:
			type = "Definition";
			subTitle = std::format("{} - {} ({})", Utils::Stringify(solution.partition), Utils::Stringify(solution.mainStat), Disc::sets.at(solution.set).name);
			hasChosenSubstats = true;
			break;
		case ::Optimization::SolutionUpgrade::Kind::levelUp:
			type = "Upgrade";
			break;
	}

	return DisplayCard{
		.widget = widget,
		.title = std::format("Result #{} ({})", entryIndex, type),
		.subtitle = subTitle,
		.children{
			Column{
				.widget{
					.height = Size::Shrink,
					.padding = 4.f,
				},
				.spacing = 4.f,
				.children{
					chanceEntry,
					averageEntry,
					hasChosenSubstats//
						? SkillEntry{
							  .isTransparent = false,
							  .name = solution.chosenCount > 0 ? Utils::Stringify(solution.chosenSubStats.at(0)) : std::string{"No targeted substats"},
						  }
						: Child{},
					hasChosenSubstats && solution.chosenCount > 1//
						? SkillEntry{
							  .isTransparent = true,
							  .name = Utils::Stringify(solution.chosenSubStats.at(1)),
						  }
						: Child{},
					hasChosenSubstats//
						? Child{}
						: DiscCard{
							  .disc = solution.disc,
							  .actions = DiscCard::Actions::list,
						  },
				},
			},
		},
	};
}
