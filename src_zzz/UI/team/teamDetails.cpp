#include "teamDetails.hpp"

#include "UI/agent/agentDetails.hpp"

#include "agent/data.hpp"
#include "store.hpp"
#include "teamBuffs.hpp"
#include "widgets/image.hpp"
#include "widgets/navigator.hpp"
#include "widgets/sideNav.hpp"


using namespace squi;
squi::core::Child UI::TeamDetails::State::build(const Element &element) {
	return SideNav{
		.backAction = !widget->enableBackButton//
						? std::function<void()>{}
						: [this]() {
							  Navigator::of(this).pop();
						  },
		.defaultExpanded = false,
		.pages = [&]() {
			std::vector<SideNav::Page> pages{};
			pages.emplace_back(SideNav::Page{
				.name = "Team Buffs",
				.icon = 0xe7ef,
				.content = TeamBuffs{
					.instanceKey = widget->teamKey,
				},
			});
			auto &team = ::Store::teams.at(widget->teamKey);
			for (auto &agent: team.stats.agents) {
				if (agent) {
					pages.emplace_back(SideNav::Page{
						.name = std::string(agent->state.stats.data.name),
						.icon = Image{
							.widget{
								.width = 16.f,
								.height = 16.f,
							},
							.fit = squi::Image::Fit::contain,
							.image = ImageProvider::fromFile(std::format("assets/Agents/{}/avatar.png", agent->state.stats.data.name)),
						},
						.content = AgentDetails{
							.key = IndexKey{static_cast<int64_t>(agent->instanceKey.key)},
							.agentKey = agent->instanceKey,
							.teamKey = widget->teamKey,
						},
					});
				}
			}
			return pages;
		}(),
	};
}
