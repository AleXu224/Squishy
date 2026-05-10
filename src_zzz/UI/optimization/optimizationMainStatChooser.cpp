#include "optimizationMainStatChooser.hpp"
#include "UI/utils/displayCard.hpp"
#include "UI/utils/editorItem.hpp"
#include "widgets/row.hpp"
#include "widgets/toggleSwitch.hpp"

namespace UI {
	squi::core::Child OptimizationMainStatChooser::State::generateFor(std::string name, std::map<Stat, bool> &mainStats) {
		Children options{};
		for (const auto &[stat, enabled]: mainStats) {
			options.emplace_back(EditorItem{
				.name = Utils::Stringify(stat),
				.child = ToggleSwitch{
					.active = enabled,
					.statePosition = ToggleSwitch::StatePosition::None,
					.onToggle = [&mainStats, stat, this](bool active) {
						setState([&]() {
							mainStats[stat] = active;
						});
					},
				},
			});
		}

		return DisplayCard{
			.title = std::move(name),
			.children{
				Column{
					.widget{
						.padding = 16.f,
					},
					.spacing = 8.f,
					.children = std::move(options),
				},
			},
			.footer{
				Row{
					.widget{
						.height = Size::Wrap,
					},
					.justifyContent = Row::JustifyContent::end,
					.spacing = 8.f,
					.children{
						Button{
							.onClick = [&mainStats, this]() {
								setState([&]() {
									for (auto &[_, enabled]: mainStats) {
										enabled = true;
									}
								});
							},
							.child = "Select all",
						},
						Button{
							.onClick = [&mainStats, this]() {
								setState([&]() {
									for (auto &[_, enabled]: mainStats) {
										enabled = false;
									}
								});
							},
							.child = "Deselect all",
						},
					},
				},
			}
		};
	}
	squi::core::Child OptimizationMainStatChooser::State::build(const Element &element) {
		return Row{
			.widget{
				.padding = 8.f,
			},
			.justifyContent = Flex::JustifyContent::center,
			.spacing = 8.f,
			.children{
				generateFor("Partition 4", widget->partition4MainStats),
				generateFor("Partition 5", widget->partition5MainStats),
				generateFor("Partition 6", widget->partition6MainStats),
			},
		};
	}
}// namespace UI