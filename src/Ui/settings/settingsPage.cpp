#include "settingsPage.hpp"

#include "button.hpp"
#include "expander.hpp"
#include "scrollableFrame.hpp"

#include "glaze/glaze.hpp"// IWYU pragma: keep
#include "nfd.hpp"

#include "fstream"

#include "serialization/good/ICharacterHelpers.hpp"
#include "store.hpp"

using namespace squi;

UI::SettingsPage::operator squi::Child() const {
	auto storage = std::make_shared<Storage>();

	return ScrollableFrame{
		.scrollableWidget{
			.padding = 8.f,
		},
		.alignment = Scrollable::Alignment::center,
		.spacing = 8.f,
		.children{
			Expander{
				.widget{
					.sizeConstraints{
						.maxWidth = 800.f,
					},
				},
				.heading = "Save to GOOD format (genshin optimizer)",
				.actions{
					Button{
						.text = "Save",
						.style = ButtonStyle::Standard(),
						.onClick = [](GestureDetector::Event) {
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
					},
				},
			},
			Expander{
				.widget{
					.sizeConstraints{
						.maxWidth = 800.f,
					},
				},
				.heading = "Load GOOD format save (genshin optimizer)",
				.actions{
					Button{
						.text = "Load",
						.style = ButtonStyle::Standard(),
						.onClick = [](GestureDetector::Event) {
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
					},
				},
			},
		},
	};
}
