#include "homePage.hpp"

#include "UI/artifact/artifactPage.hpp"
#include "UI/character/characterList.hpp"
#include "UI/settings/settingsPage.hpp"
#include "UI/team/teamPage.hpp"
#include "UI/weapon/weaponPage.hpp"
#include "widgets/topNav.hpp"


using namespace squi;

squi::Child UI::HomePage::build(const squi::Element &) const {
	return TopNav{
		.pages{
			TopNav::Page{
				.name{"Artifacts"},
				.icon = 0xe545,
				.content = ArtifactPage{},
			},
			TopNav::Page{
				.name{"Characters"},
				.icon = 0xe7fd,
				.content = CharacterList{},
			},
			TopNav::Page{
				.name{"Teams"},
				.icon = 0xf233,
				.content = TeamPage{},
			},
			TopNav::Page{
				.name{"Weapons"},
				.icon = 0xf889,
				.content = WeaponPage{},
			},
			TopNav::Page{
				.name{"Settings"},
				.icon = 0xe8b8,
				.content = SettingsPage{},
			},
		},
	};
}
