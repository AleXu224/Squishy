#include "characterTransformativeReactions.hpp"

#include "Ui/elementToColor.hpp"
#include "Ui/utils/displayCard.hpp"
#include "Ui/utils/skillEntry.hpp"
#include "Ui/utils/tooltip.hpp"
#include "character/data.hpp"
#include "reaction/transformative.hpp"
#include "utils.hpp"


using namespace squi;

UI::CharacterTransformativeReactions::operator squi::Child() const {
	auto storage = std::make_shared<Storage>();

	return UI::DisplayCard{
		.widget = widget,
		.title = "Transformative Reactions",
		.children = [&]() -> Children {
			Children ret;
			const auto &state = ctx.active;
			// auto reactions = Reaction::List::Transformative::getMembers();

			auto reactions = std::tuple{
				&Reaction::List::Transformative::burning,
				&Reaction::List::Transformative::superconduct,
				&Reaction::List::Transformative::pyroSwirl,
				&Reaction::List::Transformative::hydroSwirl,
				&Reaction::List::Transformative::electroSwirl,
				&Reaction::List::Transformative::cryoSwirl,
				&Reaction::List::Transformative::electroCharged,
				&Reaction::List::Transformative::shattered,
				&Reaction::List::Transformative::overloaded,
				&Reaction::List::Transformative::bloom,
				&Reaction::List::Transformative::burgeon,
				&Reaction::List::Transformative::hyperbloom,
				&Reaction::List::Transformative::lunarCharged,
				&Reaction::List::Transformative::lunarCrystalize,
			};

			std::vector elements{state.stats.data.baseStats.element};

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
