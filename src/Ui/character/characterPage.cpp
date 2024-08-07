#include "characterPage.hpp"

#include "Ui/utils/grid.hpp"
#include "characterCard.hpp"
#include "characterDetails.hpp"
#include "characterStatInspector.hpp"
#include "gestureDetector.hpp"
#include "navigator.hpp"
#include "scrollableFrame.hpp"
#include "store.hpp"
#include "window.hpp"
#include <GLFW/glfw3.h>

using namespace squi;

UI::CharacterPage::operator squi::Child() const {
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
							for (auto &[_, character]: Store::characters) {
								w.addChild(GestureDetector{
									.onClick = [controller, characterKey = character.key](GestureDetector::Event event) {
										controller.push(CharacterDetails{
											.characterKey = characterKey,
											.controller = controller,
										});
									},
									.onUpdate = [controller, characterKey = character.key](GestureDetector::Event event) {
										if (event.state.active && GestureDetector::isKey(GLFW_MOUSE_BUTTON_2, GLFW_RELEASE)) {
											Window::of(&event.widget).addOverlay(CharacterStatInspector{
												.characterKey = characterKey,
											});
										}
									},
									.child = CharacterCard{
										.character = character,
									},
								});
							}
						},
					},
					.spacing = 2.f,
					.columnCount = Grid::MinSize{.value = 256.f},
				}
			},
		},
	};
}
