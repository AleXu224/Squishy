#include "agentLoadouts.hpp"

#include "UI/loadout/loadoutCard.hpp"
#include "observer.hpp"
#include "ranges"
#include "store.hpp"
#include "widgets/column.hpp"
#include "widgets/dialog.hpp"
#include "widgets/dropdownButton.hpp"
#include "widgets/expander.hpp"


using namespace squi;

namespace {
	struct AgentLoadoutsHeadingEntry : StatelessWidget {
		// Args
		Key key;
		Args widget{};
		Agent::InstanceKey agentKey;

		[[nodiscard]] Child build(const Element &) const {
			return Expander{
				.title = "Add loadout",
				.alwaysExpanded = true,
				.action{
					DropdownButton{
						.text = "Add",
						.items{
							ContextMenu::Button{
								.text = "Build",
								.callback = [agentKey = agentKey]() {
									auto &agent = ::Store::agents.at(agentKey);
									agent.state.loadouts.emplace_back(Stats::Loadout{
										.disc{},
									});
									agent.updateEvent.notify();
								},
							},
							ContextMenu::Button{
								.text = "TC Build",
								.callback = [agentKey = agentKey]() {
									auto &agent = ::Store::agents.at(agentKey);
									agent.state.loadouts.emplace_back(Stats::Loadout{
										.disc{
											.equipped = Stats::Disc::Theorycraft{},
										},
									});
									agent.updateEvent.notify();
								},
							},
						},
					},
				},
			};
		}
	};
}// namespace

squi::core::Child UI::AgentLoadouts::State::build(const Element &element) {
	auto &agent = Store::agents.at(widget->agentKey);

	Child content = Column{
		.spacing = 8.f,
		.children = [&]() {
			Children ret{
				AgentLoadoutsHeadingEntry{
					.agentKey = widget->agentKey,
				},
				LoadoutCard{
					.agentKey = widget->agentKey,
				},
			};

			for (const auto &[index, loadout]: std::views::enumerate(agent.state.loadouts)) {
				ret.emplace_back(LoadoutCard{.agentKey = widget->agentKey, .loadoutIndex = index});
			}

			return ret;
		}(),
	};

	Children buttonFooter{
		Button{
			.widget{
				.alignment = Alignment::CenterRight,
			},
			.theme = Button::Theme::Standard(),
			.onClick = [this]() {
				closeEvent.notify();
			},
			.child = "Close",
		},
	};


	return Dialog{
		.width = 1280.f,
		.closeEvent = closeEvent,
		.title = "Loadouts",
		.content = content,
		.buttons = buttonFooter,
	};
}
