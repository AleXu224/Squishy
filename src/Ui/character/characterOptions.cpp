#include "characterOptions.hpp"

#include "Ui/utils/displayCard.hpp"
#include "Ui/utils/selector.hpp"


using namespace squi;

squi::core::Child UI::CharacterOptions::State::build(const Element &element) {
	return UI::DisplayCard{
		.title = "Options",
		.children{
			Column{
				.widget{
					.padding = Padding{4.f},
				},
				.spacing = 4.f,
				.children{
					CharacterLevelSelector{.characterKey = widget->characterKey},
					ConstellationSelector{.characterKey = widget->characterKey},
					WeaponLevelSelector{.characterKey = widget->characterKey},
					RefinementSelector{.characterKey = widget->characterKey},
					NormalTalentLevelSelector{.characterKey = widget->characterKey},
					SkillTalentLevelSelector{.characterKey = widget->characterKey},
					BurstTalentLevelSelector{.characterKey = widget->characterKey},
				},
			},
		},
	};
}
