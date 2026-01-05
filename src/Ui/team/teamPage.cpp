#include "teamPage.hpp"

#include "store.hpp"
#include "teamCard.hpp"
#include "teamEditor.hpp"
#include "widgets/button.hpp"
#include "widgets/grid.hpp"
#include "widgets/navigator.hpp"
#include "widgets/row.hpp"
#include "widgets/scrollview.hpp"

using namespace squi;

squi::core::Child UI::TeamPage::State::build(const Element &element) {
	auto addTeamButton = Button{
		.onClick = [this]() {
			++Store::lastTeamId;
			auto &instance = Store::teams.emplace(Store::lastTeamId, Team::Instance{.instanceKey{Store::lastTeamId}}).first->second;
			Store::teamListUpdateEvent.notify();

			Navigator::of(this).pushOverlay(TeamEditor{
				.instance = instance,
				.onSubmit = [](const Team::Instance &newInstance) {
					Store::teams.at(newInstance.instanceKey) = newInstance;
					Store::teamListUpdateEvent.notify();
				},
			});
		},
		.child = "Add team",
	};

	return ScrollView{
		.scrollWidget{
			.padding = 8.f,
		},
		.alignment = Flex::Alignment::center,
		.spacing = 8.f,
		.children{
			Row{
				.widget{
					.height = Size::Shrink,
				},
				.children{
					addTeamButton,
				},
			},
			Grid{
				.widget{
					.height = Size::Shrink,
					.sizeConstraints = BoxConstraints{
						.maxWidth = 1520.f,
					},
				},
				.columnCount = Grid::MinSize{.value = 300.f},
				.spacing = 2.f,
				.children = [&]() {
					Children ret;
					for (auto &[teamKey, team]: ::Store::teams) {
						ret.emplace_back(UI::TeamCard{
							.teamKey = teamKey,
						});
					}
					return ret;
				}(),
			}
		},
	};
}
