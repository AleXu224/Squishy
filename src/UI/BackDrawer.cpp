#include "UI/BackDrawer.hpp"

using namespace squi;
using namespace Squishy;

BackDrawer::Impl::Impl(const BackDrawer &args) : Widget(args.widget, Widget::Flags::Default()) {
	setChildren(args.children);
}

void BackDrawer::Impl::updateChildren() {
	auto &children = getChildren();

	for (auto &child: children) {
		child->state.parent = this;
		child->state.root = state.root;
		child->flags.visible = false;
	}

	if (!children.empty()) {
		children.back()->flags.visible = true;
        children.back()->update();
	}
}