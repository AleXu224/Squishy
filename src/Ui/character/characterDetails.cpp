#include "characterDetails.hpp"

#include "Ui/conditional/toggleConditional.hpp"
#include "Ui/conditional/valueListConditional.hpp"
#include "Ui/elementToColor.hpp"
#include "Ui/utils/card.hpp"
#include "Ui/utils/masonry.hpp"
#include "Ui/utils/statDisplay.hpp"
#include "Ui/utils/tooltip.hpp"
#include "Ui/utils/trueFalse.hpp"
#include "artifact/set.hpp"
#include "character/data.hpp"
#include "store.hpp"
#include "utils/overloaded.hpp"


#include "align.hpp"
#include "box.hpp"
#include "column.hpp"
#include "navigationView.hpp"
#include "scrollableFrame.hpp"
#include "stack.hpp"
#include "text.hpp"
#include "wrapper.hpp"
#include <numeric>


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
				.height = Size::Shrink,
				.padding = Padding{16.f},
			},
			.color{1.f, 1.f, 1.f, 0.1f},
			.borderRadius{7.f, 7.f, 0.f, 0.f},
			.child = Align{
				.xAlign = 0.f,
				.child = Text{
					.text = name,
					.fontSize = 20.f,
					.lineWrap = true,
					.font = FontStore::defaultFontBold,
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
				.margin = Margin{0.f},
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
			std::array displayStats{Stats::characterDisplayStats, std::vector{Stats::fromElement(character.stats.character.base.element)}};

			Children ret2{};

			for (auto [stat, transparent]: std::views::zip(std::views::join(displayStats), Utils::trueFalse)) {
				ret2.emplace_back(UI::Tooltip{
					.message = [&]() {
						std::vector<std::string> a{};
						auto &modifiers = character.stats.character.sheet.fromStat(stat).modifiers;
						for (auto &modifier: modifiers) {
							a.emplace_back(modifier.print(character.stats));
						}
						return std::accumulate(
							a.begin(), a.end(),
							std::string(),
							[&](const std::string &val1, const std::string &val2) {
								return val1 + (val1.empty() ? "" : " + ") + val2;
							}
						);
					}(),
					.child = UI::StatDisplay{
						.isTransparent = transparent,
						.stat{
							.stat = stat,
							.value = character.stats.character.sheet.fromStat(stat).getTotal(character.stats),
						},
					},
				});
			}
			ret.emplace_back(Column{
				.widget{
					.padding = Padding{4.f},
				},
				.children = ret2,
			});
			return ret;
		}(),
	});

	std::vector<std::reference_wrapper<std::unordered_map<size_t, Conditional::Types>>> conditionals{};
	for (auto &condPtr: Conditional::CharacterMap::getMembers()) {
		conditionals.emplace_back(std::invoke(condPtr, character.stats.character.conditionals));
	}
	conditionals.emplace_back(character.stats.weapon.conditionals);
	conditionals.emplace_back(character.stats.artifact.conditionals);
	std::vector<std::reference_wrapper<std::vector<Node::Types>>> nodes{};
	for (auto &nodePtr: Node::CharacterList::getMembers()) {
		nodes.emplace_back(std::invoke(nodePtr, character.stats.character.data.nodes));
	}
	nodes.emplace_back(character.stats.weapon.data.nodes);
	if (character.stats.artifact.set.has_value()) {
		nodes.emplace_back(character.stats.artifact.set->get().nodes);
	}
	const std::vector<std::string_view> names = {
		"Normal attack",
		"Charged attack",
		"Plunge attack",
		"Elemental skill",
		"Elemental burst",
		"Passive 1",
		"Passive 2",
		"Constellation 1",
		"Constellation 2",
		"Constellation 4",
		"Constellation 6",
		character.stats.weapon.data.name,
		character.stats.artifact.set.has_value() ? character.stats.artifact.set->get().name : "",
	};

	for (const auto &[nodeWrapper, conditionalWrapper, name]: std::views::zip(nodes, conditionals, names)) {
		auto &nodes = nodeWrapper.get();
		auto &conditionals = conditionalWrapper.get();
		if (nodes.empty() && conditionals.empty()) continue;

		w.addChild(CharacterDetailsSkill{
			.title = name,
			.children = [&]() -> Children {
				Children ret{};

				if (!nodes.empty()) {
					Children ret2{};
					for (auto [node, transparent]: std::views::zip(nodes, Utils::trueFalse)) {
						ret2.emplace_back(UI::Tooltip{
							.message = node.formula.print(character.stats),
							.child = SkillEntry{
								.isTransparent = transparent,
								.name = node.name,
								.value = node.formula.eval(character.stats),
								.color = Utils::elementToColor(Formula::_getElement(node.source, node.element, character.stats)),
							},
						});
					}
					ret.emplace_back(Column{
						.widget{
							.padding = Padding{4.f},
						},
						.children = ret2,
					});
				}

				if (conditionals.empty()) return ret;

				ret.emplace_back(Box{
					.widget{
						.height = Size::Shrink,
						.padding = Padding{4.f},
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
												.characterKey = characterKey,
											});
										},
										[&](Conditional::ValueList &cond) {
											ret.emplace_back(UI::ValueListConditional{
												.conditional = cond,
												.characterKey = characterKey,
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
								w.customState.add(Store::characters.at(characterKey).updateEvent.observe([wPtr = w.weak_from_this(), characterKey]() {
									if (auto w = wPtr.lock()) {
										w->setChildren({});
										initializeList(characterKey, *w);
									}
								}));
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
