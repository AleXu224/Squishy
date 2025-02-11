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
#include "characterStats.hpp"
#include "characterTransformativeReactions.hpp"
#include "rebuilder.hpp"
#include "store.hpp"
#include "utils/overloaded.hpp"

#include "formula/requires.hpp"
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
	std::string_view subtitle{};
	Character::InstanceKey characterKey{};
	const Formula::Context &ctx;
	const std::vector<Node::Types> &nodes;
	std::optional<std::unordered_map<uint32_t, std::reference_wrapper<Option::Types>>> options;
	Formula::BoolNode displayCondition = Formula::ConstantBool(true);

	operator squi::Child() const {
		if (!displayCondition.eval(ctx)) return nullptr;

		return UI::DisplayCard{
			.title = name,
			.subtitle = subtitle,
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

				if (!options.has_value() || options->empty())
					return ret;

				ret.emplace_back(Box{
					.widget{
						.height = Size::Shrink,
						.padding = Padding{4.f},
					},
					.color{1.f, 1.f, 1.f, 0.05f},
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
	using MakeOptsRet = std::unordered_map<uint32_t, std::reference_wrapper<Option::Types>>;
	MakeOptsRet makeOpts(std::unordered_map<uint32_t, Option::Types> &opts) {
		MakeOptsRet ret{};
		for (auto &[key, value]: opts) {
			ret.insert({key, std::ref(value)});
		}
		return ret;
	}
	MakeOptsRet makeArtifactOpts(const Option::ArtifactList &opts, Option::ArtifactMap &map) {
		MakeOptsRet ret{};
		for (const auto &opt: opts) {
			std::visit(
				[&](auto &&val) {
					ret.insert({val.key, map.at(val.key)});
				},
				opt
			);
		}
		return ret;
	}

	Child makeMainContent(Character::InstanceKey characterKey, Team::InstanceKey teamKey, Enemy::Key enemyKey) {
		auto &character = ::Store::characters.at(characterKey);
		auto &team = ::Store::teams.at(teamKey);
		auto &enemy = ::Store::enemies.at(enemyKey);
		Formula::Context ctx{
			.source = character.loadout,
			.target = character.loadout,
			.team = team.stats,
			.enemy = enemy.stats,
		};

		auto transformativeReactions = UI::CharacterTransformativeReactions{.ctx = ctx};

		auto characterStats = UI::CharacterStats{
			.ctx = ctx,
			.characterKey = characterKey,
		};

		auto weaponOpts = makeOpts(character.loadout.weapon.options);

		auto weaponStats = DetailsSkill<Modifiers::Weapon::displayStats>{
			.name = character.loadout.weapon.data->name,
			.subtitle = "Weapon",
			.characterKey = characterKey,
			.ctx = ctx,
			.nodes = character.loadout.weapon.data->data.nodes,
			.options = weaponOpts,
		};

		std::optional<MakeOptsRet> artifactOpts1;
		if (character.loadout.artifact.bonus1.has_value())
			artifactOpts1 = makeArtifactOpts(character.loadout.artifact.bonus1->bonusPtr.opts, character.loadout.artifact.options);

		std::optional<MakeOptsRet> artifactOpts2;
		if (character.loadout.artifact.bonus2.has_value())
			artifactOpts2 = makeArtifactOpts(character.loadout.artifact.bonus2->bonusPtr.opts, character.loadout.artifact.options);

		Child artifactStats1 = character.loadout.artifact.bonus1.has_value()
								 ? DetailsSkill<Modifiers::Artifact::display1>{
									   .name = character.loadout.artifact.bonus1->setPtr.name,
									   .subtitle = "Artifact",
									   .characterKey = characterKey,
									   .ctx = ctx,
									   .nodes = character.loadout.artifact.bonus1->bonusPtr.nodes,
									   .options = artifactOpts1,
								   }
								 : Child{};

		Child artifactStats2 = character.loadout.artifact.bonus2.has_value()
								 ? DetailsSkill<Modifiers::Artifact::display2>{
									   .name = character.loadout.artifact.bonus2->setPtr.name,
									   .subtitle = "Artifact",
									   .characterKey = characterKey,
									   .ctx = ctx,
									   .nodes = character.loadout.artifact.bonus2->bonusPtr.nodes,
									   .options = artifactOpts2,
								   }
								 : Child{};

		Children mainContent{
			characterStats,
			transformativeReactions,
			weaponStats,
			artifactStats1,
			artifactStats2,
		};

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
		const std::vector<std::pair<std::string_view, Formula::BoolNode>> namesAndConditions = {
			{"Normal attack", Formula::ConstantBool(true)},
			{"Charged attack", Formula::ConstantBool(true)},
			{"Plunge attack", Formula::ConstantBool(true)},
			{"Elemental skill", Formula::ConstantBool(true)},
			{"Elemental burst", Formula::ConstantBool(true)},
			{"Passive 1", Requirement::passive1},
			{"Passive 2", Requirement::passive2},
			{"Constellation 1", Requirement::constellation1},
			{"Constellation 2", Requirement::constellation2},
			{"Constellation 4", Requirement::constellation4},
			{"Constellation 6", Requirement::constellation6},
		};

		for (const auto &[nodeWrapper, options, nameAndCondition]: std::views::zip(nodes, characterOpts, namesAndConditions)) {
			auto &[name, condition] = nameAndCondition;
			const auto &nodes = nodeWrapper.get();
			if (nodes.empty() && options.empty()) continue;

			mainContent.emplace_back(DetailsSkill<0>{
				.name = name,
				.characterKey = characterKey,
				.ctx = ctx,
				.nodes = nodes,
				.options = options,
				.displayCondition = condition,
			});
		}

		return UI::Masonry{
			.widget{
				.height = Size::Shrink,
				.padding = 8.f,
			},
			.spacing = 4.f,
			.columnCount = UI::Masonry::MinSize{256.f},
			.children = mainContent,
		};
	}
}// namespace

UI::CharacterDetails::operator squi::Child() const {
	// TODO: make each item rebuild individually
	return ScrollableFrame{
		.children{
			Rebuilder{
				.rebuildEvent = Store::characters.at(characterKey).updateEvent,
				.buildFunc = std::bind(makeMainContent, characterKey, teamKey, enemyKey),
			},
		},
	};
}
