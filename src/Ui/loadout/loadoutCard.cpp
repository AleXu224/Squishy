#include "loadoutCard.hpp"
#include "Ui/artifact/artifactCard.hpp"
#include "Ui/utils/card.hpp"
#include "Ui/utils/tooltip.hpp"
#include "Ui/weapon/weaponCard.hpp"
#include "button.hpp"
#include "container.hpp"
#include "expander.hpp"
#include "row.hpp"
#include "store.hpp"


using namespace squi;

UI::LoadoutCard::operator squi::Child() const {
	auto &character = Store::characters.at(characterKey);
	auto &loadout = character.state.loadoutByIndex(loadoutIndex);

	return Expander{
		.heading = loadoutIndex.has_value() ? "Loadout name" : "Equipped",
		.alwaysExpanded = true,
		.actions{
			loadoutIndex.has_value() && std::holds_alternative<Stats::Artifact::Slotted>(loadout.artifact.equipped)//
				? Tooltip{
					  .message = "Sets the equipped build to this build. If any of the artifacts in this build are equipped on other characters, they will be swapped with this character's.",
					  .child = Button{
						  .text = "Equip",
						  .onClick = [&character, loadoutIndex = loadoutIndex](GestureDetector::Event) {
							  auto &loadout = character.state.loadoutByIndex(loadoutIndex);
							  character.state.equippedLoadout.swapWeapon(loadout.weaponInstanceKey);
							  if (!std::holds_alternative<::Stats::Artifact::Slotted>(loadout.artifact.equipped)) return;
							  auto &slotted = std::get<::Stats::Artifact::Slotted>(loadout.artifact.equipped);
							  for (const auto &slot: Artifact::slots) {
								  auto &art = slotted.fromSlot(slot);
								  if (art) Store::artifacts.at(art).equipOn(character.instanceKey);
							  }
						  },
					  },
				  }
				: Child{},
			Button{
				.text = "Activate",
				.disabled = character.state.loadoutIndex == loadoutIndex,
				.onClick = [&character, loadoutIndex = loadoutIndex](GestureDetector::Event) {
					character.state.loadoutIndex = loadoutIndex;
					character.updateEvent.notify();
					character.loadoutChangedEvent.notify();
				},
			},
			loadoutIndex.has_value()//
				? Button{
					  .text = "Remove",
					  .style = ButtonStyle::Standard(),
					  .disabled = character.state.loadoutIndex == loadoutIndex,
					  .onClick = [&character, loadoutIndex = loadoutIndex](GestureDetector::Event) {
						  if (character.state.loadoutIndex.has_value() && character.state.loadoutIndex.value() > loadoutIndex.value()) {
							  character.state.loadoutIndex.value()--;
						  }
						  character.state.loadouts.erase(character.state.loadouts.begin() + loadoutIndex.value());
						  character.updateEvent.notify();
						  character.loadoutChangedEvent.notify();
					  },
				  }
				: Child{},
		},
		.expandedContent = Row{
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
								ret.emplace_back(Container{});
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
