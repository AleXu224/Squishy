#include "UI/Grid.hpp"
#include "ranges"
#include <algorithm>

using namespace squi;
using namespace Squishy;

Grid::Impl::Impl(const Grid &args) : Widget(args.widget, Widget::Flags::Default()), spacing(args.spacing), minChildWidth(args.minChildWidth) {
    setChildren(args.children);
}

// FIXME: Remake this as a column of rows
// Will be easier to implement and more flexible
squi::vec2 Grid::Impl::layoutChildren(squi::vec2 maxSize, squi::vec2 minSize, ShouldShrink shouldShrink) {
	auto &children = getChildren();
	
	if (shouldShrink.width) {
		float maxWidth = minSize.x;

		for (auto &child: children) {
			const auto size = child->layout(maxSize, minSize.withY(0), ShouldShrink{true, true});
			maxWidth = std::clamp(size.x, minSize.x, maxSize.x);
		}

		maxSize.x = std::clamp(maxWidth, minSize.x, maxSize.x);
	}

	const float columnCount = shouldShrink.width ? 1 : std::floor(std::max(1.f, maxSize.x / minChildWidth));
	const float columnWidth = (maxSize.x - spacing * (columnCount - 1.f)) / columnCount;

    for (auto &child: getChildren()) {
        const auto childSize = child->layout({columnWidth, maxSize.y}, {columnWidth, 0}, ShouldShrink{false, true});
        minSize.x = std::clamp(childSize.x, minSize.x, maxSize.x);
        minSize.y = std::clamp(childSize.y, minSize.y, maxSize.y);
    }

	return minSize;
}

void Grid::Impl::arrangeChildren(vec2 &pos) {
    const auto childPos = pos + state.margin.getPositionOffset() + state.padding.getPositionOffset();

    const auto contentSize = getContentSize();

    const float columnCount = std::floor(std::max(1.f, contentSize.x / minChildWidth));
    const float columnWidth = (contentSize.x - spacing * (columnCount - 1.f)) / columnCount;

    auto chunks = getChildren() | std::views::chunk(static_cast<size_t>(columnCount));
    float yOffset = 0.f;
    for (auto row : chunks) {
        float rowHeight = 0.f;
        for (auto &child : row) {
            const auto childSize = child->getLayoutSize();
            rowHeight = std::max(rowHeight, childSize.y);
        }

        uint32_t columnIndex = 0;
        for (auto &child : row) {
            const auto childSize = child->getLayoutSize();
            child->arrange({
                childPos.x + std::round(static_cast<float>(columnIndex) * (columnWidth + spacing)),
                childPos.y + std::round(yOffset + (rowHeight - childSize.y) / 2.f),
            });
            ++columnIndex;
        }

        yOffset += rowHeight + spacing;
    }
}