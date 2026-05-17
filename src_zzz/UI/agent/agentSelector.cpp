#include "agentSelector.hpp"

#include "agent/agents.hpp"
#include "agent/instance.hpp"
#include "misc/attribute.hpp"
#include "misc/promotion.hpp"
#include "store.hpp"
#include "widgets/box.hpp"
#include "widgets/button.hpp"
#include "widgets/column.hpp"
#include "widgets/fontIcon.hpp"
#include "widgets/grid.hpp"
#include "widgets/image.hpp"
#include "widgets/liteFilter.hpp"


#include "ranges"
#include "widgets/modal.hpp"
#include "widgets/row.hpp"
#include "widgets/scrollview.hpp"
#include "widgets/stack.hpp"

using namespace squi;

struct AgentSelectorAgentCard : StatelessWidget {
	// Args
	Key key;
	const Agent::Instance &agent;
	VoidObservable closeEvent;
	std::function<void(Agent::InstanceKey)> notifySelection;

	[[nodiscard]] Child build(const Element &) const {
		Child stars = Row{
			.widget{
				.width = Size::Shrink,
				.height = Size::Shrink,
			},
			.children = [&]() {
				Children ret{};
				for (uint8_t i = 0; i < Agent::list.at(agent.dataKey).baseStats.rarity; i++) {
					ret.emplace_back(FontIcon{
						.size = 16.f,
						.font = FontStore::defaultIconsFilled,
						.color = Color::orange,
						.icon = 0xE838,
					});
				}
				return ret;
			}(),
		};

		const auto &agentData = Agent::list.at(agent.dataKey);

		Child details = Column{
			.widget{
				.height = Size::Shrink,
				.padding = Padding(0.f, 8.f).withRight(8.f),
			},
			.spacing = 12.f,
			.children{
				Text{
					.text = std::string(agentData.name),
					.fontSize = 14.f,
					.lineWrap = true,
				},
				Text{
					.text = std::format("Lvl {}/{}", agent.state.stats.sheet.level, Misc::promotions.at(agent.state.stats.sheet.promotion).maxLevel),
				},
				stars,
			},
		};

		Child image = Box{
			.widget{
				.width = 96.f,
				.height = 96.f,
			},
			.color = Color::css(255, 255, 255, 0.1f),
			.borderRadius = BorderRadius::Left(4.f),
			.child = Image{
				.fit = squi::Image::Fit::contain,
				.image = ImageProvider::fromFile(std::format("assets/Agents/{}/avatar.png", agentData.name)),
			},
		};

		Child content = Row{
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
			.theme = Button::Theme::Standard(),
			.onClick = [this]() {
				if (notifySelection) notifySelection(agent.instanceKey);
				closeEvent.notify();
			},
			.child = content,
		};
	}
};

void UI::AgentSelector::State::initState() {
	for (const auto &attribute: Misc::agentAttributes) {
		agentAttributes[attribute] = true;
	}
	for (const auto &specialty: Misc::specialties) {
		agentSpecialty[specialty] = true;
	}
}

squi::core::Child UI::AgentSelector::State::build(const Element &) {
	VoidObservable closeEvent{};

	Child attributeFilter = LiteFilter{
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
	Child specialtyFilter = LiteFilter{
		.items = [&]() {
			std::vector<LiteFilter::Item> ret{};
			ret.reserve(Misc::specialties.size());

			for (const auto &specialty: Misc::specialties) {
				ret.emplace_back(LiteFilter::Item{
					.name = Utils::Stringify(specialty),
					.onUpdate = [specialty, this](bool active) {
						auto &status = agentSpecialty.at(specialty);
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

	auto makeAgentList = [closeEvent = closeEvent, this]() {
		Children ret{};

		for (const auto &[_, agent]: Store::agents | std::views::filter([&](auto &&iter) {
										 const auto &[_, agent] = iter;
										 const auto &agentData = Agent::list.at(agent.dataKey);
										 return this->agentAttributes.at(agentData.baseStats.attribute) && this->agentSpecialty.at(agentData.baseStats.specialty);
									 })) {
			ret.emplace_back(AgentSelectorAgentCard{.agent = agent, .closeEvent = closeEvent, .notifySelection = widget->onSelect});
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
						.widget{.alignment = Alignment::CenterLeft},
						.text = "Select agent",
						.fontSize = 28.f,
						.font = FontStore::defaultFontBold,
					},
					// FIXME: implement searching
					// TextBox{},
				},
			},
			Column{
				.widget{.height = Size::Shrink},
				.spacing = 8.f,
				.children{
					attributeFilter,
					specialtyFilter,
				},
			},
		},
	};

	return Modal{
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
