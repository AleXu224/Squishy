#include "loadoutCard.hpp"
#include "Ui/artifact/artifactCard.hpp"
#include "Ui/utils/card.hpp"
#include "Ui/weapon/weaponCard.hpp"
#include "store.hpp"
#include "widgets/button.hpp"
#include "widgets/expander.hpp"
#include "widgets/row.hpp"
#include "widgets/tooltip.hpp"


using namespace squi;

squi::core::Child UI::LoadoutCard::State::build(const Element &element) {
	auto &character = Store::characters.at(widget->characterKey);
	auto &loadout = character.state.loadoutByIndex(widget->loadoutIndex);

	return Expander{
		.title = widget->loadoutIndex.has_value() ? "Loadout name" : "Equipped",
		.alwaysExpanded = true,
		.action = Row{
			.children{
				widget->loadoutIndex.has_value() && std::holds_alternative<Stats::Artifact::Slotted>(loadout.artifact.equipped)//
					? Tooltip{
						  .text = "Sets the equipped build to this build. If any of the artifacts in this build are equipped on other characters, they will be swapped with this character's.",
						  .child = Button{
							  .onClick = [this, &character]() {
								  auto &loadout = character.state.loadoutByIndex(widget->loadoutIndex);
								  setState([&]() {
									  character.state.equippedLoadout.swapWeapon(loadout.weaponInstanceKey);
									  if (!std::holds_alternative<::Stats::Artifact::Slotted>(loadout.artifact.equipped)) return;
									  auto &slotted = std::get<::Stats::Artifact::Slotted>(loadout.artifact.equipped);
									  for (const auto &slot: Artifact::slots) {
										  auto &art = slotted.fromSlot(slot);
										  if (art) Store::artifacts.at(art).equipOn(character.instanceKey);
									  }
								  });
							  },
							  .child = "Equip",
						  },
					  }
					: Child{},
				Button{
					.disabled = character.state.loadoutIndex == widget->loadoutIndex,
					.onClick = [this, &character]() {
						character.state.loadoutIndex = widget->loadoutIndex;
						character.updateEvent.notify();
						character.loadoutChangedEvent.notify();
					},
					.child = "Activate",
				},
				widget->loadoutIndex.has_value()//
					? Button{
						  .theme = Button::Theme::Standard(),
						  .disabled = character.state.loadoutIndex == widget->loadoutIndex,
						  .onClick = [this, &character]() {
							  setState([&]() {
								  if (character.state.loadoutIndex.has_value() && character.state.loadoutIndex.value() > widget->loadoutIndex.value()) {
									  character.state.loadoutIndex.value()--;
								  }
								  character.state.loadouts.erase(character.state.loadouts.begin() + widget->loadoutIndex.value());
							  });
							  character.updateEvent.notify();
							  character.loadoutChangedEvent.notify();
						  },
						  .child = "Remove",
					  }
					: Child{},
			},
		},
		.content = Row{
			.widget{
				.width = Size::Expand,
				.padding = 8.f,
			},
			.spacing = 8.f,
			.children = [&]() {
				Children ret{
					WeaponCard{
						.widget{.width = Size::Expand, .height = Size::Expand},
						.weapon = Store::weapons.at(loadout.weaponInstanceKey),
						.actions = WeaponCard::Actions::showcase,
					},
				};

				std::visit(
					Utils::overloaded{
						[&](const Stats::Artifact::Slotted &slotted) {
							for (const auto &slot: Artifact::slots) {
								const auto &arti = slotted.fromSlot(slot);
								if (!arti) {
									ret.emplace_back(Card{});
									continue;
								}

								ret.emplace_back(ArtifactCard{
									.widget{.width = Size::Expand, .height = Size::Expand},
									.artifact = Store::artifacts.at(arti),
									.actions = ArtifactCard::Actions::showcase,
								});
							}
						},
						[&](const Stats::Artifact::Theorycraft &) {
							for (const auto &_: Artifact::slots) {
								ret.emplace_back(Child{});
							}
						},
					},
					loadout.artifact.equipped
				);

				return ret;
			}(),
		},
	};
}
