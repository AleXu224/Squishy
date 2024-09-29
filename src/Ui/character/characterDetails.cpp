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

#include "formula/stat.hpp"

#include "modifiers/artifact/displayStats.hpp"
#include "modifiers/weapon/displayStats.hpp"

#include "box.hpp"
#include "scrollableFrame.hpp"

using namespace squi;

template<auto sheet = int{}>
struct DetailsSkill {
	// Args
	std::string_view name;
	Character::InstanceKey characterKey{};
	const Formula::Context &ctx;
	const std::vector<Node::Types> &nodes;
	size_t maxPreModifierIndex = 0;
	size_t maxPostModifierIndex = 0;
	std::optional<std::unordered_map<uint32_t, std::reference_wrapper<Option::Types>>> options;

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

				if constexpr (!std::is_same_v<decltype(sheet), int>) {
					Children ret3{};

					bool transparent = true;
					for (const auto &stat: Stats::all) {
						auto message = Formula::EvalStat(sheet, stat, [&](auto &&stat) {
							return stat.print(ctx, Formula::Step::none);
						});
						auto value = Formula::EvalStat(sheet, stat, [&](auto &&stat) {
							return stat.eval(ctx);
						});

						if (value == 0.f) continue;
						ret3.emplace_back(UI::Tooltip{
							.message = message,
							.child = UI::SkillEntry{
								.isTransparent = transparent = !transparent,
								.name = Utils::Stringify(stat),
								.value = value,
								.color = Color(0xFFFFFFFF),
								.isPercentage = Utils::isPercentage(stat),
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

				if (!options.has_value()) return ret;
				if (options->empty()) return ret;

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

							for (const auto &option: options.value()) {
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

namespace {
	void initializeList(Character::InstanceKey characterKey, Team::InstanceKey teamKey, Enemy::Key enemyKey, Widget &w) {
		auto &character = ::Store::characters.at(characterKey);
		auto &team = ::Store::teams.at(teamKey);
		auto &enemy = ::Store::enemies.at(enemyKey);
		Formula::Context ctx{
			.source = character.loadout,
			.target = character.loadout,
			.team = team.stats,
			.enemy = enemy.stats,
		};

		w.addChild(UI::CharacterStats{.ctx = ctx});
		w.addChild(UI::CharacterOptions{.characterKey = characterKey});
		w.addChild(UI::CharacterTransformativeReactions{.ctx = ctx});

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

		using MakeOptsRet = std::unordered_map<uint32_t, std::reference_wrapper<Option::Types>>;
		auto makeOpts = [](std::unordered_map<uint32_t, Option::Types> &opts) {
			MakeOptsRet ret{};
			for (auto &[key, value]: opts) {
				ret.insert({key, std::ref(value)});
			}
			return ret;
		};

		auto weaponOpts = makeOpts(character.loadout.weapon.options);

		w.addChild(DetailsSkill<Modifiers::Weapon::displayStats>{
			.name = character.loadout.weapon.data.name,
			.characterKey = characterKey,
			.ctx = ctx,
			.nodes = character.loadout.weapon.data.data.nodes,
			.maxPreModifierIndex = 2,
			.maxPostModifierIndex = 1,
			.options = weaponOpts,
		});

		auto makeArtifactOpts = [](const Option::ArtifactList &opts, Option::ArtifactMap &map) {
			MakeOptsRet ret{};
			for (const auto &opt: opts) {
				std::visit([&](auto &&val) {
					ret.insert({val.key, map.at(val.key)});
				},
						   opt);
			}
			return ret;
		};

		std::optional<MakeOptsRet> artifactOpts1 = [&]() -> std::optional<MakeOptsRet> {
			if (character.loadout.artifact.bonus1.has_value())
				return makeArtifactOpts(character.loadout.artifact.bonus1->bonusPtr.opts, character.loadout.artifact.options);

			return std::nullopt;
		}();
		std::optional<MakeOptsRet> artifactOpts2 = [&]() -> std::optional<MakeOptsRet> {
			if (character.loadout.artifact.bonus2.has_value())
				return makeArtifactOpts(character.loadout.artifact.bonus2->bonusPtr.opts, character.loadout.artifact.options);

			return std::nullopt;
		}();
		if (character.loadout.artifact.bonus1.has_value()) {
			w.addChild(DetailsSkill<Modifiers::Artifact::display1>{
				.name = character.loadout.artifact.bonus1->setPtr.name,
				.characterKey = characterKey,
				.ctx = ctx,
				.nodes = character.loadout.artifact.bonus1->bonusPtr.nodes,
				.maxPreModifierIndex = 1,
				.maxPostModifierIndex = 1,
				.options = artifactOpts1,
			});
		}
		if (character.loadout.artifact.bonus1.has_value()) {
			w.addChild(DetailsSkill<Modifiers::Artifact::display2>{
				.name = character.loadout.artifact.bonus2->setPtr.name,
				.characterKey = characterKey,
				.ctx = ctx,
				.nodes = character.loadout.artifact.bonus2->bonusPtr.nodes,
				.maxPreModifierIndex = 1,
				.maxPostModifierIndex = 1,
				.options = artifactOpts2,
			});
		}
	}
}// namespace

UI::CharacterDetails::operator squi::Child() const {
	return ScrollableFrame{
		.children{
			Masonry{
				.widget{
					.height = Size::Shrink,
					.padding = Padding{8.f},
					.onInit = [characterKey = characterKey, teamKey = teamKey, enemyKey = enemyKey](Widget &w) {
						w.customState.add(::Store::characters.at(characterKey).updateEvent.observe([wPtr = w.weak_from_this(), characterKey, teamKey, enemyKey]() {
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
