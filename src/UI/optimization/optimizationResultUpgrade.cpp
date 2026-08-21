#include "optimizationResultUpgrade.hpp"

#include "UI/artifact/artifactCard.hpp"
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

	auto hasChosenSubstats = false;
	std::string subTitle;

	std::string type;
	switch (solution.kind) {
		case ::Optimization::SolutionUpgrade::Kind::definition:
			type = "Definition";
			subTitle = std::format("{} — {} ({})", Utils::Stringify(solution.slot), Utils::Stringify(solution.mainStat), Artifact::sets.at(solution.set).name);
			hasChosenSubstats = true;
			break;
		case ::Optimization::SolutionUpgrade::Kind::reroll:
			type = "Reroll";
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
							  .name = Utils::Stringify(solution.chosenSubStats.at(0)),
						  }
						: Child{},
					hasChosenSubstats//
						? SkillEntry{
							  .isTransparent = true,
							  .name = Utils::Stringify(solution.chosenSubStats.at(1)),
						  }
						: Child{},
					solution.kind != ::Optimization::SolutionUpgrade::Kind::definition//
						? ArtifactCard{
							  .artifact = solution.artifact,
							  .actions = ArtifactCard::Actions::list,
						  }
						: Child{},
				},
			},
		},
	};
}
