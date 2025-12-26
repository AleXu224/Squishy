#include "homePage.hpp"

#include "Ui/artifact/artifactPage.hpp"
#include "widgets/topNav.hpp"
#include "Ui/character/characterList.hpp"
#include "Ui/settings/settingsPage.hpp"
// #include "Ui/team/teamPage.hpp"
#include "Ui/weapon/weaponPage.hpp"


using namespace squi;

squi::Child UI::HomePage::build(const squi::Element &) const {
	return TopNav{
		.pages{
			TopNav::Page{
				.name{"Artifacts"},
				.content = ArtifactPage{},
			},
			TopNav::Page{
				.name{"Characters"},
				.content = CharacterList{},
			},
			// TopNav::Page{
			// 	.name{"Teams"},
			// 	.content = TeamPage{},
			// },
			TopNav::Page{
				.name{"Weapons"},
				.content = WeaponPage{},
			},
			TopNav::Page{
				.name{"Settings"},
				.content = SettingsPage{},
			},
		},
	};
}
