#include "homePage.hpp"

#include "UI/agent/agentList.hpp"
#include "UI/disc/discPage.hpp"
// #include "UI/settings/settingsPage.hpp"
#include "UI/engine/enginePage.hpp"
#include "UI/team/teamPage.hpp"
#include "widgets/topNav.hpp"


using namespace squi;

squi::Child UI::HomePage::build(const squi::Element &) const {
	return TopNav{
		.pages{
			TopNav::Page{
				.name{"Discs"},
				.icon = 0xe545,
				.content = DiscPage{},
			},
			TopNav::Page{
				.name{"Agents"},
				.icon = 0xe7fd,
				.content = AgentList{},
			},
			TopNav::Page{
				.name{"Teams"},
				.icon = 0xf233,
				.content = TeamPage{},
			},
			TopNav::Page{
				.name{"Engines"},
				.icon = 0xf889,
				.content = EnginePage{},
			},
			// TopNav::Page{
			// 	.name{"Settings"},
			// 	.icon = 0xe8b8,
			// 	.content = SettingsPage{},
			// },
		},
	};
}
