#include "characterPage.hpp"

#include "Ui/character/characterDetails.hpp"
#include "widgets/navigator.hpp"
#include "widgets/sideNav.hpp"


using namespace squi;

squi::core::Child UI::CharacterPage::State::build(const Element &element) {
	return SideNav{
		.backAction = [this]() {
			Navigator::of(this).popPage();
		},
		.defaultExpanded = false,
		.pages{
			SideNav::Page{
				.name = "Details",
				.content = CharacterDetails{
					.characterKey = widget->characterKey,
				},
			},
		},
	};
}
