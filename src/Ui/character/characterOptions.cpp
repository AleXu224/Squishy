#include "characterOptions.hpp"

#include "Ui/utils/displayCard.hpp"
#include "Ui/utils/selector.hpp"


using namespace squi;

UI::CharacterOptions::operator squi::Child() const {
	auto storage = std::make_shared<Storage>();

	return UI::DisplayCard{
		.title = "Options",
		.children{
			Column{
				.widget{
					.padding = Padding{4.f},
				},
				.spacing = 4.f,
				.children{
					CharacterLevelSelector{.characterKey = characterKey},
					ConstellationSelector{.characterKey = characterKey},
					WeaponLevelSelector{.characterKey = characterKey},
					RefinementSelector{.characterKey = characterKey},
					NormalTalentLevelSelector{.characterKey = characterKey},
					SkillTalentLevelSelector{.characterKey = characterKey},
					BurstTalentLevelSelector{.characterKey = characterKey},
				},
			},
		},
	};
}
