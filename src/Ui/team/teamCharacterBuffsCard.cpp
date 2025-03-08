#include "teamCharacterBuffsCard.hpp"
#include "Ui/character/characterCardBanner.hpp"
#include "Ui/option/toggleOption.hpp"
#include "Ui/option/valueListOption.hpp"
#include "Ui/utils/displayCard.hpp"

#include "character/data.hpp"
#include "rebuilder.hpp"
#include "store.hpp"
#include "weapon/data.hpp"


using namespace squi;

namespace {
	Child teamCharacterBuffsCardBuildFunc(Character::Instance &character, Team::Instance &team) {
		return UI::DisplayCard{
			.title = character.loadout.character.data.name,
			.children{
				UI::CharacterCardBanner{
					.characterKey = character.instanceKey,
				},
			},
			.footer = [&character = character, &team = team]() {
				Children ret{};

				Formula::Context ctx{
					.source = character.loadout,
					.active = character.loadout,
					.team = team.stats,
					.enemy = Store::enemies.at(0).stats,
				};

				auto parseOption = [&](Option::Types &option) {
					std::visit(
						Utils::overloaded{
							[&](Option::Boolean &opt) {
								if (opt.displayCondition && !opt.displayCondition.value().eval(ctx)) return;
								ret.emplace_back(UI::ToggleOption{
									.option = opt,
									.instanceKey = team.instanceKey,
									.ctx = ctx,
								});
							},
							[&](Option::ValueList &opt) {
								if (opt.displayCondition && !opt.displayCondition.value().eval(ctx)) return;
								ret.emplace_back(UI::ValueListOption{
									.option = opt,
									.instanceKey = team.instanceKey,
									.ctx = ctx,
								});
							},
						},
						option
					);
				};
				auto isTeamBuff = [](const Option::Types &option) {
					return std::visit(
						[](auto &&opt) {
							return opt.teamBuff;
						},
						option
					);
				};

				for (auto &opt: character.loadout.weapon->data->data.opts) {
					if (!isTeamBuff(opt)) continue;
					parseOption(character.loadout.weapon->options.at(
						std::visit(
							[](auto &&opt) {
								return opt.key.hash;
							},
							opt
						)
					));
				}

				if (character.loadout.artifact.bonus1.has_value()) {
					for (auto &opt: character.loadout.artifact.bonus1->bonusPtr.opts) {
						if (!isTeamBuff(opt)) continue;
						parseOption(character.loadout.artifact.options.at(
							std::visit(
								[](auto &&opt) {
									return opt.key.hash;
								},
								opt
							)
						));
					}
				}
				if (character.loadout.artifact.bonus2.has_value()) {
					for (auto &opt: character.loadout.artifact.bonus2->bonusPtr.opts) {
						if (!isTeamBuff(opt)) continue;
						parseOption(character.loadout.artifact.options.at(
							std::visit(
								[](auto &&opt) {
									return opt.key.hash;
								},
								opt
							)
						));
					}
				}

				for (const auto &[optPtr, condition]: Option::CharacterList::getMembersAndConditions()) {
					if (!condition.eval(ctx)) continue;
					auto &optList = std::invoke(optPtr, character.loadout.character.data.data.opts);
					for (auto &optionData: optList) {
						bool isTeamBuff = std::visit(
							[](auto &&opt) {
								return opt.teamBuff;
							},
							optionData
						);
						if (!isTeamBuff) continue;
						parseOption(character.loadout.character.options.at(
							std::visit(
								[](auto &&opt) {
									return opt.key.hash;
								},
								optionData
							)
						));
					}
				}

				return ret;
			}(),
		};
	}
}// namespace

UI::TeamCharacterBuffsCard::operator squi::Child() const {
	auto storage = std::make_shared<Storage>();

	return Rebuilder{
		.rebuildEvent = character.updateEvent,
		.buildFunc = std::bind(teamCharacterBuffsCardBuildFunc, std::ref(character), std::ref(team)),
	};
}
