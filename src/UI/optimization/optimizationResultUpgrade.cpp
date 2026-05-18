#include "optimizationResultUpgrade.hpp"

#include "UI/artifact/artifactCard.hpp"
#include "UI/utils/displayCard.hpp"
#include "UI/utils/skillEntry.hpp"
#include "widgets/column.hpp"


using namespace squi;

[[nodiscard]] squi::core::Child UI::OptimizationResultUpgrade::build(const Element &element) const {
	return DisplayCard{
		.widget = widget,
		.title = std::format("Result #{}", entryIndex),
		.children{
			Column{
				.widget{
					.height = Size::Shrink,
					.padding = 4.f,
				},
				.spacing = 4.f,
				.children{
					SkillEntry{
						.isTransparent = false,
						.name = "Upgrade chance",
						.value = solution.upgradeChance,
						.isPercentage = true,
					},
					SkillEntry{
						.isTransparent = true,
						.name = "Average upgrade",
						.value = solution.upgradeAverage,
						.isPercentage = true,
					},
					ArtifactCard{
						.artifact = solution.artifact,
						.actions = ArtifactCard::Actions::list,
					},
				},
			},
		},
	};
}
