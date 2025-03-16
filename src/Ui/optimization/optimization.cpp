#include "optimization.hpp"
#include "Ui/combo/nodePicker.hpp"
#include "Ui/utils/card.hpp"
#include "button.hpp"
#include "column.hpp"
#include "optimization/optimize.hpp"
#include "store.hpp"


using namespace squi;
UI::Optimization::operator squi::Child() const {
	auto storage = std::make_shared<Storage>();
	auto &character = ::Store::characters.at(characterKey);
	auto &team = teamKey ? ::Store::teams.at(teamKey.value()) : ::Store::defaultTeam;
	auto &enemy = ::Store::enemies.at(enemyKey);
	Formula::Context ctx{
		.source = character.loadout,
		.active = character.loadout,
		.team = team.stats,
		.enemy = enemy.stats,
	};

	return Card{
		.widget{
			.padding = 8.f,
		},
		.child = Column{
			.children{
				NodePicker{
					.characterKey = characterKey,
					.ctx = ctx,
					.onSelect = [storage](const Combo::Source::Types &source) {
						storage->nodeSource = source;
					},
				},
				Button{
					.text = "Optimize",
					.onClick = [ctx, &character, storage](auto) {
						if (!storage->nodeSource.has_value()) return;
						auto &&node = std::visit(
							[](auto &&node) {
								return node.resolve().formula;
							},
							storage->nodeSource.value()
						);
						::Optimization::Optimization optimization{
							.character = character,
							.ctx = ctx,
							.optimizedNode = node,
						};

						optimization.optimize();
					},
				},
			},
		},
	};
}
