#include "characterDetails.hpp"

#include "Ui/character/characterDetailsSkill.hpp"
#include "Ui/combo/comboDisplay.hpp"
#include "Ui/optimization/optimization.hpp"
#include "Ui/utils/masonry.hpp"
#include "artifact/set.hpp"
#include "character/data.hpp"
#include "characterStats.hpp"
#include "characterTransformativeReactions.hpp"
#include "ranges"
#include "rebuilder.hpp"
#include "store.hpp"


#include "formula/requires.hpp"
#include "formula/stat.hpp"// IWYU pragma: keep

#include "modifiers/artifact/displayStats.hpp"
#include "modifiers/weapon/displayStats.hpp"

#include "scrollableFrame.hpp"

using namespace squi;

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

		auto weaponOpts = makeOpts(character.loadout.weapon->options);

		std::variant<Character::InstanceKey, Team::InstanceKey> keyParam = characterKey;
		if (teamKey.has_value()) keyParam = teamKey.value();

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
			artifactOpts1 = makeArtifactOpts(character.loadout.artifact.bonus1->bonusPtr.opts, character.loadout.artifact.options);

		std::optional<MakeOptsRet> artifactOpts2;
		if (character.loadout.artifact.bonus2.has_value())
			artifactOpts2 = makeArtifactOpts(character.loadout.artifact.bonus2->bonusPtr.opts, character.loadout.artifact.options);

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
			transformativeReactions,
			combos,
			weaponStats,
			artifactStats1,
			artifactStats2,
		};

		std::vector<std::unordered_map<uint32_t, std::reference_wrapper<Option::Types>>> characterOpts{};
		for (auto &optPtr: Option::CharacterList::getMembers()) {
			const auto &optList = std::invoke(optPtr, character.loadout.character.data.data.opts);
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

			mainContent.emplace_back(UI::DetailsSkill{
				.name = name,
				.instanceKey = keyParam,
				.ctx = ctx,
				.nodes = nodes,
				.options = options,
				.displayCondition = condition,
			});
		}

		return UI::Masonry{
			.widget{
				.height = Size::Shrink,
				.sizeConstraints{
					.maxWidth = 1520.f,
				},
			},
			.spacing = 4.f,
			.columnCount = UI::Masonry::MinSize{250.f},
			.children = mainContent,
		};
	}
}// namespace

UI::CharacterDetails::operator squi::Child() const {
	// TODO: make each item rebuild individually
	return ScrollableFrame{
		.scrollableWidget{
			.padding = 4.f,
		},
		.alignment = squi::Column::Alignment::center,
		.spacing = 4.f,
		.children{
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
