#include "agentCardBanner.hpp"

#include "agent/agents.hpp"
#include "store.hpp"
#include "widgets/column.hpp"
#include "widgets/container.hpp"
#include "widgets/image.hpp"
#include "widgets/row.hpp"
#include "widgets/stack.hpp"
#include "widgets/text.hpp"

using namespace squi;

[[nodiscard]] squi::core::Child UI::AgentCardBanner::build(const Element &) const {
	auto &agent = ::Store::agents.at(agentKey);
	return Stack{
		.widget{
			.height = 64.f,
		},
		.children{
			Container{
				.child = Image{
					.widget{
						.width = Size::Expand,
						.height = Size::Expand,
					},
					.fit = squi::Image::Fit::cover,
					.image = ImageProvider::fromFile(std::format("assets/Agents/{}/banner.png", Agent::list.at(agent.dataKey).name)),
				},
			},
			Row{
				.children{
					Image{
						.fit = squi::Image::Fit::contain,
						.image = ImageProvider::fromFile(std::format("assets/Agents/{}/avatar.png", Agent::list.at(agent.dataKey).name)),
					},
					Column{
						.widget{
							.padding = Padding{4.f},
						},
						.spacing = 4.f,
						.children{
							Text{
								.text = std::string(Agent::list.at(agent.dataKey).name),
								.fontSize = 24.f,
							},
							Text{
								.text = std::format("Lvl {} M{}", agent.state.stats.sheet.level, agent.state.stats.sheet.mindscape),
							},
						},
					},
				},
			},
		},
	};
}
