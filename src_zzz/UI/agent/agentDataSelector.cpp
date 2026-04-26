#include "agentDataSelector.hpp"

#include "misc/rarityToColor.hpp"
#include "ranges"

#include "agent/agents.hpp"

#include "observer.hpp"
#include "widgets/box.hpp"
#include "widgets/button.hpp"
#include "widgets/column.hpp"
#include "widgets/fontIcon.hpp"
#include "widgets/grid.hpp"
#include "widgets/image.hpp"
#include "widgets/liteFilter.hpp"
#include "widgets/modal.hpp"
#include "widgets/row.hpp"
#include "widgets/scrollview.hpp"
#include "widgets/stack.hpp"


using namespace squi;

struct AgentDataSelectorAgentCard : StatelessWidget {
	// Args
	Key key;
	Args widget{};
	const Agent::Data &agent;
	VoidObservable closeEvent;
	std::function<void(Agent::DataKey)> notifySelection;

	[[nodiscard]] Child build(const Element &) const {
		Child stars = Row{
			.widget{
				.width = Size::Shrink,
				.height = Size::Shrink,
			},
			.children = [&]() {
				Children ret;
				for (int i = 0; i < agent.baseStats.rarity; ++i) {
					ret.emplace_back(FontIcon{.size = 16.f, .font = FontStore::defaultIconsFilled, .color = Color::orange, .icon = 0xE838});
				}
				return ret;
			}(),
		};

		Child details = Column{
			.widget{
				.height = Size::Shrink,
				.padding = Padding(0.f, 8.f).withRight(8.f),
			},
			.spacing = 12.f,
			.children{
				Text{
					.text = std::string(agent.name),
					.fontSize = 14.f,
					.lineWrap = true,
				},
				stars,
			},
		};

		Child image = Box{
			.widget{
				.width = 96.f,
				.height = 96.f,
			},
			.color = Misc::rarityToColor.at(agent.baseStats.rarity),
			.borderRadius = BorderRadius::Left(4.f),
			.child = Image{
				.fit = squi::Image::Fit::contain,
				.image = ImageProvider::fromFile(std::format("assets/Agents/{}/avatar.png", agent.name)),
			},
		};

		Child content = Row{
			.crossAxisAlignment = Row::Alignment::center,
			.spacing = 12.f,
			.children{
				image,
				details,
			},
		};

		return Button{
			.widget{
				.width = Size::Expand,
				.height = Size::Shrink,
				.padding = 0.f,
			},
			.onClick = [this]() {
				if (notifySelection) notifySelection(agent.key);
				closeEvent.notify();
			},
			.child = content,
		};
	}
};

squi::core::Child UI::AgentDataSelector::State::build(const Element &element) {
	Child elementFilter = LiteFilter{
		.items = [&]() {
			std::vector<LiteFilter::Item> ret{};
			ret.reserve(Misc::agentAttributes.size());

			for (const auto &agentAttribute: Misc::agentAttributes) {
				ret.emplace_back(LiteFilter::Item{
					.name = Utils::Stringify(agentAttribute),
					.onUpdate = [agentAttribute, this](bool active) {
						auto &status = agentAttributes.at(agentAttribute);
						if (status != active) {
							setState([&]() {
								status = active;
							});
						}
					},
				});
			}

			return ret;
		}(),
	};
	Child engineTypeFilter = LiteFilter{
		.items = [&]() {
			std::vector<LiteFilter::Item> ret{};
			ret.reserve(Misc::specialties.size());

			for (const auto &specialty: Misc::specialties) {
				ret.emplace_back(LiteFilter::Item{
					.name = Utils::Stringify(specialty),
					.onUpdate = [specialty, this](bool active) {
						auto &status = agentSpecialties.at(specialty);
						if (status != active) {
							setState([&]() {
								status = active;
							});
						}
					},
				});
			}

			return ret;
		}(),
	};

	auto makeAgentList = [this]() {
		Children ret{};

		for (const auto &[_, agent]: Agent::list | std::views::filter([&](auto &&iter) {
										 const auto &[_, agent] = iter;
										 return agentAttributes.at(agent.baseStats.attribute) && agentSpecialties.at(agent.baseStats.specialty);
									 })) {
			ret.emplace_back(AgentDataSelectorAgentCard{.agent = agent, .closeEvent = closeEvent, .notifySelection = widget->onSelect});
		}

		return ret;
	};
	Child content = ScrollView{
		.children{
			Grid{
				.widget{
					.height = Size::Shrink,
					.margin = Margin(24.f, 0.f),
				},
				.columnCount = Grid::MinSize{200.f},
				.spacing = 8.f,
				.children = makeAgentList(),
			},
		},
	};

	Child header = Column{
		.widget{
			.height = Size::Shrink,
			.margin = Margin(24.f, 0.f),
		},
		.spacing = 16.f,
		.children{
			Stack{
				.children{
					Text{
						.widget{
							.alignment = Alignment::CenterLeft,
						},
						.text = "Select agent",
						.fontSize = 28.f,
						.font = FontStore::defaultFontBold,
					},
					// Align{
					// 	.xAlign = 1.f,
					// 	// FIXME: implement searching
					// 	// .child = TextBox{},
					// },
				},
			},
			Column{
				.widget{.height = Size::Shrink},
				.spacing = 8.f,
				.children{
					elementFilter,
					engineTypeFilter,
				},
			},
		},
	};

	return Modal{
		.widget = widget->widget,
		.closeEvent = closeEvent,
		.child = Box{
			.widget{
				.height = Size::Shrink,
				.alignment = Alignment::Center,
				.sizeConstraints = BoxConstraints{.maxWidth = 800.f},
			},
			.color = Color::css(0x2C2C2C),
			.borderColor = Color::css(117, 117, 117, 0.4f),
			.borderWidth = 1.f,
			.borderRadius = 8.f,
			.borderPosition = squi::Box::BorderPosition::outset,
			.child = Column{
				.widget{
					.padding = Padding(0.f, 24.f),
				},
				.spacing = 24.f,
				.children{
					header,
					content
				},
			},
		},
	};
}
