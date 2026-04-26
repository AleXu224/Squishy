#include "comboDisplay.hpp"
#include "UI/utils/displayCard.hpp"
#include "UI/utils/skillEntry.hpp"
#include "UI/utils/trueFalse.hpp"
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
				.color = Color::white,
			};
		}
	};
}// namespace UI

[[nodiscard]] squi::core::Child UI::ComboDisplay::build(const Element &element) const {
	return DisplayCard{
		.title = "Combos",
		.children = [&]() {
			Children ret;

			auto &agent = ::Store::agents.at(agentKey);

			for (const auto &[comboPair, transparent]: std::views::zip(agent.combos, Utils::trueFalse)) {
				const auto &[comboKey, combo] = comboPair;
				ret.emplace_back(UI::ComboDisplayEntry{
					.transparent = transparent,
					.combo = combo,
					.ctx = ctx,
					.color = Color::white,
				});
			}

			return ret;
		}(),
		.footer{
			Button{
				.onClick = [&]() {
					Navigator::of(element).pushOverlay(ComboList{
						.agentKey = agentKey,
						.ctx = ctx,
					});
				},
				.child = "Edit",
			},
		},
	};
}
