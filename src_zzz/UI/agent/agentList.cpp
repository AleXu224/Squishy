#include "agentList.hpp"

#include "agentCard.hpp"
#include "agentDataSelector.hpp"
#include "store.hpp"
#include "widgets/button.hpp"
#include "widgets/grid.hpp"
#include "widgets/navigator.hpp"
#include "widgets/row.hpp"
#include "widgets/scrollview.hpp"
#include <GLFW/glfw3.h>


using namespace squi;

squi::core::Child UI::AgentList::State::build(const Element &element) {
	Child agentAddButton = Button{
		.onClick = [this]() {
			Navigator::of(*this->element).pushOverlay(AgentDataSelector{
				.onSelect = [](Agent::DataKey dataKey) {
					++Store::lastAgentId;
					auto agent = Store::agents.emplace(Store::lastAgentId, Agent::Instance({Store::lastAgentId}, dataKey));

					Store::agentListUpdateEvent.notify();
				},
			});
		},
		.child = "Add agent",
	};

	Child buttonBar = Row{
		.widget{.height = Size::Shrink},
		.children{
			agentAddButton,
		},
	};

	return ScrollView{
		.scrollWidget{
			.padding = 8.f,
		},
		.alignment = Flex::Alignment::center,
		.spacing = 8.f,
		.children{
			buttonBar,
			Grid{
				.widget{
					.height = Size::Shrink,
					.sizeConstraints = BoxConstraints{
						.maxWidth = 1520.f,
					},
				},
				.columnCount = Grid::MinSize{.value = 256.f},
				.spacing = 2.f,
				.children = [&]() {
					Children ret;
					for (auto &[_, agent]: ::Store::agents) {
						ret.emplace_back(UI::AgentCard{
							.agentKey = agent.instanceKey,
						});
					}
					return ret;
				}(),
			}
		},
	};
}
