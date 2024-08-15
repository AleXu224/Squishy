#include "characterDetails.hpp"

#include "Ui/conditional/toggleConditional.hpp"
#include "Ui/conditional/valueListConditional.hpp"
#include "Ui/elementToColor.hpp"
#include "Ui/utils/displayCard.hpp"
#include "Ui/utils/masonry.hpp"
#include "Ui/utils/skillEntry.hpp"
#include "Ui/utils/tooltip.hpp"
#include "Ui/utils/trueFalse.hpp"
#include "artifact/set.hpp"
#include "characterStats.hpp"
#include "store.hpp"
#include "utils/overloaded.hpp"


#include "box.hpp"
#include "scrollableFrame.hpp"

#include "numeric"

using namespace squi;

template<class T = Stats::CharacterSheet>
struct DetailsSkill {
	// Args
	std::string_view name;
	Character::Key characterKey;
	std::vector<Node::Types> &nodes;
	std::optional<std::reference_wrapper<T>> sheet{};
	size_t maxModifierIndex = 0;
	std::unordered_map<size_t, Conditional::Types> &conditionals;

	operator squi::Child() const {
		auto &character = Store::characters.at(characterKey);

		return UI::DisplayCard{
			.title = name,
			.children = [&]() -> Children {
				Children ret{};

				if (!nodes.empty()) {
					Children ret2{};
					for (auto [node, transparent]: std::views::zip(nodes, Utils::trueFalse)) {
						ret2.emplace_back(UI::Tooltip{
							.message = node.formula.print(character.stats),
							.child = UI::SkillEntry{
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

				if (sheet.has_value()) {
					Children ret3{};

					bool transparent = true;
					for (auto ptr: Stats::getSheetValuesMembers<T>()) {
						auto &stat = std::invoke(ptr, sheet.value().get());
						auto &modifiers = stat.modifiers;

						std::vector<std::string> a{};
						for (auto &modifier: modifiers | std::views::take(maxModifierIndex)) {
							if (!modifier.hasValue()) continue;
							a.emplace_back(modifier.print(character.stats));
						}
						auto message = std::accumulate(
							a.begin(), a.end(),
							std::string(),
							[&](const std::string &val1, const std::string &val2) {
								return val1 + ((val1.empty() || val2.empty()) ? "" : " + ") + val2;
							}
						);

						float totalValue = 0.f;
						for (auto &modifier: modifiers | std::views::take(maxModifierIndex)) {
							if (!modifier.hasValue()) continue;
							totalValue += modifier.eval(character.stats);
						}

						if (totalValue == 0.f) continue;
						ret3.emplace_back(UI::Tooltip{
							.message = message,
							.child = UI::SkillEntry{
								.isTransparent = transparent = !transparent,
								.name = Utils::Stringify(Stats::getSheetMemberStat(ptr)),
								.value = totalValue,
								.color = Color(0xFFFFFFFF),
								.isPercentage = Stats::isSheetMemberPercentage(ptr),
							},
						});
					}

					if (!ret3.empty()) {
						ret.emplace_back(Column{
							.widget{.padding = 4.f},
							.children = ret3,
						});
					}
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
		};
	}
};

inline void initializeList(Character::Key characterKey, Widget &w) {
	auto &character = Store::characters.at(characterKey);

	w.addChild(UI::CharacterStats{.characterKey = characterKey});

	std::vector<std::reference_wrapper<std::unordered_map<size_t, Conditional::Types>>> conditionals{};
	for (auto &condPtr: Conditional::CharacterMap::getMembers()) {
		conditionals.emplace_back(std::invoke(condPtr, character.stats.character.conditionals));
	}
	std::vector<std::reference_wrapper<std::vector<Node::Types>>> nodes{};
	for (auto &nodePtr: Node::CharacterList::getMembers()) {
		nodes.emplace_back(std::invoke(nodePtr, character.stats.character.data.nodes));
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
	};

	for (const auto &[nodeWrapper, conditionalWrapper, name]: std::views::zip(nodes, conditionals, names)) {
		auto &nodes = nodeWrapper.get();
		auto &conditionals = conditionalWrapper.get();
		if (nodes.empty() && conditionals.empty()) continue;

		w.addChild(DetailsSkill{
			.name = name,
			.characterKey = characterKey,
			.nodes = nodes,
			.conditionals = conditionals,
		});
	}

	w.addChild(DetailsSkill<Stats::WeaponSheet>{
		.name = character.stats.weapon.data.name,
		.characterKey = characterKey,
		.nodes = character.stats.weapon.data.nodes,
		.sheet = std::ref(character.stats.weapon.sheet),
		.maxModifierIndex = 3,
		.conditionals = character.stats.weapon.conditionals,
	});

	std::vector<Node::Types> artiNodesPlaceholder{};
	w.addChild(DetailsSkill<Stats::ArtifactSheet>{
		.name = character.stats.artifact.set.has_value() ? character.stats.artifact.set->get().name : "Artifacts",
		.characterKey = characterKey,
		.nodes = character.stats.artifact.set.has_value() ? character.stats.artifact.set->get().nodes : artiNodesPlaceholder,
		.sheet = std::ref(character.stats.artifact.sheet),
		.maxModifierIndex = 3,
		.conditionals = character.stats.artifact.conditionals,
	});
}

UI::CharacterDetails::operator squi::Child() const {
	auto storage = std::make_shared<Storage>();

	return ScrollableFrame{
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
	};
}
