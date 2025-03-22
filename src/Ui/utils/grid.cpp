#include "grid.hpp"

#include "ranges"
#include <numeric>

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

	const auto columns = computeColumnCount(shouldShrink.width ? 0.f : maxSize.x);

	std::vector<float> maxHeights{};
	float maxWidth = 0.f;
	float totalHorizontalSpacing = (static_cast<float>(columns) - 1.f) * spacing;
	totalHorizontalSpacing = std::max(totalHorizontalSpacing, 0.f);
	auto newMax = vec2{
		(maxSize.x - totalHorizontalSpacing) / static_cast<float>(columns),
		maxSize.y,
	};
	auto newMin = vec2{
		std::max((minSize.y - totalHorizontalSpacing) / static_cast<float>(columns), 0.f),
		0.f,
	};
	for (const auto &chunk: children | std::views::chunk(columns)) {
		float maxHeight = 0.f;
		for (const auto &child: chunk) {
			auto size = child->layout(newMax, newMin, {false, true}, false);
			maxWidth = std::max(maxWidth, size.x);
			maxHeight = std::max(maxHeight, size.y);
		}
		maxHeights.emplace_back(maxHeight);
	}

	maxSize = {maxWidth, 0.f};
	minSize = {0.f};
	if (!shouldShrink.width) {
		maxSize.x = std::max(maxSize.x, newMax.x);
	}
	for (const auto &[chunk, maxHeight]: std::views::zip(children | std::views::chunk(columns), maxHeights)) {
		maxSize.y = maxHeight;
		for (const auto &child: chunk) {
			child->layout(maxSize, minSize, {false, false}, final);
		}
	}

	size_t rows = (children.size() / columns) + (children.size() % columns == 0 ? 0 : 1);
	rows = std::max(rows, static_cast<size_t>(1));
	float totalVerticalSpacing = (static_cast<float>(rows) - 1.f) * spacing;
	totalVerticalSpacing = std::max(totalVerticalSpacing, 0.f);

	if (final) {
		this->columns = columns;
		this->rowHeights = maxHeights;
	}

	return {
		(maxWidth * static_cast<float>(std::min(columns, children.size()))) + totalHorizontalSpacing,
		std::accumulate(maxHeights.begin(), maxHeights.end(), 0) + totalVerticalSpacing
	};
}

void UI::Grid::Impl::arrangeChildren(squi::vec2 &pos) {
	auto newPos = pos + state.padding->getPositionOffset() + state.margin->getPositionOffset();
	auto &children = getChildren();

	float yCursor = 0.f;
	for (auto [childrenChunk, maxHeight]: std::views::zip(children | std::views::chunk(columns), rowHeights)) {
		float xCursor = 0.f;
		for (auto &child: childrenChunk) {
			child->arrange(newPos + vec2{xCursor, yCursor});
			xCursor += child->getSize().x + spacing;
		}
		yCursor += maxHeight + spacing;
	}
}
