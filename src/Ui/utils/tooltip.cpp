#include "tooltip.hpp"

#include "box.hpp"
#include "gestureDetector.hpp"
#include "text.hpp"
#include "window.hpp"
#include "wrapper.hpp"


using namespace squi;
UI::Tooltip::operator squi::Child() const {
	return squi::Wrapper{
		.onInit = [](Widget &w) {
			w.customState.add(State{});
		},
		.child = GestureDetector{
			.onEnter = [message = message, maxWidth = maxWidth](GestureDetector::Event event) {
				auto &state = event.widget.customState.get<State>();

				state.destroyEvent.notify();

				Window::of(&event.widget).addOverlay(Box{
					.widget{
						.width = Size::Wrap,
						.height = Size::Shrink,
						.sizeConstraints{
							.maxWidth = maxWidth,
						},
						.padding = 8.f,
						.onInit = [destroyEvent = state.destroyEvent](Widget &w) {
							w.customState.add(destroyEvent.observe([&w]() {
								w.deleteLater();
							}));
						},
						.onUpdate = [follow = event.widget.weak_from_this()](Widget &w) {
							if (follow.expired()) w.deleteLater();
						},
						.onArrange = [follow = event.widget.weak_from_this()](Widget &self, vec2 &pos) {
							if (auto w = follow.lock()) {
								pos = w->getPos().withYOffset(-self.getSize().y).withXOffset((w->getSize().x - self.getSize().x) / 2.f);
								pos.x = std::max(pos.x, 0.f);
								pos.x = std::min(pos.x, self.state.parent->getSize().x - self.getSize().x);
								pos.y = std::max(pos.y, 0.f);
								pos.y = std::min(pos.y, self.state.parent->getSize().y - self.getSize().y);
							}
						},
					},
					.color = 0x000000FF,
					.borderRadius{4.f},
					.child = Text{
						.text = message,
						.lineWrap = true,
					},
				});
			},
			.onLeave = [](GestureDetector::Event event) {
				event.widget.customState.get<State>().destroyEvent.notify();
			},
			.child = child,
		},
	};
}
