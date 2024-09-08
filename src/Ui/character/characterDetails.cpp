#include "characterDetails.hpp"

#include "Ui/option/toggleOption.hpp"
#include "Ui/option/valueListOption.hpp"
#include "Ui/utils/displayCard.hpp"
#include "Ui/utils/masonry.hpp"
#include "Ui/utils/skillEntry.hpp"
#include "Ui/utils/tooltip.hpp"
#include "Ui/utils/trueFalse.hpp"
#include "artifact/set.hpp"
#include "character/data.hpp"
#include "characterOptions.hpp"
#include "characterStats.hpp"
#include "characterTransformativeReactions.hpp"
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
	Character::InstanceKey characterKey{};
	const Formula::Context &ctx;
	const std::vector<Node::Types> &nodes;
	std::optional<std::reference_wrapper<T>> sheet{};
	size_t maxPreModifierIndex = 0;
	size_t maxPostModifierIndex = 0;
	std::unordered_map<uint32_t, std::reference_wrapper<Option::Types>> &options;

	operator squi::Child() const {
		return UI::DisplayCard{
			.title = name,
			.children = [&]() -> Children {
				Children ret{};

				if (!nodes.empty()) {
					Children ret2{};
					for (const auto &[node, transparent]: std::views::zip(nodes, Utils::trueFalse)) {
						ret2.emplace_back(UI::Tooltip{
							.message = node.formula.print(ctx),
							.child = UI::SkillEntry{
								.isTransparent = transparent,
								.name = node.name,
								.value = node.formula.eval(ctx),
								.color = Node::getColor(node.data, ctx),
								.isPercentage = Node::isPercentage(node.data),
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
					for (const auto &[prePtr, postPtr, preTeamPtr, postTeamPtr]: std::views::zip(
							 Stats::getSheetValuesMembers<decltype(T::preMods)>(),
							 Stats::getSheetValuesMembers<decltype(T::postMods)>(),
							 Stats::getSheetValuesMembers<decltype(T::teamPreMods)>(),
							 Stats::getSheetValuesMembers<decltype(T::teamPostMods)>()
						 )) {
						auto &statPre = std::invoke(prePtr, sheet.value().get().preMods);
						auto &statPost = std::invoke(postPtr, sheet.value().get().postMods);
						auto &statTeamPre = std::invoke(preTeamPtr, sheet.value().get().teamPreMods);
						auto &statTeamPost = std::invoke(postTeamPtr, sheet.value().get().teamPostMods);
						auto &modifiersPre = statPre.modifiers;
						auto &modifiersPost = statPost.modifiers;
						auto &modifiersTeamPre = statTeamPre.modifiers;
						auto &modifiersTeamPost = statTeamPost.modifiers;

						std::vector<std::string> a{};
						for (auto &modifier: modifiersPre | std::views::take(maxPreModifierIndex)) {
							if (!modifier.hasValue()) continue;
							a.emplace_back(modifier.print(ctx));
						}
						for (auto &modifier: modifiersPost | std::views::take(maxPostModifierIndex)) {
							if (!modifier.hasValue()) continue;
							a.emplace_back(modifier.print(ctx));
						}
						for (auto &modifier: modifiersTeamPre | std::views::take(maxPreModifierIndex)) {
							if (!modifier.hasValue()) continue;
							a.emplace_back(modifier.print(ctx));
						}
						for (auto &modifier: modifiersTeamPost | std::views::take(maxPostModifierIndex)) {
							if (!modifier.hasValue()) continue;
							a.emplace_back(modifier.print(ctx));
						}
						auto message = std::accumulate(
							a.begin(), a.end(),
							std::string(),
							[&](const std::string &val1, const std::string &val2) {
								return std::format("{}{}{}", val1, ((val1.empty() || val2.empty()) ? "" : " + "), val2);
							}
						);

						float totalValue = 0.f;
						for (auto &modifier: modifiersPre | std::views::take(maxPreModifierIndex)) {
							if (!modifier.hasValue()) continue;
							totalValue += modifier.eval(ctx);
						}
						for (auto &modifier: modifiersPost | std::views::take(maxPostModifierIndex)) {
							if (!modifier.hasValue()) continue;
							totalValue += modifier.eval(ctx);
						}
						for (auto &modifier: modifiersTeamPre | std::views::take(maxPreModifierIndex)) {
							if (!modifier.hasValue()) continue;
							totalValue += modifier.eval(ctx);
						}
						for (auto &modifier: modifiersTeamPost | std::views::take(maxPostModifierIndex)) {
							if (!modifier.hasValue()) continue;
							totalValue += modifier.eval(ctx);
						}

						if (totalValue == 0.f) continue;
						ret3.emplace_back(UI::Tooltip{
							.message = message,
							.child = UI::SkillEntry{
								.isTransparent = transparent = !transparent,
								.name = Utils::Stringify(Stats::getSheetMemberStat(prePtr)),
								.value = totalValue,
								.color = Color(0xFFFFFFFF),
								.isPercentage = Stats::isSheetMemberPercentage(prePtr),
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

				if (options.empty()) return ret;

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

							for (auto &option: options) {
								std::visit(
									Utils::overloaded{
										[&](Option::Boolean &opt) {
											ret.emplace_back(UI::ToggleOption{
												.option = opt,
												.characterKey = characterKey,
											});
										},
										[&](Option::ValueList &opt) {
											ret.emplace_back(UI::ValueListOption{
												.option = opt,
												.characterKey = characterKey,
											});
										},
									},
									option.second.get()
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

inline void initializeList(Character::InstanceKey characterKey, Team::Key teamKey, Enemy::Key enemyKey, Widget &w) {
	auto &character = Store::characters.at(characterKey);
	auto &team = Store::teams.at(teamKey);
	auto &enemy = Store::enemies.at(enemyKey);
	Formula::Context ctx{
		.source = character.loadout,
		.target = character.loadout,
		.team = team.stats,
		.enemy = enemy.stats,
	};

	w.addChild(UI::CharacterStats{.characterKey = characterKey});
	w.addChild(UI::CharacterOptions{.characterKey = characterKey});
	w.addChild(UI::CharacterTransformativeReactions{.characterKey = characterKey});

	std::vector<std::unordered_map<uint32_t, std::reference_wrapper<Option::Types>>> characterOpts{};
	for (auto &optPtr: Option::CharacterList::getMembers()) {
		const auto &optList = std::invoke(optPtr, character.loadout.character.data.opts);
		std::unordered_map<uint32_t, std::reference_wrapper<Option::Types>> ret{};
		for (const auto &opt: optList) {
			std::visit(
				[&](auto &&optVisited) {
					ret.insert({optVisited.key.hash, std::ref(character.loadout.character.options.at(optVisited.key.hash))});
				},
				opt
			);
		}
		characterOpts.emplace_back(ret);
	}
	std::vector<std::reference_wrapper<const std::vector<Node::Types>>> nodes{};
	for (auto &nodePtr: Node::CharacterList::getMembers()) {
		nodes.emplace_back(std::invoke(nodePtr, character.loadout.character.data.data.nodes));
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

	for (const auto &[nodeWrapper, options, name]: std::views::zip(nodes, characterOpts, names)) {
		const auto &nodes = nodeWrapper.get();
		if (nodes.empty() && options.empty()) continue;

		w.addChild(DetailsSkill{
			.name = name,
			.characterKey = characterKey,
			.ctx = ctx,
			.nodes = nodes,
			.options = options,
		});
	}

	auto makeOpts = [](std::unordered_map<uint32_t, Option::Types> &opts) {
		std::unordered_map<uint32_t, std::reference_wrapper<Option::Types>> ret{};
		for (auto &[key, value]: opts) {
			ret.insert({key, std::ref(value)});
		}
		return ret;
	};

	auto weaponOpts = makeOpts(character.loadout.weapon.options);

	w.addChild(DetailsSkill<Stats::WeaponSheet>{
		.name = character.loadout.weapon.data.name,
		.characterKey = characterKey,
		.ctx = ctx,
		.nodes = character.loadout.weapon.data.data.nodes,
		.sheet = std::ref(character.loadout.weapon.sheet),
		.maxPreModifierIndex = 2,
		.maxPostModifierIndex = 1,
		.options = weaponOpts,
	});

	std::vector<Node::Types> artiNodesPlaceholder{};
	auto artifactOpts = makeOpts(character.loadout.artifact.options);
	w.addChild(DetailsSkill<Stats::ArtifactSheet>{
		.name = character.loadout.artifact.set.has_value() ? character.loadout.artifact.set->get().name : "Artifacts",
		.characterKey = characterKey,
		.ctx = ctx,
		.nodes = character.loadout.artifact.set.has_value() ? character.loadout.artifact.set->get().data.nodes : artiNodesPlaceholder,
		.sheet = std::ref(character.loadout.artifact.sheet),
		.maxPreModifierIndex = 2,
		.maxPostModifierIndex = 2,
		.options = artifactOpts,
	});
}

UI::CharacterDetails::operator squi::Child() const {
	return ScrollableFrame{
		.children{
			Masonry{
				.widget{
					.height = Size::Shrink,
					.padding = Padding{8.f},
					.onInit = [characterKey = characterKey, teamKey = teamKey, enemyKey = enemyKey](Widget &w) {
						w.customState.add(Store::characters.at(characterKey).updateEvent.observe([wPtr = w.weak_from_this(), characterKey, teamKey, enemyKey]() {
							if (auto w = wPtr.lock()) {
								w->setChildren({});
								initializeList(characterKey, teamKey, enemyKey, *w);
							}
						}));
						initializeList(characterKey, teamKey, enemyKey, w);
					},
				},
				.spacing = 4.f,
				.columnCount = Masonry::MinSize{256.f},
			}
		},
	};
}
