#include "teamDetails.hpp"

#include "Ui/character/characterDetails.hpp"

#include "character/data.hpp"
#include "store.hpp"
#include "teamBuffs.hpp"
#include "widgets/image.hpp"
#include "widgets/navigator.hpp"
#include "widgets/sideNav.hpp"


using namespace squi;
squi::core::Child UI::TeamDetails::State::build(const Element &element) {
	return SideNav{
		.backAction = !widget->enableBackButton//
						? std::function<void()>{}
						: [this]() {
							  Navigator::of(this).pop();
						  },
		.defaultExpanded = false,
		.pages = [&]() {
			std::vector<SideNav::Page> pages{};
			pages.emplace_back(SideNav::Page{
				.name = "Team Buffs",
				.icon = 0xe7ef,
				.content = TeamBuffs{
					.instanceKey = widget->teamKey,
				},
			});
			auto &team = ::Store::teams.at(widget->teamKey);
			for (auto &character: team.stats.characters) {
				if (character) {
					pages.emplace_back(SideNav::Page{
						.name = std::string(character->state.stats.data.name),
						.icon = Image{
							.widget{
								.width = 16.f,
								.height = 16.f,
							},
							.fit = squi::Image::Fit::contain,
							.image = ImageProvider::fromFile(std::format("assets/Characters/{}/avatar.png", character->state.stats.data.name)),
						},
						.content = CharacterDetails{
							.key = IndexKey{static_cast<int64_t>(character->instanceKey.key)},
							.characterKey = character->instanceKey,
							.teamKey = widget->teamKey,
						},
					});
				}
			}
			return pages;
		}(),
	};
}
