#include "teamCharacterBuffsCard.hpp"
#include "UI/character/characterCardBanner.hpp"
#include "UI/option/toggleOption.hpp"
#include "UI/option/valueListOption.hpp"
#include "UI/option/valueSliderOption.hpp"
#include "UI/utils/displayCard.hpp"

#include "character/data.hpp"
#include "store.hpp"
#include "weapon/data.hpp"


using namespace squi;

squi::core::Child UI::TeamCharacterBuffsCard::State::build(const Element &element) {
	auto &character = widget->character;
	auto &team = widget->team;

	return UI::DisplayCard{
		.title = std::string(character.state.stats.data.name),
		.children{
			UI::CharacterCardBanner{
				.characterKey = character.instanceKey,
			},
		},
		.footer = [&character = character, &team = team]() {
			Children ret{};

			auto activeCharacter = team.stats.characters.at(team.stats.activeCharacterIndex);
			Formula::Context ctx{
				.source = character.state,
				.prevSource = activeCharacter ? activeCharacter->state : character.state,
				.active = character.state,
				.team = team.stats,
				.enemy = Store::enemies.at(0).stats,
			};

			auto parseOption = [&](Option::Types &option) {
				std::visit(
					Utils::overloaded{
						[&](Option::Boolean &opt) {
							if (opt.displayCondition.hasValue() && !opt.displayCondition.eval(ctx)) return;
							ret.emplace_back(UI::ToggleOption{
								.option = opt,
								.instanceKey = team.instanceKey,
								.ctx = ctx,
							});
						},
						[&](Option::ValueList &opt) {
							if (opt.displayCondition.hasValue() && !opt.displayCondition.eval(ctx)) return;
							ret.emplace_back(UI::ValueListOption{
								.option = opt,
								.instanceKey = team.instanceKey,
								.ctx = ctx,
							});
						},
						[&](Option::ValueSlider &opt) {
							if (opt.displayCondition.hasValue() && !opt.displayCondition.eval(ctx)) return;
							ret.emplace_back(UI::ValueSliderOption{
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

			for (auto &opt: character.state.loadout().weapon->data->data.opts) {
				if (!isTeamBuff(opt)) continue;
				parseOption(character.state.options.at(
					std::visit(
						[](auto &&opt) {
							return opt.key.hash;
						},
						opt
					)
				));
			}

			if (character.state.loadout().artifact.bonus1.has_value()) {
				for (auto &opt: character.state.loadout().artifact.bonus1->bonusPtr->opts) {
					if (!isTeamBuff(opt)) continue;
					parseOption(character.state.options.at(
						std::visit(
							[](auto &&opt) {
								return opt.key.hash;
							},
							opt
						)
					));
				}
			}
			if (character.state.loadout().artifact.bonus2.has_value()) {
				for (auto &opt: character.state.loadout().artifact.bonus2->bonusPtr->opts) {
					if (!isTeamBuff(opt)) continue;
					parseOption(character.state.options.at(
						std::visit(
							[](auto &&opt) {
								return opt.key.hash;
							},
							opt
						)
					));
				}
			}

			for (const auto &slot: Node::characterSlots) {
				auto condition = Option::CharacterList::conditionFromCharacterSlot(slot);
				if (!condition.eval(ctx)) continue;
				auto &optList = character.state.stats.data.data->opts.fromCharacterSlot(slot);
				for (auto &optionData: optList) {
					bool isTeamBuff = std::visit(
						[](auto &&opt) {
							return opt.teamBuff;
						},
						optionData
					);
					if (!isTeamBuff) continue;
					parseOption(character.state.options.at(
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
