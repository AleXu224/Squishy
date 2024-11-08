#include "teamPage.hpp"

#include "Ui/utils/grid.hpp"
#include "navigator.hpp"
#include "scrollableFrame.hpp"
#include "store.hpp"
#include "teamCard.hpp"
#include <GLFW/glfw3.h>


using namespace squi;

namespace {
	Children makeTeams(const Navigator::Controller &controller) {
		Children ret;
		for (auto &[teamKey, team]: ::Store::teams) {
			ret.emplace_back(UI::TeamCard{
				.teamKey = teamKey,
				.controller = controller,
			});
		}
		return ret;
	}
}// namespace

UI::TeamPage::operator squi::Child() const {
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
							observe(w, Store::teamListUpdateEvent, [&w, controller]() {
								w.setChildren(makeTeams(controller));
							});
							w.setChildren(makeTeams(controller));
						},
					},
					.spacing = 2.f,
					.columnCount = Grid::MinSize{.value = 300.f},
				}
			},
		},
	};
}
