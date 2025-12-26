#include "comboDisplay.hpp"
#include "Ui/elementToColor.hpp"
#include "Ui/utils/displayCard.hpp"
#include "Ui/utils/skillEntry.hpp"
#include "Ui/utils/trueFalse.hpp"
#include "comboList.hpp"
#include "ranges"
#include "store.hpp"
#include "widgets/button.hpp"
#include "widgets/navigator.hpp"


using namespace squi;
namespace UI {
	struct ComboDisplayEntry : StatelessWidget {
		// Args
		Key key;
		bool transparent;
		const Combo::Combo &combo;
		Formula::Context ctx;
		Color color;

		[[nodiscard]] Child build(const Element &) const {
			return SkillEntry{
				.isTransparent = transparent,
				.name = combo.name,
				.value = combo.eval(ctx),
				.color = Utils::elementToColor(Misc::Element::physical),
			};
		}
	};
}// namespace UI

[[nodiscard]] squi::core::Child UI::ComboDisplay::build(const Element &element) const {
	return DisplayCard{
		.title = "Combos",
		.children = [&]() {
			Children ret;

			auto &character = ::Store::characters.at(characterKey);

			for (const auto &[comboPair, transparent]: std::views::zip(character.combos, Utils::trueFalse)) {
				const auto &[comboKey, combo] = comboPair;
				ret.emplace_back(UI::ComboDisplayEntry{
					.transparent = transparent,
					.combo = combo,
					.ctx = ctx,
					.color = Utils::elementToColor(Misc::Element::physical),
				});
			}

			return ret;
		}(),
		.footer{
			Button{
				.onClick = [&]() {
					Navigator::of(element).pushOverlay(ComboList{
						.characterKey = characterKey,
						.ctx = ctx,
					});
				},
				.child = "Edit",
			},
		},
	};
}
