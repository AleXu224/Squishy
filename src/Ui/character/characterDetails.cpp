#include "characterDetails.hpp"

#include "Ui/conditional/toggleConditional.hpp"
#include "Ui/conditional/valueListConditional.hpp"
#include "Ui/elementToColor.hpp"
#include "Ui/utils/card.hpp"
#include "Ui/utils/masonry.hpp"
#include "Ui/utils/statDisplay.hpp"
#include "Ui/utils/trueFalse.hpp"
#include "store.hpp"
#include "utils/overloaded.hpp"


#include "align.hpp"
#include "box.hpp"
#include "column.hpp"
#include "navigationView.hpp"
#include "scrollableFrame.hpp"
#include "stack.hpp"
#include "text.hpp"


using namespace squi;

struct CharacterDetailsSkillHeader {
	// Args
	squi::Widget::Args widget{};
	std::string_view name;

	struct Storage {
		// Data
	};

	operator squi::Child() const {
		auto storage = std::make_shared<Storage>();

		return Box{
			.widget{
				.height = 64.f,
				.padding = Padding{16.f},
			},
			.color{1.f, 1.f, 1.f, 0.1f},
			.borderRadius{7.f, 7.f, 0.f, 0.f},
			.child = Align{
				.xAlign = 0.f,
				.child = Text{
					.text = name,
					.fontSize = 20.f,
				},
			},
		};
	}
};

struct SkillEntry {
	// Args
	bool isTransparent;
	std::string_view name;
	float value;
	Color color;

	struct Storage {
		// Data
	};

	operator squi::Child() const {
		auto storage = std::make_shared<Storage>();

		return Box{
			.widget{
				.height = 36.f,
				.margin = Margin{4.f, 2.f},
				.padding = Padding{12.f, 0.f},
			},
			.color = isTransparent ? Color{1.f, 1.f, 1.f, 0.0419f} : Color{0.f, 0.f, 0.f, 0.f},
			.borderRadius{4.f},
			.child{
				Stack{
					.children{
						Align{
							.xAlign = 0.f,
							.child = Text{
								.text = name,
								.color = color,
							},
						},
						Align{
							.xAlign = 1.f,
							.child = Text{
								.text = std::format("{:.0f}", value),
							},
						},
					},
				},
			}
		};
	}
};

struct CharacterDetailsSkill {
	// Args
	squi::Widget::Args widget{};
	std::string_view title;
	Children children{};

	struct Storage {
		// Data
	};

	operator squi::Child() const {
		auto storage = std::make_shared<Storage>();

		return UI::Card{
			.widget{
				.padding = Padding{1.f},
			},
			.child = Column{
				.children{
					CharacterDetailsSkillHeader{
						.name = title,
					},
					Column{
						.children = children,
					},
				},
			},
		};
	}
};

inline void initializeList(Character::Key characterKey, Widget &w) {
	auto &character = Store::characters.at(characterKey);
	// auto &data = Character::list.at(characterKey);

	w.addChild(CharacterDetailsSkill{
		.title = "Stats",
		.children = [&character]() {
			Children ret{};
			auto displayStats = {Stats::characterDisplayStats, {Stats::fromElement(character.stats.base.element)}};

			for (auto [stat, transparent]: std::views::zip(std::views::join(displayStats), Utils::trueFalse)) {
				ret.emplace_back(UI::StatDisplay{
					.isTransparent = transparent,
					.stat{
						.stat = stat,
						.value = character.stats.sheet.fromStat(stat).getTotal(character.stats.sheet),
					},
				});
			}
			return ret;
		}(),
	});

	const auto names = {"Normal attack", "Charged attack", "Plunge attack", "Elemental skill", "Elemental burst", "Passive 1", "Passive 2", "Constellation 1", "Constellation 2", "Constellation 4", "Constellation 6"};

	for (const auto &[nodePtr, conditionalPtr, name]: std::views::zip(Node::List::getMembers(), Conditional::ListMapped::getMembers(), names)) {
		auto &nodes = std::invoke(nodePtr, character.nodes);
		auto &conditionals = std::invoke(conditionalPtr, character.conditionals);
		if (nodes.empty() && conditionals.empty()) continue;

		w.addChild(CharacterDetailsSkill{
			.title = name,
			.children = [&]() -> Children {
				Children ret{};
				for (auto [node, transparent]: std::views::zip(nodes, Utils::trueFalse)) {
					std::visit(
						[&](auto &&skill) {
							ret.emplace_back(SkillEntry{
								.isTransparent = transparent,
								.name = skill.name,
								.value = skill.calculate(character.stats),
								.color = Utils::elementToColor(skill.getElement(character.stats)),
							});
						},
						node
					);
				}
				if (conditionals.empty()) return ret;

				ret.emplace_back(Box{
					.widget{
						.height = Size::Shrink,
						.padding = Padding{8.f},
					},
					.color{1.f, 1.f, 1.f, 0.1f},
					.borderRadius{0.f, 0.f, 7.f, 7.f},
					.child = Column{
						.spacing = 4.f,
						.children = [&]() {
							Children ret{};

							for (auto &conditional: conditionals) {
								std::visit(
									Utils::overloaded{
										[&](Conditional::Boolean &cond) {
											ret.emplace_back(UI::ToggleConditional{
												.conditional = cond,
											});
										},
										[&](Conditional::ValueList &cond) {
											ret.emplace_back(UI::ValueListConditional{
												.conditional = cond,
											});
										},
									},
									conditional.second
								);
							}

							return ret;
						}(),
					},
				});

				return ret;
			}(),
		});
	}
}

UI::CharacterDetails::operator squi::Child() const {
	auto storage = std::make_shared<Storage>();

	return NavigationView{
		.expanded = false,
		.backAction = [controller = controller]() {
			controller.pop();
		},
		.pages{
			NavigationView::Page{
				.name = "Details",
				.content = ScrollableFrame{
					.children{Masonry{
						.widget{
							.height = Size::Shrink,
							.padding = Padding{8.f},
							.onInit = [characterKey = characterKey](Widget &w) {
								initializeList(characterKey, w);
							},
						},
						.spacing = 4.f,
						.columnCount = Masonry::MinSize{256.f},
					}},
				},
			},
		},
	};
}
