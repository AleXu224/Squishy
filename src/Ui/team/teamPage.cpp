#include "teamPage.hpp"

#include "Ui/character/characterDetails.hpp"

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
			auto &team = Store::teams.at(teamKey);
			for (auto &character: team.stats.characters) {
				if (character.has_value()) {
					pages.emplace_back(NavigationView::Page{
						.name = character->get().stats.character.data.name,
						.icon = Image{
							.widget{
								.width = 16.f,
								.height = 16.f,
							},
							.fit = squi::Image::Fit::contain,
							.image = Image::Data::fromFileAsync(std::format("assets/Characters/{}/avatar.png", character->get().stats.character.data.name)),
						},
						.content = CharacterDetails{
							.characterKey = character->get().key,
						},
					});
				}
			}
			return pages;
		}(),
	};
}