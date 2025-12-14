#include "artifactEditor.hpp"

#include "Ui/utils/editorItem.hpp"
#include "artifact/sets.hpp"
#include "stats/artifact.hpp"

#include "widgets/column.hpp"
#include "widgets/container.hpp"
#include "widgets/dialog.hpp"
#include "widgets/dropdownButton.hpp"
#include "widgets/numberBox.hpp"


#include "ranges"
#include "widgets/row.hpp"
#include "widgets/stack.hpp"
#include "widgets/text.hpp"

using namespace squi;

struct ArtifactEditorSubstat : StatelessWidget {
	// Args
	Key key;
	Args widget;
	std::string name;
	Children children;

	[[nodiscard]] Child build(const Element &) const {
		auto newWidget = widget;
		newWidget.width = newWidget.width.value_or(Size::Shrink);

		return Stack{
			.widget = newWidget,
			.children{
				Container{
					.widget{
						.width = Size::Shrink,
						.height = 32.f,
					},
					.child = Text{
						.widget{.alignment = Alignment::CenterLeft},
						.text = name,
					},
				},
				Row{
					.widget{
						.width = Size::Shrink,
						.height = Size::Shrink,
						.alignment = Alignment::CenterRight,
					},
					.spacing = 4.f,
					.children = children,
				},
			},
		};
	}
};

void UI::ArtifactEditor::State::updateSlot(Artifact::Slot newSlot) {
	artifact.slot = newSlot;
	const auto &stats = Stats::Artifact::bySlot(newSlot);
	if (auto it = std::ranges::find(stats, artifact.mainStat); it == stats.end()) {
		artifact.mainStat = stats.at(0);
	}
}

squi::core::Child UI::ArtifactEditor::State::createSubStat(size_t subStatIndex) {
	auto &subStat = artifact.subStats.at(subStatIndex);

	Child ret = ArtifactEditorSubstat{
		.name = std::format("Substat {}", subStatIndex + 1),
		.children{
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
						return artifact.mainStat != val;
					};
					for (const auto &stat: std::views::filter(Stats::Artifact::subStats, filter)) {
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

void UI::ArtifactEditor::State::initState() {
	if (widget->artifact.has_value()) {
		artifact = widget->artifact.value();
	} else {
		artifact = Artifact::Instance{};
	}
}

squi::core::Child UI::ArtifactEditor::State::build(const Element &) {
	// Level
	Child levelSelector = EditorItem{
		.name = "Level",
		.child = NumberBox{
			.value = static_cast<double>(artifact.level),
			.min = 0.f,
			.max = 20.f,
			.precision = 0,
			.onChange = [this](double newVal) {
				setState([&]() {
					artifact.level = std::floor(newVal);
				});
			},
		},
	};

	// Set
	Child setButton = DropdownButton{
		.theme = Button::Theme::Standard(),
		.text = artifact.set.key != 0 ? std::string(Artifact::sets.at(artifact.set).name) : "No set",
		.items = [this]() {
			std::vector<ContextMenu::Item> ret{};
			ret.reserve(Artifact::sets.size());
			for (const auto &set: Artifact::sets) {
				ret.emplace_back(ContextMenu::Button{
					.text = std::string(set.second.name),
					.callback = [&set = set.second, this]() {
						setState([&]() {
							artifact.set.key = set.key;
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
		.name = "Slot",
		.child = DropdownButton{
			.theme = Button::Theme::Standard(),
			.disabled = !artifact.usedOn().empty(),
			.text = Utils::Stringify(artifact.slot),
			.items = [&]() {
				std::vector<ContextMenu::Item> ret{};
				ret.reserve(Artifact::slots.size());
				for (const auto &slot: Artifact::slots) {
					ret.emplace_back(ContextMenu::Button{
						.text = Utils::Stringify(slot),
						.callback = [slot, this]() {
							setState([&]() {
								updateSlot(slot);
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
			.text = Utils::Stringify(artifact.mainStat),
			.items = [this]() {
				std::vector<ContextMenu::Item> ret{};
				for (const auto &stat: Stats::Artifact::bySlot(artifact.slot)) {
					ret.emplace_back(ContextMenu::Button{
						.text = Utils::Stringify(stat),
						.callback = [this, stat]() {
							setState([&]() {
								artifact.mainStat = stat;
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
			.theme = Button::Theme::Accent(),
			.disabled = artifact.set.key == 0,
			.onClick = [this]() {
				closeEvent.notify();
				if (widget->onSubmit) widget->onSubmit(artifact);
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
		.title = "Edit artifact",
		.content = content,
		.buttons = buttonFooter,
	};
}
