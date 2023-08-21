#include "UI/BackDrawer.hpp"
#include "ranges"

using namespace squi;
using namespace Squishy;

BackDrawer::Impl::Impl(const BackDrawer &args) : Widget(args.widget, Widget::Flags::Default()) {
	setChildren(args.children);
}

void BackDrawer::Impl::updateChildren() {
	auto &children = getChildren();

	bool first = true;
	for (auto &child: children | std::views::reverse) { 
		child->state.parent = this;
		child->state.root = state.root;
		child->setVisible(first);
		first = false;
	}

	if (!children.empty()) {
        children.back()->update();
	}
}