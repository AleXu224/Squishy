#pragma once

#include "widget.hpp"

namespace UI {
	struct Masonry {
		struct MinSize {
			float value;
		};

		// Args
		squi::Widget::Args widget{};
		float spacing = 0.f;
		std::variant<int, MinSize> columnCount;
		squi::Children children{};

		class Impl : public squi::Widget {
			// Data
			float spacing;
			std::variant<int, MinSize> columnCountRule;

			size_t columns = 0;
			float columnWidth = 0;
			
			size_t computeColumnCount(float availableWidth) const;

		public:
			Impl(const Masonry &args);

			squi::vec2 layoutChildren(squi::vec2 maxSize, squi::vec2 minSize, ShouldShrink shouldShrink, bool final) override;
			void arrangeChildren(squi::vec2 &pos) override;
		};

		operator squi::Child() const {
			return std::make_shared<Impl>(*this);
		}
	};
}// namespace UI