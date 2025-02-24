#include "masonry.hpp"

#include "ranges"

using namespace squi;

UI::Masonry::Impl::Impl(const Masonry &args) : Widget(args.widget, Widget::FlagsArgs::Default()), spacing(args.spacing), columnCountRule(args.columnCount) {
	setChildren(args.children);
}

[[nodiscard]] size_t UI::Masonry::Impl::computeColumnCount(float availableWidth) const {
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

float &getLowest(std::vector<float> &vals) {
	assert(!vals.empty());
	float *lowest = &vals.front();
	for (auto &val: vals) {
		if (val < *lowest) lowest = &val;
	}
	return *lowest;
}

float &getHighest(std::vector<float> &vals) {
	assert(!vals.empty());
	float *highest = &vals.front();
	for (auto &val: vals) {
		if (val > *highest) highest = &val;
	}
	return *highest;
}

size_t getLowestIndex(std::vector<float> &vals) {
	assert(!vals.empty());
	size_t lowest = 0;
	for (auto [index, val]: vals | std::views::enumerate) {
		if (val < vals.at(lowest)) lowest = index;
	}
	return lowest;
}

squi::vec2 UI::Masonry::Impl::layoutChildren(squi::vec2 maxSize, squi::vec2 minSize, ShouldShrink shouldShrink, bool final) {
	auto &children = getChildren();

	auto columns = computeColumnCount(shouldShrink.width ? 0.f : maxSize.x);
	std::vector<float> columnSizes{};
	columnSizes.resize(columns, 0);
	assert(!columnSizes.empty());

	float maxWidth = 0.f;
	auto newMax = vec2{
		(maxSize.x - static_cast<float>(columns) * spacing) / static_cast<float>(columns),
		maxSize.y,
	};
	auto newMin = vec2{
		std::max((minSize.y - static_cast<float>(columns) * spacing) / static_cast<float>(columns), 0.f),
		0.f,
	};
	for (auto &child: children) {
		auto size = child->layout(newMax, newMin, {true, true}, false);
		maxWidth = std::max(maxWidth, size.x);
	}

	maxSize = {maxWidth, maxSize.y};
	minSize = {0.f};
	if (!shouldShrink.width) {
		maxSize.x = std::max(maxSize.x, newMax.x);
	}
	for (auto &child: children) {
		auto size = child->layout(maxSize, minSize, {false, true}, final);
		auto &lowest = getLowest(columnSizes);
		if (lowest != 0.f) lowest += spacing;
		lowest += size.y;
	}

	if (final) {
		this->columns = columns;
		this->columnWidth = maxSize.x;
	}

	return {
		(maxSize.x * static_cast<float>(std::min(columns, children.size()))) + (static_cast<float>(columns - 1) * spacing),
		getHighest(columnSizes),
	};
}

void UI::Masonry::Impl::arrangeChildren(squi::vec2 &pos) {
	auto newPos = pos + state.padding->getPositionOffset() + state.margin->getPositionOffset();
	auto &children = getChildren();

	std::vector<float> columnHeights{};
	columnHeights.resize(columns, 0.f);
	for (auto &child: children) {
		auto index = getLowestIndex(columnHeights);
		auto &column = columnHeights.at(index);

		if (column != 0.f) column += spacing;

		float xCursor = (columnWidth + spacing) * index;
		child->arrange(newPos + vec2{xCursor, column}.rounded());
		column += child->getLayoutSize().y;
	}
}
