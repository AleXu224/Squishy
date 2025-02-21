#include "teamCharacterBuffsCard.hpp"
#include "Ui/option/toggleOption.hpp"
#include "Ui/option/valueListOption.hpp"
#include "Ui/utils/displayCard.hpp"

#include "character/data.hpp"
#include "rebuilder.hpp"
#include "store.hpp"


using namespace squi;

namespace {
	Child teamCharacterBuffsCardBuildFunc(Character::Instance &character, Team::Instance &team) {
		return UI::DisplayCard{
			.title = character.loadout.character.data.name,
			.footer = [&character = character, &team = team]() {
				Children ret{};

				Formula::Context ctx{
					.source = character.loadout,
					.active = character.loadout,
					.team = team.stats,
					.enemy = Store::enemies.at(0).stats,
				};

				for (const auto &optPtr: Option::CharacterList::getMembers()) {
					auto &optList = std::invoke(optPtr, character.loadout.character.data.opts);
					for (auto &optionData: optList) {
						bool isTeamBuff = std::visit(
							[](auto &&opt) {
								return opt.teamBuff;
							},
							optionData
						);
						if (!isTeamBuff) continue;
						std::visit(
							Utils::overloaded{
								[&](Option::Boolean &opt) {
									if (opt.displayCondition && !opt.displayCondition.value().eval(ctx)) return;
									ret.emplace_back(UI::ToggleOption{
										.option = opt,
										.characterKey = character.instanceKey,
									});
								},
								[&](Option::ValueList &opt) {
									if (opt.displayCondition && !opt.displayCondition.value().eval(ctx)) return;
									ret.emplace_back(UI::ValueListOption{
										.option = opt,
										.characterKey = character.instanceKey,
									});
								},
							},
							character.loadout.character.options.at(
								std::visit(
									[](auto &&opt) {
										return opt.key.hash;
									},
									optionData
								)
							)
						);
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
