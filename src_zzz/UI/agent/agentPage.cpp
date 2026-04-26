#include "agentPage.hpp"

#include "UI/agent/agentDetails.hpp"
#include "widgets/navigator.hpp"
#include "widgets/sideNav.hpp"


using namespace squi;

squi::core::Child UI::AgentPage::State::build(const Element &element) {
	return SideNav{
		.backAction = [this]() {
			Navigator::of(this).popPage();
		},
		.defaultExpanded = false,
		.pages{
			SideNav::Page{
				.name = "Details",
				.content = AgentDetails{
					.agentKey = widget->agentKey,
				},
			},
		},
	};
}
