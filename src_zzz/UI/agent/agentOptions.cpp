#include "agentOptions.hpp"

#include "UI/utils/displayCard.hpp"
#include "UI/utils/selector.hpp"


using namespace squi;

squi::core::Child UI::AgentOptions::State::build(const Element &element) {
	return UI::DisplayCard{
		.title = "Options",
		.children{
			Column{
				.widget{
					.padding = Padding{4.f},
				},
				.spacing = 4.f,
				.children{
					AgentLevelSelector{.agentKey = widget->agentKey},
					MindscapeSelector{.agentKey = widget->agentKey},
					EngineLevelSelector{.agentKey = widget->agentKey},
					OverclockSelector{.agentKey = widget->agentKey},
					BasicSkillLevelSelector{.agentKey = widget->agentKey},
					DodgeSkillLevelSelector{.agentKey = widget->agentKey},
					AssistSkillLevelSelector{.agentKey = widget->agentKey},
					SpecialSkillLevelSelector{.agentKey = widget->agentKey},
					ChainSkillLevelSelector{.agentKey = widget->agentKey},
					CoreSkillLevelSelector{.agentKey = widget->agentKey},
				},
			},
		},
	};
}
