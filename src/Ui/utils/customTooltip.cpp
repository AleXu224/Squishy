#include "customTooltip.hpp"

#include "gestureDetector.hpp"
#include "wrapper.hpp"

using namespace squi;

CustomTooltip::operator squi::Child() const {
	auto storage = std::make_shared<Storage>();

	VoidObservable destroyEvent{};

	return GestureDetector{
		.onEnter = [target = ChildRef(child), tooltip = tooltip, destroyEvent](GestureDetector::Event event) {
			event.widget.addOverlay(Wrapper{
				.onInit = [destroyEvent = destroyEvent](Widget &w) {
					observe(w, destroyEvent, [&w]() {
						w.deleteLater();
					});
				},
				.onUpdate = [target](Widget &w) {
					if (target.expired()) w.deleteLater();
				},
				.onArrange = [target](Widget &self, vec2 &pos) {
					if (auto w = target.lock()) {
						pos = w->getPos().withYOffset(-self.getSize().y).withXOffset((w->getSize().x - self.getSize().x) / 2.f);
						pos.x = std::max(pos.x, 0.f);
						pos.x = std::min(pos.x, self.state.parent->getSize().x - self.getSize().x);
						pos.y = std::max(pos.y, 0.f);
						pos.y = std::min(pos.y, self.state.parent->getSize().y - self.getSize().y);
					}
				},
				.child = tooltip,
			});
		},
		// .onLeave = [destroyEvent](GestureDetector::Event event) {
		// 	destroyEvent.notify();
		// },
		.child = child,
	};
}
