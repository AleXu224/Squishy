#include "discEditor.hpp"

#include "UI/utils/editorItem.hpp"
#include "disc/sets.hpp"
#include "stats/disc.hpp"

#include "widgets/column.hpp"
#include "widgets/container.hpp"
#include "widgets/dialog.hpp"
#include "widgets/dropdownButton.hpp"
#include "widgets/numberBox.hpp"


#include "ranges"
#include "widgets/row.hpp"
#include "widgets/stack.hpp"
#include "widgets/text.hpp"
#include "widgets/toggleSwitch.hpp"

using namespace squi;

struct DiscEditorSubstat : StatelessWidget {
	// Args
	Key key;
	Args widget;
	std::string name;
	Children children;

	[[nodiscard]] Child build(const Element &) const {
		auto newWidget = widget;
		newWidget.height = newWidget.height.value_or(Size::Wrap);

		return Stack{
			.widget = newWidget,
			.children{
				Container{
					.widget{
						.width = Size::Wrap,
						.height = 32.f,
						.alignment = Alignment::CenterLeft,
					},
					.child = Text{
						.widget{.alignment = Alignment::CenterLeft},
						.text = name,
					},
				},
				Row{
					.widget{
						.width = Size::Wrap,
						.height = Size::Shrink,
						.alignment = Alignment::CenterRight,
					},
					.crossAxisAlignment = Row::Alignment::center,
					.spacing = 4.f,
					.children = children,
				},
			},
		};
	}
};

void UI::DiscEditor::State::updatePartition(Disc::Partition newPartition) {
	disc.partition = newPartition;
	const auto &stats = Stats::Disc::byPartition(newPartition);
	if (auto it = std::ranges::find(stats, disc.mainStat); it == stats.end()) {
		disc.mainStat = stats.at(0);
	}
}

squi::core::Child UI::DiscEditor::State::createSubStat(size_t subStatIndex) {
	auto &subStat = disc.subStats.at(subStatIndex);

	Child ret = DiscEditorSubstat{
		.name = std::format("Substat {}", subStatIndex + 1),
		.children{
			ToggleSwitch{
				.active = subStat.activated,
				.statePosition = ToggleSwitch::StatePosition::Left,
				.onToggle = [this, &subStat](bool active) {
					setState([&]() {
						subStat.activated = active;
					});
				},
			},
			DropdownButton{
				.theme = Button::Theme::Standard(),
				.text = subStat.stat.transform([](auto &&val) {
										return Utils::Stringify(val);
									})
							.value_or("None"),
				.items = [this, &subStat]() {
					std::vector<ContextMenu::Item> ret{};
					ret.emplace_back(ContextMenu::Button{
						.text = "None",
						.callback = [this, &subStat]() {
							setState([&]() {
								subStat.stat = std::nullopt;
							});
						},
					});
					auto filter = [this](auto &&val) {
						return disc.mainStat != val;
					};
					for (const auto &stat: std::views::filter(Stats::Disc::subStats, filter)) {
						ret.emplace_back(ContextMenu::Button{
							.text = Utils::Stringify(stat),
							.callback = [this, &subStat, stat]() {
								setState([&]() {
									subStat.stat = stat;
								});
							},
						});
					}
					return ret;
				}(),
			},
			NumberBox{
				.disabled = !subStat.stat.has_value(),
				.value = subStat.value * (Utils::isPercentage(subStat.stat) ? 100.f : 1.f),
				.min = 0.f,
				.precision = Utils::isPercentage(subStat.stat) ? 1 : 0,
				.onChange = [&](float val) {
					if (subStat.stat.has_value()) {
						subStat.value = val / (Utils::isPercentage(subStat.stat) ? 100.f : 1.f);
					}
				},
			},
		},
	};
	return ret;
}

void UI::DiscEditor::State::initState() {
	if (widget->disc.has_value()) {
		disc = widget->disc.value();
	} else {
		disc = Disc::Instance{};
	}
}

squi::core::Child UI::DiscEditor::State::build(const Element &) {
	// Level
	Child levelSelector = EditorItem{
		.name = "Level",
		.child = NumberBox{
			.value = static_cast<double>(disc.level),
			.min = 0.f,
			.max = 15.f,
			.precision = 0,
			.onChange = [this](double newVal) {
				setState([&]() {
					disc.level = std::floor(newVal);
				});
			},
		},
	};

	// Set
	Child setButton = DropdownButton{
		.theme = Button::Theme::Standard(),
		.text = disc.set.key != 0 ? std::string(Disc::sets.at(disc.set).name) : "No set",
		.items = [this]() {
			std::vector<ContextMenu::Item> ret{};
			ret.reserve(Disc::sets.size());
			for (const auto &set: Disc::sets) {
				ret.emplace_back(ContextMenu::Button{
					.text = std::string(set.second.name),
					.callback = [&set = set.second, this]() {
						setState([&]() {
							disc.set.key = set.key.key;
						});
					},
				});
			}
			return ret;
		}(),
	};
	Child setSelector = UI::EditorItem{
		.name = "Set",
		.child = setButton,
	};

	// Slot
	Child slotSelector = UI::EditorItem{
		.name = "Partition",
		.child = DropdownButton{
			.theme = Button::Theme::Standard(),
			.disabled = !disc.usedOn().empty(),
			.text = Utils::Stringify(disc.partition),
			.items = [&]() {
				std::vector<ContextMenu::Item> ret{};
				ret.reserve(Disc::partitions.size());
				for (const auto &partition: Disc::partitions) {
					ret.emplace_back(ContextMenu::Button{
						.text = Utils::Stringify(partition),
						.callback = [partition, this]() {
							setState([&]() {
								updatePartition(partition);
							});
						},
					});
				}
				return ret;
			}(),
		},
	};

	// Main stat
	Child mainStatSelector = UI::EditorItem{
		.name = "Main Stat",
		.child = DropdownButton{
			.theme = Button::Theme::Standard(),
			.text = Utils::Stringify(disc.mainStat),
			.items = [this]() {
				std::vector<ContextMenu::Item> ret{};
				for (const auto &stat: Stats::Disc::byPartition(disc.partition)) {
					ret.emplace_back(ContextMenu::Button{
						.text = Utils::Stringify(stat),
						.callback = [this, stat]() {
							setState([&]() {
								disc.mainStat = stat;
							});
						},
					});
				}
				return ret;
			}(),
		},
	};

	// Substats
	auto subStat1 = createSubStat(0);
	auto subStat2 = createSubStat(1);
	auto subStat3 = createSubStat(2);
	auto subStat4 = createSubStat(3);

	Child content = Column{
		.spacing = 16.f,
		.children{
			levelSelector,
			setSelector,
			slotSelector,
			mainStatSelector,
			subStat1,
			subStat2,
			subStat3,
			subStat4,
		},
	};

	Children buttonFooter{
		Button{
			.widget{.width = Size::Expand},
			.theme = Button::Theme::Accent(element),
			.disabled = disc.set.key == 0,
			.onClick = [this]() {
				closeEvent.notify();
				if (widget->onSubmit) widget->onSubmit(disc);
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
		.title = "Edit disc",
		.content = content,
		.buttons = buttonFooter,
	};
}
