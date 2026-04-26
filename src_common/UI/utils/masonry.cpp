#include "masonry.hpp"

#include "core/app.hpp"
#include "ranges"

using namespace squi;

[[nodiscard]] size_t UI::Masonry::MasonryRenderObject::computeColumnCount(float availableWidth) const {
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
		columnCount
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
squi::vec2 UI::Masonry::MasonryRenderObject::calculateContentSize(BoxConstraints constraints, bool final) {
	auto columns = computeColumnCount(constraints.shrinkWidth ? 0.f : constraints.maxWidth);
	std::vector<float> columnSizes{};
	columnSizes.resize(columns, 0);
	assert(!columnSizes.empty());

	float maxWidth = 0.f;
	auto newMax = vec2{
		(constraints.maxWidth - static_cast<float>(columns) * spacing) / static_cast<float>(columns),
		constraints.maxHeight,
	};
	auto newMin = vec2{
		std::max((constraints.minHeight - static_cast<float>(columns) * spacing) / static_cast<float>(columns), 0.f),
		0.f,
	};
	for (auto &child: children) {
		auto size = child->calculateSize(
			BoxConstraints{
				.minWidth = newMin.x,
				.maxWidth = newMax.x,
				.minHeight = newMin.y,
				.maxHeight = newMax.y,
				.shrinkWidth = true,
				.shrinkHeight = true,
			},
			false
		);
		maxWidth = std::max(maxWidth, size.x);
	}

	auto maxSize = vec2{maxWidth, constraints.maxHeight};
	auto minSize = vec2{0.f};
	if (!constraints.shrinkWidth) {
		maxSize.x = std::max(maxSize.x, newMax.x);
	}
	for (auto &child: children) {
		auto size = child->calculateSize(
			BoxConstraints{
				.minWidth = minSize.x,
				.maxWidth = maxSize.x,
				.minHeight = minSize.y,
				.maxHeight = maxSize.y,
				.shrinkWidth = false,
				.shrinkHeight = true,
			},
			final
		);
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

void UI::Masonry::MasonryRenderObject::positionContentAt(const Rect &newBounds) {
	auto topLeft = newBounds.getTopLeft();

	std::vector<float> columnHeights{};
	columnHeights.resize(columns, 0.f);
	for (auto &child: children) {
		auto index = getLowestIndex(columnHeights);
		auto &column = columnHeights.at(index);

		if (column != 0.f) column += spacing;

		float xCursor = (columnWidth + spacing) * index;
		child->positionAt(Rect::fromPosSize(topLeft + vec2{xCursor, column}, child->getLayoutRect().size()));
		column += child->getLayoutRect().height();
	}
}


void UI::Masonry::MasonryRenderObject::init() {
	this->getWidgetAs<UI::Masonry>()->updateRenderObject(this);
}

void UI::Masonry::updateRenderObject(RenderObject *renderObject) const {
	// Update render object properties here
	if (auto *gridRenderObject = dynamic_cast<MasonryRenderObject *>(renderObject)) {
		auto *app = gridRenderObject->getApp();
		if (gridRenderObject->columnCount != columnCount) {
			gridRenderObject->columnCount = columnCount;
			app->needsRelayout = true;
		}

		if (gridRenderObject->spacing != spacing) {
			gridRenderObject->spacing = spacing;
			app->needsRelayout = true;
		}
	}
}
