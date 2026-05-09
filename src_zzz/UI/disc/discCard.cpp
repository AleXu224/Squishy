#include "discCard.hpp"

#include "UI/agent/agentSelector.hpp"
#include "UI/disc/discEditor.hpp"
#include "UI/utils/card.hpp"
#include "UI/utils/statDisplay.hpp"
#include "UI/utils/tag.hpp"
#include "agent/data.hpp"
#include "disc/sets.hpp"
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


struct DiscHeader : StatelessWidget {
	// Args
	Key key;
	Disc::SetKey set;
	Disc::Partition partition;
	StatValue mainStat;
	uint8_t rarity;
	uint8_t level;
	Agent::InstanceKey equippedAgent;

	[[nodiscard]] Child build(const Element &) const {
		Child icon = Image{
			.widget{
				.alignment = Alignment::CenterRight,
			},
			.fit = squi::Image::Fit::contain,
			.image = ImageProvider::fromFile(std::format("assets/Discs/{}/icon.png", Disc::sets.at(set).name)),
		};
		Child title = Column{
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
		Child agentImage = [&]() -> Child {
			if (!equippedAgent) return Child{};
			return Image{
				.widget{
					.width = 32.f,
					.height = 32.f,
					.alignment = Alignment::BottomRight,
					.margin = Margin{}.withRight(64.f),
				},
				.fit = squi::Image::Fit::contain,
				.image = ImageProvider::fromFile(std::format("assets/Agents/{}/avatar.png", ::Store::agents.at(equippedAgent).state.stats.data.name))
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
					agentImage,
					title,
				},
			},
		};
	}
};

struct DiscCardContent : StatelessWidget {
	// Args
	Key key;
	Args widget{};
	Disc::Instance &disc;
	UI::DiscCard::Actions actions;

	[[nodiscard]] Child build(const Element &element) const {
		Child header = DiscHeader{
			.set = disc.set,
			.partition = disc.partition,
			.mainStat{
				.stat = disc.mainStat,
				.value = Stats::Values::mainStat.at(disc.mainStat).at(disc.rarity, disc.level),
			},
			.rarity = disc.rarity,
			.level = disc.level,
			.equippedAgent = disc.equippedOn(),
		};
		Child subStats = Column{
			.widget{
				.padding = Padding{4.f},
			},
			.children = [&]() {
				Children ret{};
				auto trueFalse = {false, true};
				auto trueFalseInf = std::views::join(std::views::repeat(trueFalse));

				for (const auto &[transparent, substat]: std::views::zip(trueFalseInf, disc.subStats)) {
					if (!substat.stat.has_value()) continue;
					ret.emplace_back(UI::StatDisplay{
						.isTransparent = transparent,
						.showRolls = true,
						.rarity = disc.rarity,
						.stat = substat,
					});
				}
				return ret;
			}(),
		};

		auto equippedAgent = disc.equippedOn();
		Child equippedButton = Button{
			.widget{
				.width = Size::Expand,
			},
			.theme = equippedAgent ? Button::Theme::Accent(element) : Button::Theme::Standard(),
			.onClick = [&]() {
				Navigator::of(element).pushOverlay(UI::AgentSelector{
					.onSelect = [this](Agent::InstanceKey instanceKey) {
						disc.equipOn(instanceKey);
					},
				});
			},
			.child = equippedAgent ? std::string(Store::agents.at(equippedAgent).state.stats.data.name) : "Unequipped",
		};

		Child footer = Row{
			.widget{
				.height = Size::Shrink,
				.padding = 4.f,
			},
			.spacing = 4.f,
			.children{
				actions == UI::DiscCard::Actions::list ? equippedButton : Child{},
				Button{
					.theme = Button::Theme::Standard(),
					.onClick = [&]() {
						Navigator::of(element).pushOverlay(UI::DiscEditor{
							.disc = disc,
							.onSubmit = [key = disc.key](Disc::Instance disc) {
								disc.key = key;
								disc.updateStats();
								Store::discs[key] = disc;
								disc.refreshUsages();
								disc.updateEvent.notify();
							},
						});
					},
					.child = "Edit",
				},
				actions == UI::DiscCard::Actions::list//
					? Button{
						  .theme = Button::Theme::Standard(),
						  .onClick = [key = disc.key]() {
							  auto &disc = Store::discs.at(key);
							  disc.unequip();

							  Store::discs.erase(key);
							  Store::discListUpdateEvent.notify();
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
				actions != UI::DiscCard::Actions::showcase ? footer : Child{},
			},
		};
	}
};

squi::core::Child UI::DiscCard::State::build(const Element &) {
	return Card{
		.widget{
			.padding = Padding{1.f},
		},
		.borderColor = Misc::rarityToColor.at(widget->disc.rarity),
		.child = DiscCardContent{
			.disc = widget->disc,
			.actions = widget->actions,
		},
	};
}
