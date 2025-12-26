
#include "characterDetails.hpp"

#include "Ui/artifact/artifactCard.hpp"
#include "Ui/character/characterDetailsSkill.hpp"
#include "Ui/combo/comboDisplay.hpp"
#include "Ui/optimization/optimization.hpp"
#include "Ui/optimization/tcOptimization.hpp"
#include "Ui/utils/card.hpp"
#include "Ui/utils/masonry.hpp"
#include "Ui/weapon/weaponCard.hpp"
#include "artifact/set.hpp"
#include "character/data.hpp"
#include "characterStats.hpp"
#include "characterTransformativeReactions.hpp"
#include "ranges"
#include "store.hpp"


#include "formula/stat.hpp"// IWYU pragma: keep

#include "modifiers/artifact/displayStats.hpp"
#include "modifiers/weapon/displayStats.hpp"

#include "utils/slotToCondition.hpp"
#include "weapon/data.hpp"
#include "widgets/column.hpp"
#include "widgets/expander.hpp"
#include "widgets/grid.hpp"
#include "widgets/scrollview.hpp"
#include <map>

using namespace squi;

namespace {
	using MakeOptsRet = std::map<uint32_t, std::reference_wrapper<Option::Types>>;
	MakeOptsRet makeOptsSimple(std::unordered_map<uint32_t, Option::Types> &opts) {
		MakeOptsRet ret{};
		for (auto &[key, value]: opts) {
			ret.insert({key, std::ref(value)});
		}
		return ret;
	}

	MakeOptsRet makeOpts(const Option::WeaponList &optList, std::unordered_map<uint32_t, Option::Types> &opts) {
		MakeOptsRet ret{};
		for (const auto &opt: optList) {
			const auto &key = std::visit(
				[](auto &&val) {
					return val.key.hash;
				},
				opt
			);
			if (opts.contains(key)) {
				ret.insert({key, std::ref(opts.at(key))});
			}
		}
		return ret;
	}
	MakeOptsRet makeArtifactOpts(const Option::ArtifactList &opts, Option::TypesMap &map) {
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

	Child makeMainContent(Character::InstanceKey characterKey, std::optional<Team::InstanceKey> teamKey, Enemy::Key enemyKey) {
		auto &character = ::Store::characters.at(characterKey);
		auto &team = teamKey ? ::Store::teams.at(teamKey.value()) : ::Store::defaultTeam;
		auto &enemy = ::Store::enemies.at(enemyKey);
		Formula::Context ctx{
			.source = character.state,
			.active = character.state,
			.team = team.stats,
			.enemy = enemy.stats,
		};

		auto transformativeReactions = UI::CharacterTransformativeReactions{.ctx = ctx};

		auto combos = UI::ComboDisplay{
			.characterKey = characterKey,
			.ctx = ctx,
		};

		auto characterStats = UI::CharacterStats{
			.ctx = ctx,
			.characterKey = characterKey,
		};

		std::variant<Character::InstanceKey, Team::InstanceKey> keyParam = characterKey;
		if (teamKey.has_value()) keyParam = teamKey.value();

		auto teamOpts = makeOptsSimple(team.stats.options);

		std::vector<Node::Types> nodesPlaceholder;
		auto teamStats = UI::DetailsSkill{
			.name = "Resonance",
			.instanceKey = keyParam,
			.ctx = ctx,
			.nodes = nodesPlaceholder,
			.options = teamOpts,
			.modsGenerator = std::make_shared<UI::ModsGenerator>(),
		};

		auto weaponOpts = makeOpts(character.state.loadout().weapon->data->data.opts, character.state.options);

		auto weaponStats = UI::DetailsSkill{
			.name = character.state.loadout().weapon->data->name,
			.subtitle = "Weapon",
			.instanceKey = keyParam,
			.ctx = ctx,
			.nodes = character.state.loadout().weapon->data->data.nodes,
			.options = weaponOpts,
			.modsGenerator = std::make_shared<UI::DerivedModsGenerator>(Modifiers::Weapon::displayStats()),
		};

		std::optional<MakeOptsRet> artifactOpts1;
		if (character.state.loadout().artifact.bonus1.has_value())
			artifactOpts1 = makeArtifactOpts(character.state.loadout().artifact.bonus1->bonusPtr->opts, character.state.options);

		std::optional<MakeOptsRet> artifactOpts2;
		if (character.state.loadout().artifact.bonus2.has_value())
			artifactOpts2 = makeArtifactOpts(character.state.loadout().artifact.bonus2->bonusPtr->opts, character.state.options);

		Child artifactStats1 = character.state.loadout().artifact.bonus1.has_value()
								 ? UI::DetailsSkill{
									   .name = std::string(character.state.loadout().artifact.bonus1->setPtr->name),
									   .subtitle = "Artifact",
									   .instanceKey = keyParam,
									   .ctx = ctx,
									   .nodes = character.state.loadout().artifact.bonus1->bonusPtr->nodes,
									   .options = artifactOpts1,
									   .modsGenerator = std::make_shared<UI::DerivedModsGenerator>(Modifiers::Artifact::display1()),
								   }
								 : Child{};

		Child artifactStats2 = character.state.loadout().artifact.bonus2.has_value()
								 ? UI::DetailsSkill{
									   .name = std::string(character.state.loadout().artifact.bonus2->setPtr->name),
									   .subtitle = "Artifact",
									   .instanceKey = keyParam,
									   .ctx = ctx,
									   .nodes = character.state.loadout().artifact.bonus2->bonusPtr->nodes,
									   .options = artifactOpts2,
									   .modsGenerator = std::make_shared<UI::DerivedModsGenerator>(Modifiers::Artifact::display2()),
								   }
								 : Child{};

		Child leftColumn = Column{
			.spacing = 4.f,
			.children{
				teamStats,
				transformativeReactions,
				combos,
				weaponStats,
				artifactStats1,
				artifactStats2,
			},
		};

		Children normalColumnChildren{};
		Children skillColumnChildren{};
		Children burstColumnChildren{};
		Children otherColumnChildren{};

		std::vector<std::map<uint32_t, std::reference_wrapper<Option::Types>>> characterOpts{};
		for (auto &optPtr: Option::CharacterList::getMembers()) {
			const auto &optList = std::invoke(optPtr, character.state.stats.data.data.opts);
			characterOpts.emplace_back(makeOpts(optList, character.state.options));
		}
		std::vector<std::reference_wrapper<const std::vector<Node::Types>>> nodes{};
		for (auto &nodePtr: Node::CharacterList::getMembers()) {
			nodes.emplace_back(std::invoke(nodePtr, character.state.stats.data.data.nodes));
		}

		for (const auto &[nodeWrapper, options, slot]: std::views::zip(nodes, characterOpts, Node::characterSlots)) {
			auto name = Utils::Stringify(slot);
			auto condition = Utils::slotToCondition(slot);
			const auto &nodes = nodeWrapper.get();
			if (nodes.empty() && options.empty()) continue;

			auto ret = UI::DetailsSkill{
				.name = name,
				.instanceKey = keyParam,
				.ctx = ctx,
				.nodes = nodes,
				.options = options,
				.displayCondition = condition,
			};
			switch (slot) {
				case Node::CharacterSlot::normal:
				case Node::CharacterSlot::charged:
				case Node::CharacterSlot::plunge:
					normalColumnChildren.emplace_back(ret);
					break;
				case Node::CharacterSlot::skill:
					skillColumnChildren.emplace_back(ret);
					break;
				case Node::CharacterSlot::burst:
					burstColumnChildren.emplace_back(ret);
					break;
				default:
					otherColumnChildren.emplace_back(ret);
					break;
			}
		}

		Child normalColumn = Column{.spacing = 4.f, .children = normalColumnChildren};
		Child skillColumn = Column{.spacing = 4.f, .children = skillColumnChildren};
		Child burstColumn = Column{.spacing = 4.f, .children = burstColumnChildren};
		Child otherColumn = Column{.spacing = 4.f, .children = otherColumnChildren};

		Children mainContent{
			characterStats,
			leftColumn,
			normalColumn,
			skillColumn,
			burstColumn,
			otherColumn,
		};

		return Column{
			.widget{
				.height = Size::Shrink,
				.sizeConstraints = BoxConstraints{
					.maxWidth = 1520.f,
				},
			},
			.spacing = 4.f,
			.children{
				UI::Masonry{
					.columnCount = UI::Masonry::MinSize{250.f},
					.spacing = 4.f,
					.children = mainContent,
				},
			},
		};
	}

	struct EquipmentExpander : StatelessWidget {
		// Args
		Key key;
		Args widget{};
		Character::InstanceKey characterKey;

		[[nodiscard]] Child build(const Element &) const {
			auto &character = ::Store::characters.at(characterKey);

			return Expander{
				.widget = widget,
				.title = "Equipment",
				.content = Grid{
					.widget{
						.padding = Padding{8.f, 8.f},
					},
					.columnCount = UI::Grid::MinSize{250.f},
					.spacing = 4.f,
					.children = [&]() {
						Children ret;
						ret.emplace_back(UI::WeaponCard{
							.weapon = ::Store::weapons.at(character.state.loadout().weaponInstanceKey),
							.actions = UI::WeaponCard::Actions::character,
						});
						std::visit(
							Utils::overloaded{
								[&](const Stats::Artifact::Slotted &artifacts) {
									for (const auto &slot: Artifact::slots) {
										const auto &key = artifacts.fromSlot(slot);
										if (!key) {
											ret.emplace_back(UI::Card{});
											continue;
										}
										auto &artifact = ::Store::artifacts.at(key);
										ret.emplace_back(UI::ArtifactCard{
											.artifact = artifact,
											.actions = UI::ArtifactCard::Actions::character,
										});
									}
								},
								[&](const Stats::Artifact::Theorycraft &stats) {
									// FIXME: add display for theorycraft display
								},
							},
							character.state.loadout().artifact.equipped
						);
						return ret;
					}(),
				},
			};
		}
	};
}// namespace

squi::core::Child UI::CharacterDetails::State::build(const Element &) {
	auto &character = Store::characters.at(widget->characterKey);
	return ScrollView{
		.scrollWidget{
			.padding = 4.f,
		},
		.alignment = Flex::Alignment::center,
		.spacing = 4.f,
		.children{
			EquipmentExpander{
				.widget{
					.sizeConstraints = BoxConstraints{
						.maxWidth = 1520.f,
					},
				},
				.characterKey = widget->characterKey,
			},
			makeMainContent(widget->characterKey, widget->teamKey, widget->enemyKey),
			std::visit(//
				Utils::overloaded{
					[&](const Stats::Artifact::Slotted &lotted) -> Child {
						return UI::Optimization{
							.characterKey = widget->characterKey,
							.teamKey = widget->teamKey,
							.enemyKey = widget->enemyKey,
						};
					},
					[&](const Stats::Artifact::Theorycraft &theorycraft) -> Child {
						return UI::TCOptimization{
							.characterKey = widget->characterKey,
							.teamKey = widget->teamKey,
							.enemyKey = widget->enemyKey,
						};
					},
				},
				character.state.loadout().artifact.equipped
			),
		},
	};
}
