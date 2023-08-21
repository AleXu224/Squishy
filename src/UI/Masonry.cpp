#include "UI/Masonry.hpp"
#include "widget.hpp"
#include <algorithm>
#include <cmath>

using namespace squi;
using namespace Squishy;

Masonry::Impl::Impl(const Masonry &args) : Widget(args.widget, Widget::Flags::Default()), spacing(args.spacing), minChildWidth(args.minChildWidth) {
	setChildren(args.children);
}

vec2 Masonry::Impl::layoutChildren(squi::vec2 maxSize, squi::vec2 minSize, ShouldShrink shouldShrink) {
	auto &children = getChildren();

	if (shouldShrink.width) {
		float maxWidth = minSize.x;
		for (auto &child: children) {
			const auto size = child->layout(maxSize, {minSize.x, 0}, ShouldShrink{true, true});
			maxWidth = std::clamp(size.x, minSize.x, maxSize.x);
		}

		maxSize.x = std::clamp(maxWidth, minSize.x, maxSize.x);
	}

	const float columnCount = shouldShrink.width ? 1 : std::floor(std::max(1.f, maxSize.x / minChildWidth));
	const float columnWidth = (maxSize.x - spacing * (columnCount - 1.f)) / columnCount;

	std::vector<float> columnHeights(static_cast<size_t>(columnCount), 0.f);

	for (auto &child: children) {
		const auto minColumn = std::min_element(columnHeights.begin(), columnHeights.end());

		const auto childSize = child->layout({columnWidth, maxSize.y - *minColumn}, {columnWidth, 0}, ShouldShrink{false, true});

		*minColumn += (*minColumn == 0 ? 0.f : spacing) + childSize.y;
	}

	return vec2{
		std::clamp(columnWidth * columnCount + spacing * (columnCount - 1.f), minSize.x, maxSize.x),
		std::clamp(*std::max_element(columnHeights.begin(), columnHeights.end()), minSize.y, maxSize.y),
	};
}

void Squishy::Masonry::Impl::arrangeChildren(squi::vec2 &pos) {
    const auto childPos = pos + state.margin.getPositionOffset() + state.padding.getPositionOffset();

	const auto contentSize = getContentSize();

	const float columnCount = std::floor(std::max(1.f, contentSize.x / minChildWidth));
	const float columnWidth = (contentSize.x - spacing * (columnCount - 1.f)) / columnCount;

	std::vector<float> columnHeights(static_cast<size_t>(columnCount), 0.f);

	for (auto &child: getChildren()) {
		const auto childSize = child->getLayoutSize();

		const auto minColumn = std::min_element(columnHeights.begin(), columnHeights.end());
		const auto columnIndex = std::distance(columnHeights.begin(), minColumn);

		child->arrange({
			childPos.x + std::round(static_cast<float>(columnIndex) * (columnWidth + spacing)),
            childPos.y + std::round(*minColumn + (*minColumn == 0 ? 0.f : spacing)),
		});
        *minColumn += (*minColumn == 0 ? 0.f : spacing) + childSize.y;
	}
}
