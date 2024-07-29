#include "homePage.hpp"

#include "Ui/artifact/artifactPage.hpp"
#include "Ui/character/characterPage.hpp"
#include "Ui/utils/topnav.hpp"
#include "align.hpp"
#include "box.hpp"


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
				.name{"3"},
				.content = Align{.xAlign = 1.f, .yAlign = 1.f, .child = Box{.widget{.width = 20.f, .height = 20.f}}},
			},
			TopNav::Page{
				.name{"4"},
				.content = Align{.xAlign = 0.f, .yAlign = 1.f, .child = Box{.widget{.width = 20.f, .height = 20.f}}},
			},
		},
	};
}
