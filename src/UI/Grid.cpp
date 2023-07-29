#include "UI/Grid.hpp"
#include "ranges"
#include <iterator>

using namespace squi;
using namespace Squishy;

Grid::Impl::Impl(const Grid &args) : Widget(args.widget, Widget::Flags::Default()), spacing(args.spacing), minChildWidth(args.minChildWidth) {
    setChildren(args.children);
}

void Grid::Impl::layoutChildren(vec2 &maxSize, vec2 &minSize) {
	vec2 contentSize = maxSize - state.padding.getSizeOffset();
	if (state.sizeMode.width.index() == 0) {
		contentSize.x = std::min(contentSize.x, std::get<0>(state.sizeMode.width));
	} else {
		auto val = std::get<1>(state.sizeMode.width);
		if (val == Size::Shrink) {
			contentSize.x = minSize.x - state.padding.getSizeOffset().x;
		}
	}
	if (state.sizeMode.height.index() == 0) {
		contentSize.y = std::min(contentSize.y, std::get<0>(state.sizeMode.height));
	} else {
		auto val = std::get<1>(state.sizeMode.height);
		if (val == Size::Shrink) {
			contentSize.y = minSize.y - state.padding.getSizeOffset().y;
		}
	}

	const float columnCount = std::floor(std::max(1.f, contentSize.x / minChildWidth));
	const float columnWidth = (contentSize.x - spacing * (columnCount - 1.f)) / columnCount;

    const auto &padding = state.padding.getSizeOffset();
    for (auto &child: getChildren()) {
        const auto childSize = child->layout({columnWidth, contentSize.y});
        minSize.x = std::clamp(childSize.x + padding.x, minSize.x, maxSize.x);
        minSize.y = std::clamp(childSize.y + padding.y, minSize.y, maxSize.y);
    }
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

float Grid::Impl::getMinHeight(const vec2 &maxSize) {
	if (!flags.visible) return 0.0f;
	switch (state.sizeMode.height.index()) {
		case 0: {
			return std::get<0>(state.sizeMode.height) + state.margin.getSizeOffset().y;
		}
		case 1: {
			const auto contentWidth = getContentSize().x;

			const uint32_t columnCount = std::floor(std::max(1.f, contentWidth / minChildWidth));

			std::vector<float> columnHeights(columnCount, 0.f);

			for (auto &child: getChildren()) {
				const auto childSize = child->getMinHeight(maxSize);

				const auto minColumn = std::min_element(columnHeights.begin(), columnHeights.end());

				*minColumn += (*minColumn == 0 ? 0.f : spacing) + childSize;
			}

			return *std::max_element(columnHeights.begin(), columnHeights.end()) + state.padding.getSizeOffset().y;
		}
		default: {
			std::unreachable();
			return 0.0f;
		}
	}
}