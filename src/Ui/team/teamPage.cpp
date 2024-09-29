#include "teamPage.hpp"

#include "Ui/character/characterDetails.hpp"

#include "character/data.hpp"
#include "image.hpp"
#include "navigationView.hpp"
#include "store.hpp"
#include "wrapper.hpp"


using namespace squi;

UI::TeamPage::operator squi::Child() const {
	auto storage = std::make_shared<Storage>();

	return NavigationView{
		.expanded = false,
		.backAction = [controller = controller]() {
			controller.pop();
		},
		.pages = [teamKey = teamKey]() {
			std::vector<NavigationView::Page> pages{};
			auto &team = ::Store::teams.at(teamKey);
			for (auto &character: team.stats.characters) {
				if (character) {
					pages.emplace_back(NavigationView::Page{
						.name = character->loadout.character.data.name,
						.icon = Image{
							.widget{
								.width = 16.f,
								.height = 16.f,
							},
							.fit = squi::Image::Fit::contain,
							.image = ImageProvider::fromFile(std::format("assets/Characters/{}/avatar.png", character->loadout.character.data.name)),
						},
						.content = CharacterDetails{
							.characterKey = character->instanceKey,
						},
					});
				}
			}
			return pages;
		}(),
	};
}
