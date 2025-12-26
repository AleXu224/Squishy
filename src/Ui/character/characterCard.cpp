#include "characterCard.hpp"

#include "Ui/utils/card.hpp"
#include "Ui/utils/statDisplay.hpp"
#include "Ui/utils/trueFalse.hpp"
#include "characterCardBanner.hpp"
#include "characterEditor.hpp"
#include "characterPage.hpp"
#include "formula/stat.hpp"
#include "store.hpp"

#include "widgets/button.hpp"
#include "widgets/column.hpp"
#include "widgets/gestureDetector.hpp"
#include "widgets/navigator.hpp"
#include "widgets/row.hpp"

using namespace squi;

struct CharacterCardsContents : StatelessWidget {
	// Args
	Key key;
	Character::InstanceKey characterKey;

	[[nodiscard]] Child build(const Element &element) const {
		auto &character = ::Store::characters.at(characterKey);
		auto statsToDisplay = std::vector{Stats::characterDisplayStats, {Stats::fromElement(character.state.stats.base.element)}};
		Team::Instance placeholderTeam{};
		placeholderTeam.stats.characters.at(0) = &character;
		Formula::Context ctx{
			.source = character.state,
			.active = character.state,
			.team = placeholderTeam.stats,
			.enemy = ::Store::enemies.at(0).stats,
		};

		return Column{
			.children{
				Gesture{
					.onClick = [&](Gesture::State) {
						Navigator::of(element).push(UI::CharacterPage{
							.characterKey = characterKey,
						});
					},
					.child = UI::CharacterCardBanner{
						.characterKey = characterKey,
					},
				},
				Column{
					.children = [&]() {
						Children ret;
						for (const auto &[stat, transparent]: std::views::zip(
								 std::views::join(statsToDisplay),
								 Utils::trueFalse
							 )) {
							auto formula = Formula::Stat{stat};
							ret.emplace_back(UI::StatDisplay{
								.isTransparent = transparent,
								.stat = StatValue{.stat = stat, .value = formula.eval(ctx)},
							});
						}
						return ret;
					}(),
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
							.onClick = [this, &element]() {
								auto &character = ::Store::characters.at(characterKey);
								Navigator::of(element).pushOverlay(UI::CharacterEditor{
									.character = character,
									.onSubmit = [](const Character::Instance &character) {
										auto &instance = Store::characters.at(character.instanceKey);
										instance.state.stats.sheet = character.state.stats.sheet;
										instance.updateEvent.notify();
									},
								});
							},
							.child = "Edit",
						},
						Button{
							.theme = Button::Theme::Standard(),
							.onClick = [this]() {
								auto &character = Store::characters.at(characterKey);
								std::get<Stats::Artifact::Slotted>(character.state.equippedLoadout.artifact.equipped).unequipAll();
								Store::characters.erase(characterKey);
								Store::characterListUpdateEvent.notify();
							},
							.child = "Delete",
						},
					},
				}
			},
		};
	}
};

squi::core::Child UI::CharacterCard::State::build(const Element &element) {
	return Card{
		.child = CharacterCardsContents{
			.characterKey = widget->characterKey,
		},
	};
}
