#include "teamPage.hpp"

#include "Ui/utils/grid.hpp"
#include "button.hpp"
#include "navigator.hpp"
#include "row.hpp"
#include "scrollableFrame.hpp"
#include "store.hpp"
#include "teamCard.hpp"
#include "teamEditor.hpp"
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

	auto addTeamButton = Button{
		.text = "Add team",
		.onClick = [](GestureDetector::Event event) {
			++Store::lastTeamId;
			auto &instance = Store::teams.emplace(Store::lastTeamId, Team::Instance{.instanceKey{Store::lastTeamId}}).first->second;
			Store::teamListUpdateEvent.notify();

			event.widget.addOverlay(TeamEditor{
				.instance = instance,
				.onSubmit = [](const Team::Instance &newInstance) {
					Store::teams.at(newInstance.instanceKey) = newInstance;
					Store::teamListUpdateEvent.notify();
				},
			});
		},
	};

	auto buttonBar = Row{
		.widget{
			.height = Size::Shrink,
		},
		.children{
			addTeamButton,
		},
	};

	return Navigator{
		.controller = controller,
		.child = ScrollableFrame{
			.scrollableWidget{
				.padding = 8.f,
			},
			.alignment = squi::Column::Alignment::center,
			.spacing = 8.f,
			.children{
				buttonBar,
				Grid{
					.widget{
						.height = Size::Shrink,
						.sizeConstraints{
							.maxWidth = 1520.f,
						},
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
