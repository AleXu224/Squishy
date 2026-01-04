#include "teamCard.hpp"

#include "Ui/utils/card.hpp"
#include "Ui/utils/skillHeader.hpp"
#include "character/data.hpp"
#include "core/app.hpp"
#include "teamDetails.hpp"
#include "teamEditor.hpp"

#include "store.hpp"
#include "widgets/box.hpp"
#include "widgets/button.hpp"
#include "widgets/column.hpp"
#include "widgets/gestureDetector.hpp"
#include "widgets/image.hpp"
#include "widgets/navigator.hpp"
#include "widgets/row.hpp"


using namespace squi;

struct TeamAvatar : StatelessWidget {
	// Args
	Key key;
	Character::Instance *character;

	[[nodiscard]] Child build(const Element &) const {
		return Box{
			.widget{
				.width = 64.f,
				.height = 64.f,
			},
			.color{1.f, 1.f, 1.f, 0.1f},
			.borderRadius{4.f},
			.child = character
					   ? Image{
							 .fit = Image::Fit::contain,
							 .image = ImageProvider::fromFile(std::format("assets/Characters/{}/avatar.png", character->state.stats.data.name))
						 }
					   : Child{},
		};
	}
};

squi::core::Child UI::TeamCard::State::build(const Element &element) {
	auto &team = ::Store::teams.at(widget->teamKey);

	return Card{
		.child = Column{
			.children{
				Gesture{
					.onClick = [this](Gesture::State) {
						Navigator::of(this).push(UI::TeamDetails{
							.teamKey = widget->teamKey,
						});
					},
					.onUpdate = [this](Gesture::State state) {
						if (state.hovered && state.isKeyPressedOrRepeat(GestureMouseKey::middle)) {
							auto thread = std::thread([teamKey = widget->teamKey]() {
								std::optional<App> app;
								App::addMainThreadTask([&]() {
									app.emplace(
										Engine::WindowOptions{
											.name = "Team Details",
											.width = 1280,
											.height = 720,
										},
										UI::TeamDetails{
											.enableBackButton = false,
											.teamKey = teamKey,
										}
									);
								}).wait();
								app->initialize();
								app->finished.wait();
							});
							thread.detach();
						}
					},
					.child = UI::SkillHeader{
						.name = team.name,
					},
				},
				Row{
					.widget{
						.height = Size::Shrink,
						.padding = 8.f,
					},
					.justifyContent = Row::JustifyContent::spaceBetween,
					.children{
						TeamAvatar{.character = team.stats.characters.at(0)},
						TeamAvatar{.character = team.stats.characters.at(1)},
						TeamAvatar{.character = team.stats.characters.at(2)},
						TeamAvatar{.character = team.stats.characters.at(3)},
					},
				},
				Row{
					.widget{
						.height = Size::Shrink,
						.padding = 4.f,
					},
					.spacing = 4.f,
					.children{
						Button{
							.theme = Button::Theme::Standard(),
							.onClick = [this]() {
								Navigator::of(this).pushOverlay(UI::TeamEditor{
									.instance = ::Store::teams.at(widget->teamKey),
									.onSubmit = [](const Team::Instance &team) {
										auto &teamInStore = ::Store::teams.at(team.instanceKey) = team;
										teamInStore.updateEvent.notify();
										::Store::teamListUpdateEvent.notify();
									},
								});
							},
							.child = "Edit",
						},
						Button{
							.theme = Button::Theme::Standard(),
							.onClick = [this]() {
								::Store::teams.erase(widget->teamKey);
								::Store::teamListUpdateEvent.notify();
							},
							.child = "Delete",
						},
					},
				},
			},
		},
	};
}
