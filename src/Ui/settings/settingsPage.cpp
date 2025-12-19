#include "settingsPage.hpp"


#include "glaze/glaze.hpp"// IWYU pragma: keep
#include "nfd.hpp"

#include "widgets/button.hpp"
#include "widgets/expander.hpp"
#include "widgets/scrollview.hpp"

#include "fstream"

#include "serialization/good/ICharacterHelpers.hpp"
#include "store.hpp"

using namespace squi;

squi::core::Child UI::SettingsPage::State::build(const Element &element) {
	return ScrollView{
		.scrollWidget{
			.padding = 8.f,
		},
		.alignment = Flex::Alignment::center,
		.spacing = 8.f,
		.children{
			Expander{
				.widget{
					.sizeConstraints = BoxConstraints{
						.maxWidth = 800.f,
					},
				},
				.title = "Save to GOOD format (genshin optimizer)",
				.action{
					Button{
						.theme = Button::Theme::Standard(),
						.onClick = []() {
							NFD::Guard guard{};
							NFD::UniquePath path{};
							nfdu8filteritem_t filters[2] = {{"JSON File", "json"}};
							auto res = NFD::SaveDialog(path, filters, 1, nullptr);
							if (res == NFD_OKAY) {
								auto file = std::ofstream(path.get());
								if (!file) return;

								auto data = Store::saveToGOOD();
								std::string jsonData{};
								auto res = glz::write<glz::opts{.prettify = false}>(data, jsonData);
								if (res) return;

								file << jsonData;
								file.close();
							}
						},
						.child = "Save",
					},
				},
			},
			Expander{
				.widget{
					.sizeConstraints = BoxConstraints{
						.maxWidth = 800.f,
					},
				},
				.title = "Load GOOD format save (genshin optimizer)",
				.action{
					Button{
						.theme = Button::Theme::Standard(),
						.onClick = []() {
							NFD::Guard guard{};
							NFD::UniquePath path{};
							nfdu8filteritem_t filters[2] = {{"JSON File", "json"}};
							auto res = NFD::OpenDialog(path, filters, 1);
							if (res == NFD_OKAY) {
								auto file = std::ifstream(path.get());
								if (!file) return;

								std::stringstream str{};
								str << file.rdbuf();
								file.close();

								Serialization::Good::IGOOD dst{};
								auto res = glz::read<glz::opts{.error_on_unknown_keys = false}>(dst, str.str());
								if (res) return;

								Store::loadFromGOOD(dst);
							}
						},
						.child = "Load",
					},
				},
			},
		},
	};
}
