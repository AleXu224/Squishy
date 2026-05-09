#include "optimizationResult.hpp"

#include "UI/disc/discCard.hpp"
#include "disc/sets.hpp"
#include "store.hpp"
#include "widgets/box.hpp"
#include "widgets/button.hpp"
#include "widgets/column.hpp"
#include "widgets/expander.hpp"
#include "widgets/image.hpp"
#include "widgets/row.hpp"


using namespace squi;

[[nodiscard]] squi::core::Child UI::OptimizationResult::build(const Element &element) const {
	return Expander{
		.widget = widget,
		.icon = Row{
			.widget{
				.width = Size::Shrink,
			},
			.spacing = 4.f,
			.children = [&]() {
				Children ret;
				for (const auto &partition: Disc::partitions) {
					auto &disc = solution.discs.fromPartition(partition);
					ret.emplace_back(Box{
						.color = Color::css(0xffffff, 0.2f),
						.borderRadius = 4.f,
						.child = Image{
							.fit = squi::Image::Fit::contain,
							.image = ImageProvider::fromFile(std::format("assets/Discs/{}/icon.png", Disc::sets.at(::Store::discs.at(disc).set).name)),
						},
					});
				}
				return ret;
			}(),
		},
		.title = std::format("Result #{}", entryIndex),
		.subtitle = std::format("Score: {}", solution.score),
		.defaultExpanded = true,
		.action = Button{
			.theme = Button::Theme::Accent(element),
			.onClick = [this]() {
				auto &agent = ::Store::agents.at(agentKey);

				for (const auto &partition: Disc::partitions) {
					auto &discKey = solution.discs.fromPartition(partition);

					if (!discKey && agent.state.loadout().disc.getSlotted().fromPartition(partition)) {
						agent.state.loadout().disc.getSlotted().fromPartition(partition).clear();
					}

					auto &disc = ::Store::discs.at(discKey);
					disc.equipOn(agentKey, agent.state.loadoutIndex);
				}
				agent.state.loadout().disc.refreshStats();
				agent.updateEvent.notify();
			},
			.child = "Equip",
		},
		.content = Column{
			.widget{
				.padding = 4.f,
			},
			.children{
				Row{
					.spacing = 4.f,
					.children = [solution = solution]() {
						Children ret{};
						for (const auto &partition: Disc::partitions) {
							auto &key = solution.discs.fromPartition(partition);
							if (!key) continue;
							ret.emplace_back(DiscCard{
								.disc = ::Store::discs.at(key),
								.actions = DiscCard::Actions::showcase,
							});
						}
						return ret;
					}(),
				},
			},
		},
	};
}
