#include "grid.hpp"

#include "ranges"

using namespace squi;

UI::Grid::Impl::Impl(const Grid &args) : Widget(args.widget, Widget::FlagsArgs::Default()), spacing(args.spacing), columnCountRule(args.columnCount) {
	setChildren(args.children);
}

[[nodiscard]] size_t UI::Grid::Impl::computeColumnCount(float availableWidth) const {
	return std::visit(
		[&](const auto &val) -> size_t {
			using T = std::decay_t<decltype(val)>;
			if constexpr (std::is_same_v<T, int>) {
				return val;
			} else if constexpr (std::is_same_v<T, MinSize>) {
				const float &minWidth = val.value;
				return std::max(
					static_cast<size_t>(std::floor(availableWidth / minWidth)),
					static_cast<size_t>(1)
				);
			} else {
				static_assert(false, "someone messed up the types");
			}
		},
		columnCountRule
	);
}

squi::vec2 UI::Grid::Impl::layoutChildren(squi::vec2 maxSize, squi::vec2 minSize, ShouldShrink shouldShrink, bool final) {
	auto &children = getChildren();

	auto columns = computeColumnCount(shouldShrink.width ? 0.f : maxSize.x);

	float maxWidth = 0.f;
	float maxHeight = 0.f;
	auto newMax = (maxSize - static_cast<float>(columns) * spacing) / static_cast<float>(columns);
	auto newMin = (minSize - static_cast<float>(columns) * spacing) / static_cast<float>(columns);
	for (auto &child: children) {
		auto size = child->layout(newMax, newMin, {true, true}, false);
		maxWidth = std::max(maxWidth, size.x);
		maxHeight = std::max(maxHeight, size.y);
	}

	minSize = maxSize = {maxWidth, maxHeight};
	if (!shouldShrink.width) {
		maxSize.x = std::max(maxSize.x, newMax.x);
	}
	for (auto &child: children) {
		child->layout(maxSize, minSize, {false, false}, final);
	}

	size_t rows = (children.size() / columns) + (children.size() % columns == 0 ? 0 : 1);
	rows = std::max(rows, static_cast<size_t>(1));

	if (final) {
		this->columns = columns;
		this->rowHeight = maxHeight;
	}

	return {
		(maxWidth * static_cast<float>(columns)) + (static_cast<float>(columns - 1) * spacing),
		(maxHeight * static_cast<float>(rows)) + (static_cast<float>(rows - 1) * spacing)
	};
}

void UI::Grid::Impl::arrangeChildren(squi::vec2 &pos) {
	auto newPos = pos + state.padding->getPositionOffset() + state.margin->getPositionOffset();
	auto &children = getChildren();

	float yCursor = 0.f;
	for (auto childrenChunk: children | std::views::chunk(columns)) {
		float xCursor = 0.f;
		for (auto &child: childrenChunk) {
			child->arrange(newPos + vec2{xCursor, yCursor});
			xCursor += child->getSize().x + spacing;
		}
		yCursor += rowHeight + spacing;
	}
}
