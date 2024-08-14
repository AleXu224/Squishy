#include "characterList.hpp"

#include "Ui/utils/grid.hpp"
#include "characterCard.hpp"
#include "characterPage.hpp"
#include "characterStatInspector.hpp"
#include "gestureDetector.hpp"
#include "navigator.hpp"
#include "scrollableFrame.hpp"
#include "store.hpp"
#include "window.hpp"
#include <GLFW/glfw3.h>

using namespace squi;

UI::CharacterList::operator squi::Child() const {
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
										controller.push(CharacterPage{
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
										.characterKey = character.key,
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
