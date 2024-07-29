#pragma once

#include "observer.hpp"
#include "widget.hpp"

namespace UI {
	struct TopNav {
		struct Page {
			std::string name;
			squi::Child content;
		};

		// Args
		squi::Widget::Args widget{};
		int64_t startIndex = 0;
		squi::Observable<int64_t> pageChanger{};
		std::vector<Page> pages{};

		struct Storage {
			// Data
			const size_t quota{};
			size_t ready{};
			int64_t currentPage{};
			int64_t maxPage{};
			squi::VoidObserver readyObserver{};
			squi::Observer<int64_t> pageObserver{};
		};

		operator squi::Child() const;
	};
}// namespace UI