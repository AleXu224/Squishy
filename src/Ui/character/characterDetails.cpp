#include "characterDetails.hpp"

#include "Ui/artifact/artifactCard.hpp"
#include "Ui/character/characterDetailsSkill.hpp"
#include "Ui/combo/comboDisplay.hpp"
#include "Ui/optimization/optimization.hpp"
#include "Ui/utils/card.hpp"
#include "Ui/utils/grid.hpp"
#include "Ui/utils/masonry.hpp"
#include "Ui/weapon/weaponCard.hpp"
#include "artifact/set.hpp"
#include "character/data.hpp"
#include "characterStats.hpp"
#include "characterTransformativeReactions.hpp"
#include "column.hpp"
#include "expander.hpp"
#include "ranges"
#include "rebuilder.hpp"
#include "store.hpp"


#include "formula/stat.hpp"// IWYU pragma: keep

#include "modifiers/artifact/displayStats.hpp"
#include "modifiers/weapon/displayStats.hpp"

#include "scrollableFrame.hpp"
#include "utils/slotToCondition.hpp"
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
			.source = character.loadout,
			.active = character.loadout,
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

		auto weaponOpts = makeOpts(character.loadout.weapon->data->data.opts, character.loadout.options);

		auto weaponStats = UI::DetailsSkill{
			.name = character.loadout.weapon->data->name,
			.subtitle = "Weapon",
			.instanceKey = keyParam,
			.ctx = ctx,
			.nodes = character.loadout.weapon->data->data.nodes,
			.options = weaponOpts,
			.modsGenerator = std::make_shared<UI::DerivedModsGenerator<Modifiers::Weapon::displayStats>>(),
		};

		std::optional<MakeOptsRet> artifactOpts1;
		if (character.loadout.artifact.bonus1.has_value())
			artifactOpts1 = makeArtifactOpts(character.loadout.artifact.bonus1->bonusPtr.opts, character.loadout.options);

		std::optional<MakeOptsRet> artifactOpts2;
		if (character.loadout.artifact.bonus2.has_value())
			artifactOpts2 = makeArtifactOpts(character.loadout.artifact.bonus2->bonusPtr.opts, character.loadout.options);

		Child artifactStats1 = character.loadout.artifact.bonus1.has_value()
								 ? UI::DetailsSkill{
									   .name = character.loadout.artifact.bonus1->setPtr.name,
									   .subtitle = "Artifact",
									   .instanceKey = keyParam,
									   .ctx = ctx,
									   .nodes = character.loadout.artifact.bonus1->bonusPtr.nodes,
									   .options = artifactOpts1,
									   .modsGenerator = std::make_shared<UI::DerivedModsGenerator<Modifiers::Artifact::display1>>(),
								   }
								 : Child{};

		Child artifactStats2 = character.loadout.artifact.bonus2.has_value()
								 ? UI::DetailsSkill{
									   .name = character.loadout.artifact.bonus2->setPtr.name,
									   .subtitle = "Artifact",
									   .instanceKey = keyParam,
									   .ctx = ctx,
									   .nodes = character.loadout.artifact.bonus2->bonusPtr.nodes,
									   .options = artifactOpts2,
									   .modsGenerator = std::make_shared<UI::DerivedModsGenerator<Modifiers::Artifact::display2>>(),
								   }
								 : Child{};

		Children mainContent{
			characterStats,
			teamStats,
			transformativeReactions,
			combos,
			weaponStats,
			artifactStats1,
			artifactStats2,
		};

		std::vector<std::map<uint32_t, std::reference_wrapper<Option::Types>>> characterOpts{};
		for (auto &optPtr: Option::CharacterList::getMembers()) {
			const auto &optList = std::invoke(optPtr, character.loadout.character.data.data.opts);
			characterOpts.emplace_back(makeOpts(optList, character.loadout.options));
		}
		std::vector<std::reference_wrapper<const std::vector<Node::Types>>> nodes{};
		for (auto &nodePtr: Node::CharacterList::getMembers()) {
			nodes.emplace_back(std::invoke(nodePtr, character.loadout.character.data.data.nodes));
		}

		for (const auto &[nodeWrapper, options, slot]: std::views::zip(nodes, characterOpts, Node::characterSlots)) {
			auto name = Utils::Stringify(slot);
			auto condition = Utils::slotToCondition(slot);
			const auto &nodes = nodeWrapper.get();
			if (nodes.empty() && options.empty()) continue;

			mainContent.emplace_back(UI::DetailsSkill{
				.name = name,
				.instanceKey = keyParam,
				.ctx = ctx,
				.nodes = nodes,
				.options = options,
				.displayCondition = condition,
			});
		}

		return Column{
			.widget{
				.height = Size::Shrink,
				.sizeConstraints{
					.maxWidth = 1520.f,
				},
			},
			.spacing = 4.f,
			.children{
				UI::Masonry{
					.spacing = 4.f,
					.columnCount = UI::Masonry::MinSize{250.f},
					.children = mainContent,
				},
			},
		};
	}

	struct EquipmentExpander {
		// Args
		squi::Widget::Args widget{};
		Character::InstanceKey characterKey;

		static inline Child makeContent(Character::InstanceKey characterKey) {
			auto &character = ::Store::characters.at(characterKey);

			return UI::Grid{
				.widget{
					.padding = Padding{8.f, 8.f},
				},
				.spacing = 4.f,
				.columnCount = UI::Grid::MinSize{250.f},
				.children = [&]() {
					Children ret;
					ret.emplace_back(UI::WeaponCard{
						.weapon = ::Store::weapons.at(character.weaponInstanceKey),
						.actions = UI::WeaponCard::Actions::character,
					});
					for (const auto &slot: Artifact::slots) {
						auto &key = character.loadout.artifact.equipped.fromSlot(slot);
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
					return ret;
				}(),
			};
		}

		operator squi::Child() const {
			return Expander{
				.widget = widget,
				.heading = "Equipment",
				.expandedContent = Rebuilder{
					.rebuildEvent = ::Store::characters.at(characterKey).updateEvent,
					.buildFunc = std::bind(makeContent, characterKey),
				},
			};
		}
	};
}// namespace

UI::CharacterDetails::operator squi::Child() const {
	// TODO: make each item rebuild individually
	return ScrollableFrame{
		.scrollableWidget{
			.padding = 4.f,
		},
		.alignment = Scrollable::Alignment::center,
		.spacing = 4.f,
		.children{
			EquipmentExpander{
				.widget{
					.sizeConstraints{
						.maxWidth = 1520.f,
					},
				},
				.characterKey = characterKey,
			},
			Rebuilder{
				.rebuildEvent = Store::characters.at(characterKey).updateEvent,
				.buildFunc = std::bind(makeMainContent, characterKey, teamKey, enemyKey),
			},
			UI::Optimization{
				.characterKey = characterKey,
				.teamKey = teamKey,
				.enemyKey = enemyKey,
			},
		},
	};
}
