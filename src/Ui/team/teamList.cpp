#include "teamList.hpp"

#include "Ui/utils/grid.hpp"
#include "teamCard.hpp"
// #include "characterPage.hpp"
#include "gestureDetector.hpp"
#include "navigator.hpp"
#include "scrollableFrame.hpp"
#include "store.hpp"
#include "teamPage.hpp"
#include <GLFW/glfw3.h>

using namespace squi;

UI::TeamList::operator squi::Child() const {
	auto storage = std::make_shared<Storage>();

	Navigator::Controller controller{};

	return Navigator{
		.controller = controller,
		.child = ScrollableFrame{
			.children{
				Grid{
					.widget{
						.height = Size::Shrink,
						.padding = Padding{8.f},
						.onInit = [controller](Widget &w) {
							for (auto &[teamKey, team]: ::Store::teams) {
								w.addChild(GestureDetector{
									.onClick = [controller, teamKey](GestureDetector::Event) {
										controller.push(TeamPage{
											.teamKey = teamKey,
											.controller = controller,
										});
									},
									.child = TeamCard{
										.teamKey = teamKey,
									},
								});
							}
						},
					},
					.spacing = 2.f,
					.columnCount = Grid::MinSize{.value = 300.f},
				}
			},
		},
	};
}
