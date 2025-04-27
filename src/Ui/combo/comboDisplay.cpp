#include "comboDisplay.hpp"
#include "Ui/elementToColor.hpp"
#include "Ui/utils/displayCard.hpp"
#include "Ui/utils/skillEntry.hpp"
#include "Ui/utils/trueFalse.hpp"
#include "button.hpp"
#include "comboList.hpp"
#include "ranges"
#include "rebuilder.hpp"
#include "store.hpp"
#include "theme.hpp"


using namespace squi;

UI::ComboDisplay::operator squi::Child() const {
	auto storage = std::make_shared<Storage>();

	return DisplayCard{
		.title = "Combos",
		.children = [characterKey = characterKey, ctx = ctx]() {
			Children ret;

			auto &character = ::Store::characters.at(characterKey);

			for (const auto &[comboPair, transparent]: std::views::zip(character.combos, Utils::trueFalse)) {
				const auto &[comboKey, combo] = comboPair;
				ret.emplace_back(Rebuilder{
					.rebuildEvent = combo.updateEvent,
					.buildFunc = [transparent, &combo, ctx, theme = ThemeManager::getTheme()]() {
						auto _ = ThemeManager::pushTheme(theme);
						return SkillEntry{
							.isTransparent = transparent,
							.name = combo.name,
							.value = combo.eval(ctx),
							.color = Utils::elementToColor(Misc::Element::physical),
						};
					},
				});
			}

			return ret;
		}(),
		.footer{
			Button{
				.text = "Edit",
				.onClick = [characterKey = characterKey, ctx = ctx, theme = ThemeManager::getTheme()](GestureDetector::Event event) {
					auto _ = ThemeManager::pushTheme(theme);
					event.widget.addOverlay(ComboList{
						.characterKey = characterKey,
						.ctx = ctx,
					});
				},
			},
		},
	};
}
