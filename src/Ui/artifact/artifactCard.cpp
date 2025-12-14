#include "artifactCard.hpp"

#include "Ui/artifact/artifactEditor.hpp"
#include "Ui/character/characterSelector.hpp"
#include "Ui/utils/card.hpp"
#include "Ui/utils/statDisplay.hpp"
#include "Ui/utils/tag.hpp"
#include "artifact/sets.hpp"
#include "character/data.hpp"
#include "misc/rarityToColor.hpp"
#include "store.hpp"
#include "widgets/box.hpp"
#include "widgets/button.hpp"
#include "widgets/column.hpp"
#include "widgets/image.hpp"
#include "widgets/navigator.hpp"


#include "ranges"
#include "widgets/row.hpp"
#include "widgets/stack.hpp"
#include "widgets/text.hpp"

using namespace squi;


struct ArtifactHeader : StatelessWidget {
	// Args
	Key key;
	Artifact::SetKey set;
	Artifact::Slot slot;
	StatValue mainStat;
	uint8_t rarity;
	uint8_t level;
	Character::InstanceKey equippedCharacter;

	[[nodiscard]] Child build(const Element &) const {
		auto icon = Image{
			.fit = squi::Image::Fit::contain,
			.image = ImageProvider::fromFile(std::format("assets/Artifacts/{}/{}.png", Artifact::sets.at(set).name, Utils::Stringify(slot))),
		};
		auto title = Column{
			.widget{
				.height = Size::Shrink,
				.alignment = Alignment::CenterLeft,
				.padding = Padding{12.f, 0.f},
			},
			.children{
				Row{
					.crossAxisAlignment = Row::Alignment::center,
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
		};
		auto characterImage = [&]() -> Child {
			if (!equippedCharacter) return Child{};
			return Image{
				.widget{
					.width = 32.f,
					.height = 32.f,
					.alignment = Alignment::BottomRight,
					.margin = Margin{}.withRight(64.f),
				},
				.fit = squi::Image::Fit::contain,
				.image = ImageProvider::fromFile(std::format("assets/Characters/{}/avatar.png", ::Store::characters.at(equippedCharacter).state.stats.data.name))
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

struct ArtifactCardContent : StatelessWidget {
	// Args
	Key key;
	Args widget{};
	Artifact::Instance &artifact;
	UI::ArtifactCard::Actions actions;

	[[nodiscard]] Child build(const Element &element) const {
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
			},
			.children = [&]() {
				Children ret{};
				auto trueFalse = {false, true};
				auto trueFalseInf = std::views::join(std::views::repeat(trueFalse));

				for (const auto &[transparent, substat]: std::views::zip(trueFalseInf, artifact.subStats)) {
					if (!substat.stat.has_value()) continue;
					ret.emplace_back(UI::StatDisplay{
						.isTransparent = transparent,
						.stat = substat,
					});
				}
				return ret;
			}(),
		};

		auto equippedCharacter = artifact.equippedOn();
		auto equippedButton = Button{
			.widget{
				.width = Size::Expand,
			},
			.theme = equippedCharacter ? Button::Theme::Accent() : Button::Theme::Standard(),
			.onClick = [&]() {
				Navigator::of(element).pushOverlay(UI::CharacterSelector{
					.onSelect = [this](Character::InstanceKey instanceKey) {
						artifact.equipOn(instanceKey);
					},
				});
			},
			.child = equippedCharacter ? std::string(Store::characters.at(equippedCharacter).state.stats.data.name) : "Unequipped",
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
					.theme = Button::Theme::Standard(),
					.onClick = [&]() {
						Navigator::of(element).pushOverlay(UI::ArtifactEditor{
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
					.child = "Edit",
				},
				actions == UI::ArtifactCard::Actions::list//
					? Button{
						  .theme = Button::Theme::Standard(),
						  .onClick = [key = artifact.key]() {
							  auto &artifact = Store::artifacts.at(key);
							  artifact.unequip();

							  Store::artifacts.erase(key);
							  Store::artifactListUpdateEvent.notify();
						  },
						  .child = "Delete",
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

squi::core::Child UI::ArtifactCard::State::build(const Element &) {
	return Card{
		.widget{
			.padding = Padding{1.f},
		},
		.borderColor = Misc::rarityToColor.at(widget->artifact.rarity),
		.child = ArtifactCardContent{
			.artifact = widget->artifact,
			.actions = widget->actions,
		},
	};
}
