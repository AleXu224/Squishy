#include "artifactCard.hpp"

#include "Ui/artifact/artifactEditor.hpp"
#include "Ui/character/characterSelector.hpp"
#include "Ui/utils/card.hpp"
#include "Ui/utils/statDisplay.hpp"
#include "Ui/utils/tag.hpp"
#include "align.hpp"
#include "artifact/sets.hpp"
#include "box.hpp"
#include "button.hpp"
#include "character/data.hpp"
#include "column.hpp"
#include "gestureDetector.hpp"
#include "image.hpp"
#include "misc/rarityToColor.hpp"
#include "rebuilder.hpp"
#include "row.hpp"
#include "stack.hpp"
#include "store.hpp"
#include "text.hpp"


#include "ranges"
#include "theme.hpp"

using namespace squi;

struct ArtifactHeader {
	// Args
	Artifact::SetKey set;
	Artifact::Slot slot;
	StatValue mainStat;
	uint8_t rarity;
	uint8_t level;
	Character::InstanceKey equippedCharacter;

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
					Row{
						.alignment = Row::Alignment::center,
						.spacing = 4.f,
						.children{
							UI::Tag{.sourceStr = std::format("+{}", level)},
							Text{
								.text = Utils::Stringify(mainStat.stat),
							},
						},
					},
					Text{
						.text = Utils::Stringify(mainStat),
						.fontSize = 24.f,
					},
				},
			},
		};
		auto characterImage = [&]() -> Child {
			if (!equippedCharacter) return Child{};
			return Align{
				.xAlign = 1.f,
				.yAlign = 1.f,
				.child = Image{
					.widget{
						.width = 32.f,
						.height = 32.f,
						.margin = Margin{}.withRight(64.f),
					},
					.fit = squi::Image::Fit::contain,
					.image = ImageProvider::fromFile(std::format("assets/Characters/{}/avatar.png", ::Store::characters.at(equippedCharacter).state.stats.data.name))
				},
			};
		}();
		return Box{
			.widget{
				.height = 64.f,
			},
			// .color = Misc::rarityToColor.at(rarity),
			.color = Color::css(0xffffff, 0.0419f),
			.borderRadius{7.f, 7.f, 0.f, 0.f},
			.child = Stack{
				.children{
					icon,
					characterImage,
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
	UI::ArtifactCard::Actions actions;

	operator squi::Child() const {
		auto header = ArtifactHeader{
			.set = artifact.set,
			.slot = artifact.slot,
			.mainStat{
				.stat = artifact.mainStat,
				.value = Stats::Values::mainStat.at(artifact.mainStat).at(artifact.rarity - 1).at(artifact.level),
			},
			.rarity = artifact.rarity,
			.level = artifact.level,
			.equippedCharacter = artifact.equippedOn(),
		};
		auto subStats = Column{
			.widget{
				.padding = Padding{4.f},
				.onInit = [artifact = artifact](Widget &w) {
					auto trueFalse = {false, true};
					auto trueFalseInf = std::views::join(std::views::repeat(trueFalse));

					for (const auto &[transparent, substat]: std::views::zip(trueFalseInf, artifact.subStats)) {
						if (!substat.stat.has_value()) continue;
						w.addChild(UI::StatDisplay{
							.isTransparent = transparent,
							.stat = substat,
						});
					}
				},
			},
		};

		auto equippedCharacter = artifact.equippedOn();
		auto equippedButton = Button{
			.widget{
				.width = Size::Expand,
			},
			.text = equippedCharacter ? Store::characters.at(equippedCharacter).state.stats.data.name : "Unequipped",
			.style = equippedCharacter ? ButtonStyle::Accent() : ButtonStyle::Standard(),
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
				actions == UI::ArtifactCard::Actions::list ? equippedButton : Child{},
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
								artifact.refreshUsages();
								artifact.updateEvent.notify();
							},
						});
					},
				},
				actions == UI::ArtifactCard::Actions::list//
					? Button{
						  .text = "Delete",
						  .style = ButtonStyle::Standard(),
						  .onClick = [key = artifact.key](GestureDetector::Event) {
							  auto &artifact = Store::artifacts.at(key);
							  artifact.unequip();

							  Store::artifacts.erase(key);
							  Store::artifactListUpdateEvent.notify();
						  },
					  }
					: Child{},
			},
		};
		return Column{
			.children{
				header,
				subStats,
				actions != UI::ArtifactCard::Actions::showcase ? footer : Child{},
			},
		};
	}
};

UI::ArtifactCard::operator squi::Child() const {
	auto storage = std::make_shared<Storage>();

	return Rebuilder{
		.widget = widget,
		.rebuildEvent = artifact.updateEvent,
		.buildFunc = [key = artifact.key, actions = actions]() -> Child {
			if (!Store::artifacts.contains(key)) {
				return Child{};
			}
			auto &artifact = Store::artifacts.at(key);
			auto _ = ThemeManager::pushTheme(Theme{.accent = Misc::rarityToColor.at(artifact.rarity)});
			return Card{
				.widget{
					.padding = Padding{1.f},
				},
				.borderColor = Misc::rarityToColor.at(artifact.rarity),
				.child = ArtifactCardContent{
					.artifact = artifact,
					.actions = actions,
				},
			};
		},
	};
}
