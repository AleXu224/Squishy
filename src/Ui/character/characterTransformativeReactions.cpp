#include "characterTransformativeReactions.hpp"

#include "Ui/elementToColor.hpp"
#include "Ui/utils/displayCard.hpp"
#include "Ui/utils/skillEntry.hpp"
#include "Ui/utils/tooltip.hpp"
#include "character/data.hpp"
#include "reaction/transformative.hpp"
#include "store.hpp"


using namespace squi;

UI::CharacterTransformativeReactions::operator squi::Child() const {
	auto storage = std::make_shared<Storage>();

	return UI::DisplayCard{
		.widget = widget,
		.title = "Transformative Reactions",
		.children = [characterKey = characterKey]() -> Children {
			Children ret;
			auto &character = Store::characters.at(characterKey);
			auto reactions = Reaction::List::Transformative::getMembers();

			auto &team = Store::teams.at(0);
			auto &enemy = Store::enemies.at(0);

			auto ctx = Formula::Context{
				.source = character.loadout,
				.target = character.loadout,
				.team = team.stats,
				.enemy = enemy.stats,
			};

			std::vector elements{character.loadout.character.data.baseStats.element};

			bool transparent = true;
			squi::utils::iterateTuple(reactions, [&](auto &&val) {
				bool found = false;
				for (const auto &trigger: val->triggers) {
					for (const auto &element: elements) {
						if (trigger == element) found = true;
					}
				}
				if (!found) return;
				ret.emplace_back(UI::Tooltip{
					.message = val->formula.print(ctx, Formula::Step::none),
					.child = UI::SkillEntry{
						.isTransparent = (transparent = !transparent),
						.name = val->name,
						.value = val->formula.eval(ctx),
						.color = Utils::elementToColor(val->damageElement),
					},
				});
			});

			return Children{
				Column{
					.widget{
						.padding = Padding{4.f},
					},
					.children = ret,
				},
			};
		}(),
	};
}
