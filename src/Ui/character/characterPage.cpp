#include "characterPage.hpp"

#include "Ui/character/characterDetails.hpp"

#include "navigationView.hpp"
#include "wrapper.hpp"


using namespace squi;

UI::CharacterPage::operator squi::Child() const {
	auto storage = std::make_shared<Storage>();

	return NavigationView{
		.expanded = false,
		.backAction = [controller = controller]() {
			controller.pop();
		},
		.pages{
			NavigationView::Page{
				.name = "Details",
				.content = CharacterDetails{
					.characterKey = characterKey,
				},
			},
		},
	};
}
