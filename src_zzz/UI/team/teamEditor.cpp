#include "teamEditor.hpp"

#include "UI/agent/agentSelector.hpp"
#include "UI/utils/editorItem.hpp"
#include "agent/data.hpp"
#include "agent/instance.hpp"
#include "agent/key.hpp"
#include "store.hpp"
#include "widgets/column.hpp"
#include "widgets/dialog.hpp"
#include "widgets/fontIcon.hpp"
#include "widgets/navigator.hpp"
#include "widgets/row.hpp"
#include "widgets/textBox.hpp"
#include "widgets/tooltip.hpp"
#include "widgets/visibility.hpp"


using namespace squi;

squi::core::Child UI::TeamEditor::State::build(const Element &element) {
	Child name = EditorItem{
		.name = "Name",
		.child = TextBox{
			.controller = nameController,
		},
	};

	Children content{
		name
	};

	for (size_t i = 0; i < team.stats.agents.size(); i++) {
		const auto &agent = team.stats.agents.at(i);

		Child changeAgentButton = Button{
			.theme = agent ? Button::Theme::Accent(element) : Button::Theme::Standard(),
			.onClick = [this, i]() {
				Navigator::of(this).pushOverlay(AgentSelector{
					.onSelect = [this, i](Agent::InstanceKey instanceKey) {
						auto agent = Store::agents.find(instanceKey);
						if (agent == Store::agents.end()) return;
						setState([&]() {
							team.stats.agents.at(i) = &agent->second;
						});
					},
				});
			},
			.child = agent ? std::string(agent->state.stats.data.name) : "None",
		};

		Child deleteButton = Tooltip{
			.text = "Remove agent",
			.child = Visibility{
				.visible = agent != nullptr,
				.child = Button{
					.onClick = [this, i]() {
						setState([&]() {
							team.stats.agents.at(i) = nullptr;
						});
					},
					.child = FontIcon{
						.icon = 0xe5cd,
					},
				},
			},
		};

		content.emplace_back(
			EditorItem{
				.name = fmt::format("Agent {}", i + 1),
				.child = Row{
					.widget{.width = Size::Shrink, .height = Size::Shrink},
					.spacing = 4.f,
					.children{
						changeAgentButton,
						deleteButton,
					},
				},
			}
		);
	}

	auto footer = Children{
		Button{
			.widget{.width = Size::Expand},
			.theme = Button::Theme::Accent(element),
			.onClick = [this]() {
				if (widget->onSubmit) widget->onSubmit(team);
				closeEvent.notify();
			},
			.child = "Save",
		},
		Button{
			.widget{.width = Size::Expand},
			.theme = Button::Theme::Standard(),
			.onClick = [this]() {
				closeEvent.notify();
			},
			.child = "Cancel",
		},
	};

	return Dialog{
		.closeEvent = closeEvent,
		.title = "Edit team",
		.content = Column{
			.spacing = 16.f,
			.children = content,
		},
		.buttons = footer,
	};
}
