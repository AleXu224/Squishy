#include "homePage.hpp"

#include "Ui/artifact/artifactPage.hpp"
#include "Ui/character/characterPage.hpp"
#include "Ui/utils/topnav.hpp"
#include "Ui/weapon/weaponPage.hpp"


using namespace squi;


UI::homePage::operator squi::Child() const {
	auto storage = std::make_shared<Storage>();

	return TopNav{
		.pages{
			TopNav::Page{
				.name{"Artifacts"},
				.content = ArtifactPage{},
			},
			TopNav::Page{
				.name{"Characters"},
				.content = CharacterPage{},
			},
			TopNav::Page{
				.name{"Weapons"},
				.content = WeaponPage{},
			},
		},
	};
}