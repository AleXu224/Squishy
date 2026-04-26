#pragma once

#include "core/core.hpp"

namespace UI {
	using namespace squi;
	struct Masonry : RenderObjectWidget {
		struct MinSize {
			float value;

			auto operator<=>(const MinSize &) const = default;
		};

		// Args
		Key key;
		Args widget;
		std::variant<int, MinSize> columnCount;
		float spacing = 0.0f;
		Children children;

		struct Element : core::MultiChildRenderObjectElement {
			using core::MultiChildRenderObjectElement::MultiChildRenderObjectElement;

			std::vector<Child> build() override {
				if (auto masonryWidget = std::static_pointer_cast<Masonry>(widget)) {
					return masonryWidget->children;
				}
				return {};
			}
		};

		struct MasonryRenderObject : core::MultiChildRenderObject {
			MasonryRenderObject() : core::MultiChildRenderObject() {}

			std::variant<int, MinSize> columnCount;
			float spacing = 0.0f;
			float columnWidth = 0.f;
			size_t columns = 1;
			std::vector<float> rowHeights{};

			vec2 calculateContentSize(BoxConstraints constraints, bool final) override;
			void positionContentAt(const Rect &newBounds) override;

			[[nodiscard]] size_t computeColumnCount(float availableWidth) const;

			void init() override;
		};

		static std::shared_ptr<RenderObject> createRenderObject() {
			return std::make_shared<MasonryRenderObject>();
		}

		void updateRenderObject(RenderObject *renderObject) const;
	};
}// namespace UI