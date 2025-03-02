#include "artifactCard.hpp"

#include "Ui/artifact/artifactEditor.hpp"
#include "Ui/character/characterSelector.hpp"
#include "Ui/utils/card.hpp"
#include "Ui/utils/statDisplay.hpp"
#include "align.hpp"
#include "artifact/sets.hpp"
#include "box.hpp"
#include "button.hpp"
#include "character/data.hpp"
#include "column.hpp"
#include "gestureDetector.hpp"
#include "image.hpp"
#include "misc/rarityToColor.hpp"
#include "row.hpp"
#include "stack.hpp"
#include "store.hpp"
#include "text.hpp"


#include "ranges"

using namespace squi;

struct ArtifactHeader {
	// Args
	Artifact::SetKey set;
	Artifact::Slot slot;
	StatValue mainStat;
	uint8_t rarity;

	struct Storage {
		// Data
	};

	operator squi::Child() const {
		auto storage = std::make_shared<Storage>();

		auto icon = Align{
			.xAlign = 1.f,
			.child = Image{
				.fit = squi::Image::Fit::contain,
				.image = ImageProvider::fromFile(std::format("assets/Artifacts/{}/{}.png", Artifact::sets.at(set).name, Utils::Stringify(slot))),
			},
		};
		auto title = Align{
			.xAlign = 0.f,
			.child = Column{
				.widget{
					.height = Size::Shrink,
					.padding = Padding{12.f, 0.f},
				},
				.children{
					Text{
						.text = Utils::Stringify(mainStat.stat),
					},
					Text{
						.text = Utils::Stringify(mainStat),
						.fontSize = 24.f,
					},
				},
			},
		};
		return Box{
			.widget{
				.height = 64.f,
			},
			.color = Misc::rarityToColor.at(rarity),
			.borderRadius{7.f, 7.f, 0.f, 0.f},
			.child = Stack{
				.children{
					icon,
					title,
				},
			},
		};
	}
};

struct ArtifactCardContent {
	// Args
	squi::Widget::Args widget{};
	Artifact::Instance &artifact;

	operator squi::Child() const {
		auto header = ArtifactHeader{
			.set = artifact.set,
			.slot = artifact.slot,
			.mainStat{
				.stat = artifact.mainStat,
				.value = Stats::Values::mainStat.at(artifact.mainStat).at(artifact.level),
			},
			.rarity = artifact.rarity,
		};
		auto subStats = Column{
			.widget{
				.padding = Padding{4.f},
				.onInit = [artifact = artifact](Widget &w) {
					auto trueFalse = {false, true};
					auto trueFalseInf = std::views::join(std::views::repeat(trueFalse));

					for (const auto &[transparent, substat]: std::views::zip(trueFalseInf, artifact.subStats)) {
						if (!substat.has_value()) continue;
						w.addChild(UI::StatDisplay{
							.isTransparent = transparent,
							.stat = substat.value(),
						});
					}
				},
			},
		};

		auto equippedButton = Button{
			.widget{
				.width = Size::Expand,
			},
			.text = artifact.equippedCharacter ? Store::characters.at(artifact.equippedCharacter).loadout.character.data.name : "Unequipped",
			.style = artifact.equippedCharacter ? ButtonStyle::Accent() : ButtonStyle::Standard(),
			.onClick = [&artifact = artifact](GestureDetector::Event event) {
				event.widget.addOverlay(UI::CharacterSelector{
					.onSelect = [&artifact](Character::InstanceKey instanceKey) {
						artifact.equipOn(instanceKey);
					},
				});
			},
		};

		auto footer = Row{
			.widget{
				.height = Size::Shrink,
				.padding = 4.f,
			},
			.spacing = 4.f,
			.children{
				equippedButton,
				Button{
					.text = "Edit",
					.style = ButtonStyle::Standard(),
					.onClick = [artifact = artifact](GestureDetector::Event event) {
						event.widget.addOverlay(UI::ArtifactEditor{
							.artifact = artifact,
							.onSubmit = [key = artifact.key](Artifact::Instance artifact) {
								artifact.key = key;
								artifact.updateStats();
								Store::artifacts[key] = artifact;
								if (artifact.equippedCharacter.key != 0) {
									auto &character = Store::characters.at(artifact.equippedCharacter);
									character.loadout.artifact.refreshStats();
									character.updateEvent.notify();
								}
								artifact.updateEvent.notify();
							},
						});
					},
				},
				Button{
					.text = "Delete",
					.style = ButtonStyle::Standard(),
					.onClick = [key = artifact.key](GestureDetector::Event) {
						auto &artifact = Store::artifacts.at(key);
						if (artifact.equippedCharacter.key) {
							auto &character = Store::characters.at(artifact.equippedCharacter);
							character.loadout.artifact.equipped.fromSlot(artifact.slot) = std::nullopt;
							character.loadout.artifact.refreshStats();
						}

						Store::artifacts.erase(key);
						Store::artifactListUpdateEvent.notify();
					},
				},
			},
		};
		return Column{
			.children{
				header,
				subStats,
				footer,
			},
		};
	}
};

UI::ArtifactCard::operator squi::Child() const {
	auto storage = std::make_shared<Storage>();

	return Card{
		.widget{
			.padding = Padding{1.f},
			.onInit = [updateEvent = artifact.updateEvent, key = artifact.key](Widget &w) {
				w.customState.add(updateEvent.observe([&w, key]() {
					if (!Store::artifacts.contains(key)) {
						w.deleteLater();
						return;
					}
					w.setChildren({ArtifactCardContent{
						.artifact = Store::artifacts.at(key),
					}});
				}));
			},
		},
		.child = ArtifactCardContent{.artifact = artifact},
	};
}
